//
// Created by tiago on 16/08/2025.
//
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#define TAM 16

typedef struct login {
    long id;
    char usuario[TAM];
    char senha[TAM];
}Login;

void createFile(void);
void createAccount(void);
bool logAccount(void);
void options(void);

inline void createFile(void) {
    system("clear");

    FILE *filePtr = NULL;
    Login login, *ptrLogin = &login;

    if ((filePtr = fopen("/home/tiago/logins.dat", "rb")) != NULL)
        options();
    else {
        if ((filePtr = fopen("/home/tiago/logins.dat", "wb")) == NULL) {
            perror("Erro ao criar arquivo.\n");
            exit(1);
        }

        for (int i = 1; i <= 1000; i++) {
            ptrLogin->id = 0;
            ptrLogin->usuario[0] = '\0';
            ptrLogin->senha[0] = '\0';

            fwrite(ptrLogin, sizeof(struct login), 1, filePtr);
        }

        printf("\nArquivo criado com sucesso.\n");
        fclose(filePtr);
        exit(0);
    }
    fclose(filePtr);
}

inline void createAccount(void) {
    system("clear");

    getchar();

    FILE *ptrAcc = NULL;
    Login login, *ptrLogin = &login;

    if ((ptrAcc = fopen("/home/tiago/logins.dat", "rb+")) != NULL) {
        long id = 1;

        fseek(ptrAcc, (id - 1) * (long) sizeof(struct login), SEEK_SET);

        fread(ptrLogin, sizeof(struct login), 1, ptrAcc);
        while (ptrLogin->id != 0) {
            id++;

            fseek(ptrAcc, (id - 1) * (long) sizeof(struct login), SEEK_SET);

            fread(ptrLogin, sizeof(struct login), 1, ptrAcc);
        }

        printf("Login:");
        fgets(ptrLogin->usuario, TAM, stdin);
        ptrLogin->usuario[strlen(ptrLogin->usuario)] = '\0';
        strtok(ptrLogin->usuario, "\n");

        printf("Senha (máximo 15 caracteres):");

        struct termios old, new;

        if (tcgetattr(fileno(stdin), &old) != 0)
            exit(1);
        new = old;
        new.c_lflag &= ~ECHO;
        if (tcsetattr(fileno(stdin), TCSAFLUSH, &new) != 0)
            exit(1);

        fgets(ptrLogin->senha, TAM, stdin);
        ptrLogin->senha[strlen(ptrLogin->senha)] = '\0';
        strtok(ptrLogin->senha, "\n");

        (void) tcsetattr(fileno(stdin), TCSAFLUSH, &old);

        ptrLogin->id = id;

        fseek(ptrAcc, (ptrLogin->id - 1) * (long) sizeof(Login), SEEK_SET);

        if (fwrite(ptrLogin, sizeof(struct login), 1, ptrAcc))
            printf("\nDados armazenados com sucesso.\n");
        else
            printf("Erro ao armazenar os dados.\n");

        fclose(ptrAcc);
    }
    else {
        fprintf(stderr, "Erro ao abrir arquivo.");
        exit(1);
    }
}

inline bool logAccount(void) {
    system("clear");

    char setLogin[TAM] = {0}, senha[TAM] = {0};
    FILE *ptrLog = NULL;
    Login login, *ptrLogin = &login;
    bool isValid = false;

    getchar();

    if ((ptrLog = fopen("/home/tiago/logins.dat", "rb")) != NULL) {
        printf("Login:");
        fgets(setLogin, TAM, stdin);
        setLogin[strlen(setLogin)] = '\0';
        strtok(setLogin, "\n");

        printf("Senha:");

        struct termios old, new;

        if (tcgetattr(fileno(stdin), &old) != 0)
            exit(1);
        new = old;
        new.c_lflag &= ~ECHO;

        if (tcsetattr(fileno(stdin), TCSAFLUSH, &new) != 0)
            exit(1);

        fgets(senha, TAM, stdin);
        senha[strlen(senha)] = '\0';
        strtok(senha, "\n");

        (void) tcsetattr(fileno(stdin), TCSAFLUSH, &old);

        size_t result = fread(ptrLogin, sizeof(Login), 1, ptrLog);

        while (!feof(ptrLog) && result != 0) {
            if (strcmp(ptrLogin->usuario, setLogin) == 0 && strcmp(ptrLogin->senha, senha) == 0)
                isValid = true;

            result = fread(ptrLogin, sizeof(Login), 1, ptrLog);
        }
    }
    else {
        printf("Não foi possível abrir o arquivo logins.dat\n");
        exit(1);
    }

    fclose(ptrLog);
    return isValid;
}

inline void options(void) {
    int option;
    bool isValid = false;

    do {
        printf("(1) Logar conta\n"
               "(2) Criar conta\n"
               "(3) Sair do programa\n");
        printf("%s", "? ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                isValid = logAccount();

                if (!isValid) {
                    system("clear");
                    printf("\nConta não encontrada.\n\n");
                }
                else {
                    system("clear");
                    printf("\nLogin realizado com sucesso.\n");
                }
                break;
            case 2:
                createAccount();
                break;
            case 3:
                exit(0);
            default:
                system("clear");
                printf("\nOpção inválida, digite novamente.\n");
        }
    }while (option != 3 && !isValid);
}