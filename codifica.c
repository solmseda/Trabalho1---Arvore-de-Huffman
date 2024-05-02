#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codifica.h"

#define MAX_TREE_HT 100

struct No {
    char simbolo;
    unsigned int frequencia;
    struct No *esquerda, *direita;
};

struct Heap {
    unsigned int tamanho;
    unsigned int capacidade;
    struct No** array;
};

/**
 * Cria um novo nó com o símbolo e a frequência fornecidos.
 *
 * @param simbolo O símbolo a ser armazenado no nó.
 * @param frequencia A frequência associada ao símbolo.
 * @return Um ponteiro para o novo nó criado.
 */
struct No* novoNo(char simbolo, unsigned int frequencia) {
    struct No* temp = (struct No*)malloc(sizeof(struct No));
    temp->simbolo = simbolo;
    temp->frequencia = frequencia;
    temp->esquerda = temp->direita = NULL;
    return temp;
}

/**
 * Cria um novo heap com a capacidade especificada.
 *
 * @param capacidade A capacidade do heap.
 * @return Um ponteiro para o heap recém-criado.
 */
struct Heap* criarHeap(unsigned int capacidade) {
    struct Heap* heap = (struct Heap*)malloc(sizeof(struct Heap));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    heap->array = (struct No**)malloc(heap->capacidade * sizeof(struct No*));
    return heap;
}

/**
 * Troca as posições de dois nós em um heap.
 *
 * @param a Ponteiro para o primeiro nó.
 * @param b Ponteiro para o segundo nó.
 */
void trocarNos(struct No** a, struct No** b) {
    struct No* temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Função para manter a propriedade de min heap na árvore.
 * Verifica se o nó no índice fornecido viola a propriedade de min heap (ou seja, se é maior que qualquer um de seus filhos). 
 * Se violar, troca o nó com o menor de seus filhos e, em seguida, chama recursivamente a função no nó trocado.
 *
 * @param heap Ponteiro para a estrutura do heap.
 * @param indice Índice do nó que será verificado.
 */
void minHeapify(struct Heap* heap, int indice) {
    int menor = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;

    if (esquerda < heap->tamanho && heap->array[esquerda]->frequencia < heap->array[menor]->frequencia)
        menor = esquerda;

    if (direita < heap->tamanho && heap->array[direita]->frequencia < heap->array[menor]->frequencia)
        menor = direita;

    if (menor != indice) {
        trocarNos(&heap->array[menor], &heap->array[indice]);
        minHeapify(heap, menor);
    }
}

int tamanhoUm(struct Heap* heap) {
    return (heap->tamanho == 1);
}

/**
 * Extrai o elemento mínimo do heap.
 * 
 * Esta função remove e retorna o elemento mínimo do heap.
 * Ela atualiza a estrutura do heap substituindo o elemento raiz pelo último elemento
 * e em seguida reorganiza o heap para manter a propriedade de min-heap.
 * 
 * @param heap O heap do qual extrair o elemento mínimo.
 * @return O elemento mínimo extraído do heap.
 */
struct No* extraiMin(struct Heap* heap) {
    struct No* temp = heap->array[0];
    heap->array[0] = heap->array[heap->tamanho - 1];
    --heap->tamanho;
    minHeapify(heap, 0);
    return temp;
}

/**
 * Insere um nó em um min heap.
 *
 * @param heap O heap onde o nó será inserido.
 * @param node O nó a ser inserido.
 */
void insereMinHeap(struct Heap* heap, struct No* node) {
    ++heap->tamanho;
    int i = heap->tamanho - 1;
    while (i && node->frequencia < heap->array[(i - 1) / 2]->frequencia) {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i] = node;
}

/**
 * Constrói um heap mínimo a partir do heap fornecido.
 *
 * @param heap O heap a partir do qual construir o heap mínimo.
 */
void constroiMinHeap(struct Heap* heap) {
    int n = heap->tamanho - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i) {
        minHeapify(heap, i);
    }
}

/**
 * Constrói uma árvore de Huffman com base nos símbolos e frequências fornecidos.
 * 
 * @param simbolos O array de símbolos.
 * @param frequencias O array de frequências correspondentes aos símbolos.
 * @param tamanho O tamanho dos arrays.
 * @param tabela A struct compactadora para armazenar os códigos de Huffman.
 */
