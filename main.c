#include <stdio.h>
#include <stdlib.h>
#include "codifica.h"

struct compactadora tabelaCompactacao[] = {
    {'K', 1, 12},
    {'W', 2, 12},
    {'Y', 3, 12},
    {'Z', 2, 11},
    {'J', 3, 11},
    {'X', 1, 9},
    {'\n', 1, 8},
    {'H', 1, 7},
    {'F', 36, 7},
    {'G', 37, 7},
    {'B', 1, 6},
    {'Q', 16, 6},
    {',', 17, 6},
    {'V', 19, 6},
    {'.', 49, 6},
    {'P', 48, 6},
    {'D', 1, 5},
    {'C', 17, 5},
    {'O', 16, 5},
    {'M', 25, 5},
    {'R', 1, 4},
    {'L', 7, 4},
    {'N', 6, 4},
    {'A', 5, 4},
    {'T', 9, 4},
    {'S', 14, 4},
    {'U', 15, 4},
    {'I', 13, 4},
    {'E', 1, 3},
    {' ', 5, 3},
    {0, 0, 0},
    {4, 0, 12}
};

void dumpArqBin(FILE *arqBin) {
    rewind(arqBin);

    // Lê e imprime cada byte
    unsigned char byte;
    while (fread(&byte, sizeof(byte), 1, arqBin)) {
        printf("%02x ", byte);
    }

    printf("\n");
}

int main() {
    FILE *arqTexto, *arqBin, *arqTextoDescompactado;
    
    // Abre o arquivo de texto
    arqTexto = fopen("teste1.txt", "r");
    if (arqTexto == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de texto.\n");
        return 1;
    }

    // Abre o arquivo binário para escrita
    arqBin = fopen("teste1.bin", "wb");
    if (arqBin == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo binário para escrita.\n");
        fclose(arqTexto);
        return 1;
    }

    // Compacta o arquivo de texto
    compacta(arqTexto, arqBin, tabelaCompactacao);

    // Fecha os arquivos
    fclose(arqTexto);
    fclose(arqBin);

    fopen("teste1.bin", "rb");
    dumpArqBin(arqBin);
    fclose(arqBin);

    // Abre o arquivo binário para leitura
    arqBin = fopen("teste1.bin", "rb");
    if (arqBin == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo binário para leitura.\n");
        return 1;
    }

    // Abre o arquivo de texto descompactado para escrita
    arqTextoDescompactado = fopen("teste1descompactado.txt", "w");
    if (arqTextoDescompactado == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de texto descompactado para escrita.\n");
        fclose(arqBin);
        return 1;
    }

    // Descompacta o arquivo binário
    descompacta(arqBin, arqTextoDescompactado, tabelaCompactacao);

    // Fecha os arquivos
    fclose(arqBin);
    fclose(arqTextoDescompactado);

    printf("Processo concluído. Verifique os arquivos gerados.\n");

    return 0;
}
