// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

typedef struct {
    int id;          // Identificador da thread
    int nthreads;    // Número total de threads
    float *matrizA;  // Matriz A
    int linhasA;     // Número de linhas da matriz A
    int colunasA;    // Número de colunas da matriz A
    float *matrizB;  // Matriz B
    int linhasB;     // Número de linhas da matriz B
    int colunasB;    // Número de colunas da matriz B
    float *resultado;// Matriz resultado
} tArgs;

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

// Função executada pelas threads
void *multiplicaMatriz(void *arg) {
    tArgs *args = (tArgs *)arg;

    // Calcula a quantidade de linhas para cada thread
    int linhas_por_thread = args->linhasA / args->nthreads;
    int inicio = args->id * linhas_por_thread;
    int fim = inicio + linhas_por_thread;

    // Multiplica as matrizes
    for (int i = inicio; i < fim; i++) {
        for (int j = 0; j < args->colunasB; j++) {
            args->resultado[i * args->colunasB + j] = 0;
            for (int k = 0; k < args->colunasA; k++) {
                args->resultado[i * args->colunasB + j] +=
                    args->matrizA[i * args->colunasA + k] * args->matrizB[k * args->colunasB + j];
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso correto: %s <arquivo_matriz1> <arquivo_matriz2> <arquivo_saida> <nthreads>\n", argv[0]);
        exit(1);
    }

    float *matrizA, *matrizB, *matrizResultado;
    int linhasA, colunasA, linhasB, colunasB;
    int nthreads = atoi(argv[4]);
    pthread_t *tid;
    tArgs *args;
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

    // Aloca memória para a matriz resultado
    matrizResultado = (float *)malloc(linhasA * colunasB * sizeof(float));
    if (matrizResultado == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a matriz resultado\n");
        exit(1);
    }

    // Aloca memória para as estruturas de dados das threads
    tid = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
    if (tid == NULL) {
        fprintf(stderr, "Erro ao alocar memória para os identificadores das threads\n");
        exit(1);
    }
    args = (tArgs *)malloc(nthreads * sizeof(tArgs));
    if (args == NULL) {
        fprintf(stderr, "Erro ao alocar memória para os argumentos das threads\n");
        exit(1);
    }

    // Cria as threads
    for (int i = 0; i < nthreads; i++) {
        (args + i)->id = i;
        (args + i)->nthreads = nthreads;
        (args + i)->matrizA = matrizA;
        (args + i)->linhasA = linhasA;
        (args + i)->colunasA = colunasA;
        (args + i)->matrizB = matrizB;
        (args + i)->linhasB = linhasB;
        (args + i)->colunasB = colunasB;
        (args + i)->resultado = matrizResultado;
        if (pthread_create(tid + i, NULL, multiplicaMatriz, (void *)(args + i))) {
            fprintf(stderr, "Erro ao criar a thread %d\n", i);
            exit(1);
        }
    }

    // Aguarda o término das threads
    for (int i = 0; i < nthreads; i++) {
        pthread_join(*(tid + i), NULL);
    }

    // Escreve a matriz resultado no arquivo de saída
    escreverMatriz(argv[3], matrizResultado, linhasA, colunasB);

    // Libera a memória alocada
    free(matrizA);
    free(matrizB);
    free(matrizResultado);
    free(tid);
    free(args);

    GET_TIME(finish);
    elapsed = finish - start;
    printf("Tempo de execução: %lfs\n", elapsed);

    return 0;
}