void constroiArvoreHuffman(char simbolos[], int frequencias[], int tamanho, struct compactadora* tabela) {
    struct No *esquerda, *direita, *top;
    struct Heap* heap = criarHeap(tamanho);

    for (int i = 0; i < tamanho; ++i)
        heap->array[i] = novoNo(simbolos[i], frequencias[i]);

    heap->tamanho = tamanho;
    constroiMinHeap(heap);

    while (!tamanhoUm(heap)) {
        esquerda = extraiMin(heap);
        direita = extraiMin(heap);
        top = novoNo('$', esquerda->frequencia + direita->frequencia);
        top->esquerda = esquerda;
        top->direita = direita;
        insereMinHeap(heap, top);
    }

    constroiTabelaCompactacao(heap->array[0], tabela, "", 0);
}

/**
 * Constrói uma tabela de compactação com base em uma árvore binária fornecida.
 * 
 * @param raiz A raiz da árvore binária.
 * @param tabela A tabela de compactação a ser construída.
 * @param codigo O código binário que representa o caminho da raiz até um nó folha.
 * @param indice O índice atual no código binário.
 */
void constroiTabelaCompactacao(struct No* raiz, struct compactadora* tabela, char* codigo, int indice) {
    if (raiz->esquerda) {
        codigo[indice] = '0';
        constroiTabelaCompactacao(raiz->esquerda, tabela, codigo, indice + 1);
    }

    if (raiz->direita) {
        codigo[indice] = '1';
        constroiTabelaCompactacao(raiz->direita, tabela, codigo, indice + 1);
    }

    if (!raiz->esquerda && !raiz->direita) {
        tabela[raiz->simbolo].simbolo = raiz->simbolo;
        tabela[raiz->simbolo].codigo = atoi(codigo);
        tabela[raiz->simbolo].tamanho = indice;
    }
}

/**
 * @brief Compacta um arquivo de texto usando a codificação de Huffman e escreve os dados compactos em um arquivo binário.
 * 
 * @param arqTexto Ponteiro para o arquivo de texto de entrada.
 * @param arqBin Ponteiro para o arquivo binário de saída.
 * @param tabela Array de structs compactadora representando a tabela de códigos de Huffman.
 */
void compacta(FILE* arqTexto, FILE* arqBin, struct compactadora* tabela) {
    // Inicialia os arrays de símbolos e frequências
    char simbolos[MAX_TREE_HT];
    int frequencias[MAX_TREE_HT] = {0};
    char simbolo;
    int tamanho = 0;

    // conta a frequência de cada símbolo no arquivo de texto
    while ((simbolo = fgetc(arqTexto)) != EOF) {
        if (simbolo == '\n') {
            simbolos[tamanho] = simbolo;
            frequencias[tamanho]++;
            tamanho++;
        } else {
            simbolos[tamanho] = simbolo;
            frequencias[tamanho]++;
            tamanho++;
        }
    }

    // cria um símbolo de fim de arquivo (EOT) e atualiza a tabela de códigos de Huffman
    struct compactadora fim_arquivo;
    fim_arquivo.simbolo = 31;  // Caractere de fim de arquivo (EOT)
    fim_arquivo.codigo = tabela[31].codigo;
    fim_arquivo.tamanho = tabela[31].tamanho;
    tabela[31] = fim_arquivo;

    // constrói a árvore de Huffman e a tabela de códigos de Huffman
    constroiArvoreHuffman(simbolos, frequencias, tamanho, tabela);

    // retorna ao início do arquivo de texto
    rewind(arqTexto);

    // inicializa um buffer de 8 bits para armazenar os códigos de Huffman
    char buffer[MAX_TREE_HT];
    memset(buffer, '\0', sizeof(buffer));
    int indice_buffer = 0;

    // escreve os códigos de Huffman no arquivo binário
    while ((simbolo = fgetc(arqTexto)) != EOF) {
        int codigo = tabela[simbolo].codigo;
        int tamanho = tabela[simbolo].tamanho;
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

    // escreve o código de fim de arquivo (EOT) no arquivo binário
    int codigo_eot = tabela[31].codigo;
    int tamanho_eot = tabela[31].tamanho;
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

    // escreve o buffer restante no arquivo binário
    if (indice_buffer != 0) {
        fwrite(buffer, 1, sizeof(buffer), arqBin);
    }
}
