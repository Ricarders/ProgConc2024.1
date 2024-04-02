// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 10

// Structure to store thread arguments
typedef struct {
    int inicio;
    int fim;
    float *vetor;
    float *soma_parcial;
} ArgumentosThread;

// Function executed by each thread
void *soma_bloco(void *arg) {
    ArgumentosThread *args = (ArgumentosThread *)arg;

    float soma_local = 0.0;

    // Soma os elementos do bloco
    for (int i = args->inicio; i < args->fim; i++) {
        soma_local += args->vetor[i];
    }

    // Armazena o resultado parcial na variável compartilhada
    *args->soma_parcial = soma_local;

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <nome_do_arquivo> <numero_threads>\n", argv[0]);
        return 1;
    }

    char *nome_arquivo = argv[1];

    // Leitura da quantidade de threads
    int num_threads = atoi(argv[2]);
    if (num_threads > MAX_THREADS) {
        printf("Numero de threads excede o limite maximo (%d)\n", MAX_THREADS);
        return 3;
    }

    // Leitura da quantidade de elementos, soma total e dos elementos do vetor
    FILE *arq_entrada = fopen(nome_arquivo, "r");
    if (arq_entrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada\n");
        return 2;
    }

    int n;
    fscanf(arq_entrada, "%d", &n);

    float *vetor = (float *)malloc(n * sizeof(float));
    for (int i = 0; i < n; i++) {
        fscanf(arq_entrada, "%f", &vetor[i]);
    }

    float soma_esperada;
    fscanf(arq_entrada, "%f", &soma_esperada); // Leitura da soma total

    fclose(arq_entrada);

    // Variável para armazenar a soma total
    float soma_total_concorrente = 0.0;

    // Variável para armazenar a soma parcial de cada thread
    float soma_parcial[MAX_THREADS];

    // Criação das threads
    pthread_t threads[MAX_THREADS];
    ArgumentosThread args[MAX_THREADS];

    for (int i = 0; i < num_threads; i++) {
        int inicio = i * (n / num_threads);
        int fim = (i + 1) * (n / num_threads);
        if (i == num_threads - 1) {
            fim = n;
        }

        args[i].inicio = inicio;
        args[i].fim = fim;
        args[i].vetor = vetor;
        args[i].soma_parcial = &soma_parcial[i];

        pthread_create(&threads[i], NULL, soma_bloco, &args[i]);
    }

    // Join das threads para esperar a finalização
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cálculo da soma total (somando as somas parciais)
    for (int i = 0; i < num_threads; i++) {
        soma_total_concorrente += soma_parcial[i];
    }

    // Cálculo da soma sequencial
    float soma_total_sequencial = 0.0;
    for (int i = 0; i < n; i++) {
        soma_total_sequencial += vetor[i];
    }

    // Comparação das somas
    if (soma_total_sequencial - soma_total_concorrente < 0.01) {
        printf("Soma correta: concorrente (%.2f) = sequencial (%.2f)\n", 
                soma_total_concorrente, soma_total_sequencial);
    } else {
        printf("Soma incorreta: concorrente (%.2f) != sequencial (%.2f)\n", 
                soma_total_concorrente, soma_total_sequencial);
    }
}
