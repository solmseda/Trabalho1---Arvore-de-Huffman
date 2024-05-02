#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codifica.h"

int main() {
    // Open the input text file in read mode
    FILE* arqTexto = fopen("arquivo.txt", "r");
    // Open the output binary file in write mode
    FILE* arqBin = fopen("arquivo.bin", "wb");

    // Create an array of compactadora structs representing the Huffman code table
    struct compactadora tabela[32];
    memset(tabela, 0, sizeof(tabela));

    // Compress the text file and write the compressed data to the binary file
    compacta(arqTexto, arqBin, tabela);

    // Close the input and output files
    fclose(arqTexto);
    fclose(arqBin);

    return 0;
}