// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

// Função para leitura de matriz a partir de arquivo binário
void lerMatriz(char *nomeArquivo, float **matriz, int *linhas, int *colunas) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nomeArquivo);
        exit(1);
    }

    // Lê dimensões da matriz
    fread(linhas, sizeof(int), 1, arquivo);
    fread(colunas, sizeof(int), 1, arquivo);

    // Aloca memória para a matriz
    *matriz = (float *)malloc((*linhas) * (*colunas) * sizeof(float));
    if (*matriz == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a matriz\n");
        exit(1);
    }

    // Lê os elementos da matriz
    fread(*matriz, sizeof(float), (*linhas) * (*colunas), arquivo);

    fclose(arquivo);
}

// Função para escrita de matriz em arquivo binário
void escreverMatriz(char *nomeArquivo, float *matriz, int linhas, int colunas) {
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s para escrita\n", nomeArquivo);
        exit(1);
    }

    // Escreve dimensões da matriz
    fwrite(&linhas, sizeof(int), 1, arquivo);
    fwrite(&colunas, sizeof(int), 1, arquivo);

    // Escreve os elementos da matriz
    fwrite(matriz, sizeof(float), linhas * colunas, arquivo);

    fclose(arquivo);
}

// Função para multiplicação de matrizes
void multiplicarMatrizes(float *matrizA, int linhasA, int colunasA,
                         float *matrizB, int linhasB, int colunasB,
                         float **resultado) {
    // Verifica se as matrizes podem ser multiplicadas
    if (colunasA != linhasB) {
        fprintf(stderr, "Não é possível multiplicar as matrizes: dimensões incompatíveis\n");
        exit(1);
    }

    // Aloca memória para a matriz resultado
    *resultado = (float *)malloc(linhasA * colunasB * sizeof(float));
    if (*resultado == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a matriz resultado\n");
        exit(1);
    }

    // Multiplica as matrizes
    for (int i = 0; i < linhasA; i++) {
        for (int j = 0; j < colunasB; j++) {
            (*resultado)[i * colunasB + j] = 0;
            for (int k = 0; k < colunasA; k++) {
                (*resultado)[i * colunasB + j] += matrizA[i * colunasA + k] * matrizB[k * colunasB + j];
            }
        }
    }
}

int main(int argc, char *argv[]) {

    // Verifica se o número de argumentos está correto
    if (argc != 4) {
        fprintf(stderr, "Uso correto: %s <arquivo_matriz1> <arquivo_matriz2> <arquivo_saida>\n", argv[0]);
        exit(1);
    }

    float *matrizA, *matrizB, *matrizResultado;
    int linhasA, colunasA, linhasB, colunasB;
    double start, finish, elapsed;

    GET_TIME(start);

    // Lê as matrizes dos arquivos de entrada
    lerMatriz(argv[1], &matrizA, &linhasA, &colunasA);
    lerMatriz(argv[2], &matrizB, &linhasB, &colunasB);

    // Verifica se as dimensões das matrizes são compatíveis
    if (colunasA != linhasB) {
        fprintf(stderr, "As matrizes não podem ser multiplicadas: dimensões incompatíveis\n");
        exit(1);
    }

    // Realiza a multiplicação das matrizes
    multiplicarMatrizes(matrizA, linhasA, colunasA, matrizB, linhasB, colunasB, &matrizResultado);

    // Escreve a matriz resultado no arquivo de saída
    escreverMatriz(argv[3], matrizResultado, linhasA, colunasB);

    // Libera a memória alocada
    free(matrizA);
    free(matrizB);
    free(matrizResultado);

    GET_TIME(finish);
    elapsed = finish - start;
    printf("tempo de execucao: %lfs\n", elapsed);

    return 0;
}
