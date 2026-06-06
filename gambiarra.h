//
// Created by tiago on 26/07/25.
//

#ifndef GAMBIARRA_H
#define GAMBIARRA_H

char *gambiarra(char []);

inline char *gambiarra(char vetor[]) {
    size_t i = strlen(vetor);

    for (; i < 76; i++) {
        vetor[i] = ' ';
    }
    vetor[i] = '\0';

    for (int j = 0; j < strlen(vetor); j++) {
        if (vetor[j] == '\n')
            vetor[j] = ' ';
    }

    return vetor;
}
#endif //GAMBIARRA_H
