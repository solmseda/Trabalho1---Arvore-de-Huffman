#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codifica.h"

/**
 * Função para compactar o arquivo de texto com base na tabela de compactação 
 * e escrever o resultado no arquivo binário.
 * */ 
void compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v) {
    char caractere;
    unsigned int codigo;
    int tamanho;

    while ((caractere = fgetc(arqTexto)) != EOF) {
        // Encontre o caractere na tabela de compactação
        for (int i = 0; i < 32; i++) {
            if (v[i].simbolo == caractere) {
                codigo = v[i].codigo;
                tamanho = v[i].tamanho;
                break;
            }
        }

        // Escreva o código compactado no arquivo binário
        for (int j = tamanho - 1; j >= 0; j--) {
            int bit = (codigo >> j) & 1;
            fputc(bit + '0', arqBin);
        }
    }

    // Adicione o caractere de fim de arquivo (EOT) no final do arquivo binário
    for (int j = v[31].tamanho - 1; j >= 0; j--) {
        int bit = (v[31].codigo >> j) & 1;
        fputc(bit + '0', arqBin);
    }
}

/**
 * Função para descompactar o arquivo binário com base na tabela de compactação.
 *
 * @param arqBin Ponteiro para o arquivo binário de entrada.
 * @param arqTexto Ponteiro para o arquivo de texto de saída.
 * @param v Tabela de compactação com símbolos e códigos.
 */
void descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v) {
    unsigned int codigo = 0;
    int tamanho = 0;
    char bit;

    while ((bit = fgetc(arqBin)) != EOF) {
        // Atualize o código e o tamanho conforme os bits lidos
        codigo = (codigo << 1) | (bit - '0');
        tamanho++;

        // Verifique se o código corresponde a algum caractere na tabela 
        // e escreve no arquivo
        for (int i = 0; i < 32; i++) {
            if (v[i].codigo == codigo && v[i].tamanho == tamanho) {
                fputc(v[i].simbolo, arqTexto);
                codigo = 0;
                tamanho = 0;
                break;
            }
        }
    }
}
