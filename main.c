#include <stdio.h>
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

    // Cria a tabela de compactação
    struct compactadora tabela[256];

    // Compacta o arquivo de texto e grava os dados compactados no arquivo binário
    compacta(arqTexto, arqBin, tabela);

    // Fecha os arquivos
    fclose(arqTexto);
    fclose(arqBin);

    return 0;
}