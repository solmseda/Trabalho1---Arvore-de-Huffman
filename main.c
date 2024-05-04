#include <stdio.h>
#include <stdlib.h>
#include "codifica.h"

int main() {
    // Abre o arquivo de texto para leitura
    FILE* arqTexto = fopen("teste1.txt", "r");
    if (arqTexto == NULL) {
        printf("Não foi possível abrir o arquivo de texto para leitura.\n");
        return 1;
    }

    // Abre o arquivo binário para escrita
    FILE* arqBin = fopen("teste1.bin", "wb");
    if (arqBin == NULL) {
        printf("Não foi possível abrir o arquivo binário para escrita.\n");
        return 1;
    }

    // Aloca dinamicamente a tabela de compactação
    struct compactadora *v = (struct compactadora*)malloc(32 * sizeof(struct compactadora));
    if (v == NULL) {
        printf("Erro ao alocar memória para a tabela de compactação.\n");
        return 1;
    }

    // Compacta o arquivo de texto e grava os dados compactados no arquivo binário
    compacta(arqTexto, arqBin, v);

    // Libera a memória alocada para a tabela
    free(v);

    // Fecha os arquivos
    fclose(arqTexto);
    fclose(arqBin);

    return 0;
}
