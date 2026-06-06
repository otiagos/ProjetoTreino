//
// Created by tiago on 16/08/2025.
//

#ifndef PROJETOTREINO_VALIDARENTRADADEDADOS_H
#define PROJETOTREINO_VALIDARENTRADADEDADOS_H

#include <ctype.h>
#include <stdbool.h>

bool validarEntradaAlfaNumerica(const char []);

inline bool validarEntradaAlfaNumerica(const char dados[]) {
    bool isNotValid = false;
    int flag = 1;

    for (int i = 0; dados[i] != '\0' && flag; i++) {
        if (!isalnum(dados[i])) {
            isNotValid = true;
            flag = 0;
        }
    }

    return isNotValid;
}

#endif //PROJETOTREINO_VALIDARENTRADADEDADOS_H