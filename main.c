#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <mariadb/mysql.h>
#include "gambiarra.h"
#include "validarEntradaAlfaNumerica.h"
#include "validarEntradaAlfabetica.h"
#include "login.h"

#ifndef SIZE
#define SIZE 76
#else
#error Error to create macro SIZE
#endif

void insertTutorData(char[], char[], char[]);
void insertCatData(char[], char[], char[]);
void insertVeterinary(char[], char[], char[]);
void fetchCat(char[], char[], char[]);
void listTutor(char[], char[], char[]);
void appointment(void);
void instructions(void);

int main(void) {
    MYSQL *conn;
    char userName[SIZE], password[SIZE] = {};
    int option = 0;
    bool isNotValid;

    createFile();

    if ('\n')
        getchar();

    printf("%s", "\nMySQL/API Connector C\n\n");

    do {
        isNotValid = false;
        memset(userName, 0, sizeof(userName));

        printf("Usuário:");
        fgets(userName, SIZE, stdin);
        userName[strlen(userName) - 1] = '\0';

        isNotValid = validarEntradaAlfaNumerica(userName);

        if (isNotValid)
        {
            memset(userName, 0, sizeof(userName));
            printf("Caracteres inválidos, digite novamente.\n");
        }

    }while (isNotValid);

    printf("Senha:");

    struct termios old, new;

    if (tcgetattr (fileno (stdin), &old) != 0)
        return -1;
    new = old;
    new.c_lflag &= ~ECHO;
    if (tcsetattr (fileno (stdin), TCSAFLUSH, &new) != 0)
        return -1;

    fgets(password, SIZE, stdin);
    password[strlen(password) - 1] = '\0';

    (void) tcsetattr (fileno (stdin), TCSAFLUSH, &old);

    if (!((conn = mysql_init(0)))) {
        fprintf(stderr, "\nErro ao inicializar estrutura de conexão.\n");
    }

    if (!mysql_real_connect(conn, "127.0.0.1", userName, password, "DB", 3306, NULL,
        0)) {
        fprintf(stderr, "\nErro de conexão ao servidor: %s.\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    system("clear");

    printf("Versão do servidor:%s\n", mysql_get_server_info(conn));
    printf("Versão do protocolo:%u\n", mysql_get_proto_info(conn));
    printf("Ping:%.2f\n", (double)mysql_ping(conn));

    printf("\nConectado\n");

    do{
        instructions();
        printf("%s", "Opção:");
        scanf("%d", &option);

        switch (option) {
            case 1:
                insertTutorData(userName, password, "DB");
                break;
            case 2:
                insertCatData(userName, password, "DB");
                break;
            case 3:
                insertVeterinary(userName, password, "DB");
                break;
            case 4:
                fetchCat(userName, password, "DB");
                break;
            case 5:
                listTutor(userName, password, "DB");
                break;
            case 6:
                printf("%s", "\nFim do programa\n\n");
                break;
            default:
                system("clear");

                printf("Opção Inválida.\n");
                break;
        }

    }while (option != 6);

    mysql_close(conn);
    mysql_library_end();

    return 0;
}

void insertTutorData(char user[], char pwd[], char database[]) {
    system("clear");

    bool isNotValid;

    MYSQL *connData;

    if (!((connData = mysql_init(0)))) {
        fprintf(stderr, "Erro ao inicializar a estrutura de conexão.\n");
        exit(1);
    }

    if (!mysql_real_connect(connData, "localhost", user, pwd, database, 3306, NULL, 0)) {
        fprintf(stderr, "Erro de conexão ao servidor: %s.\n", mysql_error(connData));
        mysql_close(connData);
        exit(1);
    }

    char cpf[12] = {0}, nome[SIZE] = {0}, telefone[12] = {0}, endereco[SIZE] = {0}, str[249] = {0};

    getchar();

    printf("CPF:");
    fgets(cpf, 12, stdin);
    cpf[strlen(cpf)] = '\0';

    getchar();

    do {
        isNotValid = false;
        memset(nome, 0, sizeof(nome));

        printf("Nome:");
        fgets(nome, SIZE, stdin);
        nome[strlen(nome) - 1] = '\0';

        isNotValid = validarEntradaAlfabetica(nome);

        if (isNotValid)
            printf("Caracteres inválidos. Digite novamente.\n");
    }while (isNotValid);

    gambiarra(nome);
    nome[strlen(nome)] = '\0';

    printf("Telefone:");
    fgets(telefone, 12, stdin);
    telefone[strlen(telefone)] = '\0';

    getchar();

    printf("Endereço:");
    fgets(endereco, SIZE, stdin);
    gambiarra(endereco);
    endereco[strlen(endereco)] = '\0';

    if (sprintf(str, "INSERT INTO TUTOR (CPF, NOME, TELEFONE, ENDERECO) VALUES ('%s', '%s', '%s', '%s')",
        cpf, nome, telefone, endereco) < 0) {
        printf("Erro ao atribuir string ao vetor 'str'.\n");
    }

    if (mysql_query(connData, str) != 0)
        fprintf(stderr, "Erro: %s", mysql_error(connData));
    else
        printf("Dados inseridos com sucesso.\n");

    mysql_close(connData);
}

void fetchCat(char user[], char pwd[], char database[]) {
    system("clear");

    MYSQL *connFetch;

    if (!((connFetch = mysql_init(0)))) {
        fprintf(stderr, "Erro ao inicializar estrutura de conexão.\n");
    }

    if (!mysql_real_connect(connFetch, "localhost", user, pwd, database, 3306, NULL, 0)) {
        fprintf(stderr, "Erro ao conectar ao servidor: %s\n.", mysql_error(connFetch));
        mysql_close(connFetch);
        exit(1);
    }

    char buscarCpf[12] = {0}, str[51] = {0};

    getchar();

    printf("Digite o CPF para busca:");
    fgets(buscarCpf, 12, stdin);
    buscarCpf[strlen(buscarCpf)] = '\0';

    system("clear");

    sprintf(str, "SELECT * FROM GATO WHERE CPF_TUTOR = '%s'", buscarCpf);

    mysql_query(connFetch, str);

    MYSQL_RES *result = mysql_store_result(connFetch);

    if (result == NULL)
        fprintf(stderr, "Erro: %s", mysql_error(connFetch));

    const unsigned int num_fields = mysql_num_fields(result);

    if (!mysql_affected_rows(connFetch))
        printf("\nO CPF informado não existe.\n");
    else {
        MYSQL_ROW row;
        printf("%-4s%14s%73s%19s", "\nCPF do Tutor", "Nome do Gato", "Raça", "Idade\n");

        while ((row = mysql_fetch_row(result))) {
            const unsigned long *lengths = mysql_fetch_lengths(result);

            for (unsigned int i = 0; i < num_fields; i++) {
                printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
            }

            printf("\n");
        }

        printf("Query OK. %llu linha(s) afetada(s).\n", mysql_affected_rows(connFetch));
    }

    mysql_free_result(result);
    mysql_close(connFetch);
}

void insertCatData(char user[], char pwd[], char database[]) {
    system("clear");

    MYSQL *connCat;
    char resposta = '\0';

    if (!((connCat = mysql_init(0))))
        fprintf(stderr, "Erro ao inicializar estrutura de conexão\n");

    if (!mysql_real_connect(connCat, "localhost", user, pwd, database, 3306, NULL, 0))
        fprintf(stderr, "Erro ao conectar ao servidor: %s\n", mysql_error(connCat));

    char nomeGato[SIZE] = {0}, cpfTutor[12] = {0}, racaGato[75] = {0}, idade[3] = {0}, str[238] = {0};

    getchar();

    printf("CPF do tutor:");
    fgets(cpfTutor, 12, stdin);
    cpfTutor[strlen(cpfTutor)] = '\0';

    getchar();

    printf("Nome do gato:");
    fgets(nomeGato, SIZE, stdin);
    gambiarra(nomeGato);
    nomeGato[strlen(nomeGato)] = '\0';

    printf("Raça:");
    fgets(racaGato, 15, stdin);
    gambiarra(racaGato);
    racaGato[strlen(racaGato)] = '\0';

    printf("Idade:");
    fgets(idade, 2, stdin);
    idade[strlen(idade)] = '\0';

    if (sprintf(str, "INSERT INTO GATO (CPF_TUTOR, NOME, RACA, IDADE) VALUES ('%s', '%s', '%s', '%s')", cpfTutor,
        nomeGato, racaGato, idade) < 0)
        printf("Erro ao atribuir string ao vetor 'str'.\n");

    if (mysql_query(connCat, str) == 0) {
        system("clear");
        printf("\nDados inseridos com sucesso.\n");
    }
    else
        fprintf(stderr, "Erro:%s\n", mysql_error(connCat));

    printf("\n%llu linhas afetadas.\n", mysql_affected_rows(connCat));

    getchar();

    printf("\n\nMarcar consulta?(S/N)");

    do {
        resposta = (char) getchar();

        if (resposta != 's' && resposta != 'n' && resposta != 'S' && resposta != 'N')
            printf("Opção inválida, digite novamente.\n");

        getchar();
    }while (resposta != 's' && resposta != 'n' && resposta != 'S' && resposta != 'N');

    if (resposta == 'S' || resposta == 's')
        appointment();
    else
        printf("Operação cancelada pelo usuário.\n\n");

    mysql_close(connCat);
}

void insertVeterinary(char user[], char pwd[], char database[]) {
    system("clear");

    MYSQL *connVet;

    if (!((connVet = mysql_init(0)))) {
        fprintf(stderr, "Erro ao inicializar estrutura de conexão.\n");
        mysql_close(connVet);
        exit(1);
    }

    if (!mysql_real_connect(connVet, "localhost", user, pwd, database, 3306, NULL, 0))
        fprintf(stderr, "Erro ao conectar ao servidor:%s\n", mysql_error(connVet));

    char nomeVeterinario[SIZE] = {0}, cpf[12] = {0}, crmv[21] = {0}, str[175] = {0};

    getchar();

    printf("Nome:");
    fgets(nomeVeterinario, SIZE, stdin);
    gambiarra(nomeVeterinario);
    nomeVeterinario[strlen(nomeVeterinario) - 1] = '\0';

    printf("CPF:");
    fgets(cpf, 12, stdin);
    cpf[strlen(cpf) - 1] = '\0';

    getchar();

    printf("CRMV:");
    fgets(crmv, 21, stdin);
    crmv[strlen(crmv) - 1] = '\0';

    if (sprintf(str,  "INSERT INTO VETERINARIO(NOME, CPF, CRMV) VALUES ('%s', '%s', '%s')",
        nomeVeterinario, cpf, crmv) < 0)
        printf("Erro ao atribuir string à variável 'str'.\n");

    if (mysql_query(connVet, str) == 0) {
        system("clear");
        printf("\nDados inseridos com sucesso.\n");
    }
    else {
        fprintf(stderr, "\nErro:%s\n", mysql_error(connVet));
    }

    printf("\n%llu linhas afetadas.\n", mysql_affected_rows(connVet));

    mysql_close(connVet);
}

void listTutor(char user[], char pwd[], char database[]) {
    system("clear");

    MYSQL *connList;

    if (!((connList = mysql_init(0)))) {
        fprintf(stderr, "Erro ao inicializar estrutura de conexão.\n");
        exit(1);
    }

    if (!mysql_real_connect(connList, "localhost", user, pwd, database, 3306, NULL, 0)) {
        fprintf(stderr, "Erro ao conectar ao servidor: %s.\n", mysql_error(connList));
        mysql_close(connList);
        exit(1);
    }

    MYSQL_ROW row;
    char selectArray[SIZE] = {};

    sprintf(selectArray, "SELECT * FROM TUTOR");

    mysql_query(connList, selectArray);

    MYSQL_RES *result = mysql_store_result(connList);

    if (result == NULL)
        fprintf(stderr, "Erro: %s\n", mysql_error(connList));

    const unsigned int num_fields = mysql_num_fields(result);

    printf("%-4s%15s%83s%16s", "\nCPF", "Nome", "Telefone", "Endereço\n");

    while ((row = mysql_fetch_row(result))) {
        const unsigned long *lengths = mysql_fetch_lengths(result);

        for (unsigned int i = 0; i < num_fields; i++) {
            printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }

    printf("Query OK. %lu linha(s) afetada(s).\n", (long) mysql_affected_rows(connList));

    mysql_free_result(result);
    mysql_close(connList);
}

void appointment(void) {

}

void instructions(void) {
    printf("\nMenu:\n"
           "(1) Cadastrar tutor\n"
           "(2) Cadastrar gato\n"
           "(3) Cadastrar veterinário\n"
           "(4) Buscar gato\n"
           "(5) Listar tutores cadastrados\n"
           "(6) Sair do programa\n");
}