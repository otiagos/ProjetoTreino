//
// Created by tiago on 16/08/2025.
//
#pragma once

#include <stdbool.h>
#include <ctype.h>

#ifndef PROJETOTREINO_VALIDARENTRADAALFABETICA_H
#define PROJETOTREINO_VALIDARENTRADAALFABETICA_H
bool validarEntradaAlfabetica(const char []);

inline bool validarEntradaAlfabetica(const char dados[]) {
    bool isValid = false;
    int flag = 1;

    for (int i = 0; dados[i] != '\0' && flag; i++) {
        if (!isalpha(dados[i]) && dados[i] != ' ') {
            isValid = true;
            flag = 0;
        }
    }

    return isValid;
}

#endif //PROJETOTREINO_VALIDARENTRADAALFABETICA_H