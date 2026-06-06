cmake_minimum_required(VERSION 3.31)
project(ProjetoTreino C)

set(CMAKE_C_STANDARD 99)

# Caminho ABSOLUTO explícito para headers do sistema
include_directories(/usr/include/)

add_executable(ProjetoTreino
        main.c
        gambiarra.h
        validarEntradaAlfaNumerica.h
        validarEntradaAlfabetica.h
        login.h
)

# Linkar biblioteca mariadb
target_link_libraries(ProjetoTreino PRIVATE mariadb)