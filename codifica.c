#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codifica.h"

/**
 * Função para compactar o arquivo de texto com base na tabela de compactação 
 * e escrever o resultado no arquivo binário.
 * */ 
void compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v) {
    char buffer = 0;
    int bits_in_buffer = 0;
    char c;
    unsigned int codigo;
    int tamanho = 0;
    int i;

    // ler o arquivo até o final
    while ((c = fgetc(arqTexto)) != EOF) {
        tamanho = 0;

        // procura o caractere atual na interação da tabela de compactação
        for (i = 0; i < 32; i++) {
            if (v[i].simbolo == c) {
                codigo = v[i].codigo;
                tamanho = v[i].tamanho;
                break;
            }
        }

        // Adicionar o código ao buffer
        codigo <<= (32 - tamanho); // Shift left para a posição correta no buffer
        buffer |= (codigo >> bits_in_buffer); // Adicionar os bits ao buffer
        bits_in_buffer += tamanho;

        // Escrever o buffer no arquivo binário quando estiver cheio
        while (bits_in_buffer >= 8) {
            fputc(buffer >> 24, arqBin); // Escrever o byte mais significativo do buffer
            buffer <<= 8;
            bits_in_buffer -= 8;
        }
    }

    // Adicionar o EOT ao final do arquivo binário
    if (bits_in_buffer > 0) {
        fputc(buffer >> 24, arqBin);
    }
    fputc(v[31].codigo, arqBin);

    printf("Compactação concluída.\n");
}

void descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v) {
    printf("\n\nDescompactando... TODO\n");
}