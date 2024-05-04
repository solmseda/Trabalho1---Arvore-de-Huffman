#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codifica.h"

#define MAX_TREE_HT 10000

typedef struct no {
    char simbolo;
    int frequencia;
    struct no *esquerda;
    struct no *direita;
} No;

/**
 * Cria um novo nó com o caractere e a frequência fornecidos.
 */
void criaNo(No **no, char simbolo, int frequencia) {
    *no = (No *)malloc(sizeof(No));
    (*no)->simbolo = simbolo;
    (*no)->frequencia = frequencia;
    (*no)->esquerda = NULL;
    (*no)->direita = NULL;
}

/**
 * Compara dois nós com base em suas frequências.
 */
int comparaNos(const void *a, const void *b) {
    No *no1 = (No *)a;
    No *no2 = (No *)b;
    return no1->frequencia - no2->frequencia;
}

/**
 * Combina dois nós em um novo nó.
*/
No *combinaNos(No *esquerda, No *direita) {
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->frequencia = esquerda->frequencia + direita->frequencia;
    novoNo->esquerda = esquerda;
    novoNo->direita = direita;
    return novoNo;
}

/**
 * Gera o codigo de Huffman para cada caractere na árvore de Huffman.
 *
 * @param no O nó atual na árvore de Huffman.
 * @param v O vetor de struct compactadora para armazenar os códigos de Huffman.
 * @param codigo O código de Huffman atual.
 * @param tamanho O tamanho do código de Huffman atual.
 */
void geraCodigoHuffman(No *no, struct compactadora v[], char codigo[], int tamanho) {
  if (no->esquerda == NULL && no->direita == NULL) {
    v[(int)no->simbolo].codigo = (unsigned int)strtoul(codigo, NULL, 2);
    v[(int)no->simbolo].tamanho = tamanho;
    printf("Código Huffman para '%c': %s (%d bits).\n", no->simbolo, codigo, tamanho);
    return;
    }

  if (no->esquerda != NULL) {
    char novoCodigo[MAX_TREE_HT];
    strcpy(novoCodigo, codigo);
    strcat(novoCodigo, "0");
    geraCodigoHuffman(no->esquerda, v, novoCodigo, tamanho + 1);
  }

  if (no->direita != NULL) {
    char novoCodigo[MAX_TREE_HT];
    strcpy(novoCodigo, codigo);
    strcat(novoCodigo, "1");
    geraCodigoHuffman(no->direita, v, novoCodigo, tamanho + 1);
  }
}


/**
 * Constrói uma árvore de Huffman com base nos símbolos, frequências e tamanho fornecidos.
 *
 * @param simbolos     Um array de símbolos.
 * @param frequencias  Um array de frequências correspondentes aos símbolos.
 * @param tamanho      O tamanho dos arrays.
 * @param v            Um array de struct compactadora para armazenar a árvore de Huffman.
 */
void constroiArvoreHuffman(char simbolos[], int frequencias[], int tamanho, struct compactadora v[]) {
    // Cria uma fila de prioridade para armazenar os nós
    No **fila = (No **)malloc(tamanho * sizeof(No *));
    int indiceFila = 0;

    // Cria um nó para cada caractere e insere na fila de prioridade
    for (int i = 0; i < tamanho; i++) {
    No *no;
    criaNo(&no, simbolos[i], frequencias[i]);
    fila[indiceFila++] = no;
    }

    // Constrói a árvore de Huffman
    while (indiceFila > 1) {
    // Remove os dois nós de menor frequência da fila
    No *no1 = fila[--indiceFila];
    No *no2 = fila[--indiceFila];

    // Combina os nós em um novo nó
    No *novoNo = combinaNos(no1, no2);
    printf("Combinando nós '%c' (%d) e '%c' (%d) para formar novo nó (%d).\n",
            no1->simbolo, no1->frequencia, no2->simbolo, no2->frequencia, novoNo->frequencia);

    // Insere o novo nó na fila de prioridade
    fila[indiceFila++] = novoNo;

    // Atualiza a ordem da fila de acordo com a nova frequência
    qsort(fila, indiceFila, sizeof(No *), comparaNos);
    }

    // Extrai o nó raiz da árvore de Huffman
    No *raiz = fila[0];

    // Gera os códigos de Huffman para cada caractere
    geraCodigoHuffman(raiz, v, "", 0);

    free(fila);
}


/**
 * Compacta o conteúdo de um arquivo de texto usando uma v de compactação.
 *
 * @param arqTexto O ponteiro para o arquivo de texto a ser compactado.
 * @param arqBin O ponteiro para o arquivo binário onde o conteúdo compactado será armazenado.
 * @param v vetor de 32 posições do tipo da estrutura representada anteriormente
 */
void compacta(FILE* arqTexto, FILE* arqBin, struct compactadora *v) {
    char simbolos[MAX_TREE_HT];
    int frequencias[MAX_TREE_HT] = {0};
    char simbolo;
    int tamanho = 0;

    while ((simbolo = fgetc(arqTexto)) != EOF) {
        simbolos[tamanho] = simbolo;
        frequencias[tamanho]++;
        tamanho++;
    }

    struct compactadora fim_arquivo;
    fim_arquivo.simbolo = 31;
    fim_arquivo.codigo = v[31].codigo;
    fim_arquivo.tamanho = v[31].tamanho;
    v[31] = fim_arquivo;

    printf("Construindo árvore de Huffman.\n");
    constroiArvoreHuffman(simbolos, frequencias, tamanho, v);
    
    printf("Rebobinando arquivo de texto.\n");

    rewind(arqTexto);

    char buffer[MAX_TREE_HT];
    memset(buffer, '\0', sizeof(buffer));
    int indice_buffer = 0;

    printf("Escrevendo códigos de Huffman no arquivo binário.\n");
    while ((simbolo = fgetc(arqTexto)) != EOF) {
        int codigo = v[(int)simbolo].codigo;
        int tamanho = v[(int)simbolo].tamanho;
        int i;
        for (i = 0; i < tamanho; i++) {
            int bit = (codigo >> (tamanho - i - 1)) & 1;
            buffer[indice_buffer] = bit + '0';
            indice_buffer++;
            if (indice_buffer == 8) {
                fwrite(buffer, 1, sizeof(buffer), arqBin);
                memset(buffer, '\0', sizeof(buffer));
                indice_buffer = 0;
            }
        }
    }

    int codigo_eot = v[31].codigo;
    int tamanho_eot = v[31].tamanho;
    for (int i = 0; i < tamanho_eot; i++) {
        int bit = (codigo_eot >> (tamanho_eot - i - 1)) & 1;
        buffer[indice_buffer] = bit + '0';
        indice_buffer++;
        if (indice_buffer == 8) {
            fwrite(buffer, 1, sizeof(buffer), arqBin);
            memset(buffer, '\0', sizeof(buffer));
            indice_buffer = 0;
        }
    }

    if (indice_buffer != 0) {
        fwrite(buffer, 1, sizeof(buffer), arqBin);
    }

    printf("Compactação concluída.\n");

    // Print the content of arqBin in hexadecimal
    rewind(arqBin);
    printf("Conteúdo do arquivo binário em hexadecimal:\n");
    unsigned char byte;
    while (fread(&byte, sizeof(byte), 1, arqBin) == 1) {
        printf("%02X ", byte);
    }
    printf("\n");
}