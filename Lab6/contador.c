// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <math.h>        //este codigo precisa ser compilado usando -lm
#include <pthread.h>
#include <semaphore.h>
#include <string.h>


long long int *bufferCompartilhado;  
int tamanhoBuffer;                  
int numeroConsumidores;             
int totalPrimos = 0;                
int *primosPorThread;               

sem_t slotsOcupados;                
sem_t slotsVazios;                  
sem_t mutex;                        


// Função que verifica se um número é primo
int ehPrimo(long long int numero) {
    if (numero <= 1) return 0;
    if (numero == 2) return 1;
    if (numero % 2 == 0) return 0;
    for (long long int i = 3; i <= sqrt(numero); i += 2) {
        if (numero % i == 0)
            return 0;
    }
    return 1;
}


// Função executada pela thread produtora
void produtor(char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    long long int numero;
    static int posicaoInsercao = 0;  


    while (fread(&numero, sizeof(long long int), 1, arquivo) == 1) {
        sem_wait(&slotsVazios);
        sem_wait(&mutex);

        bufferCompartilhado[posicaoInsercao] = numero; 
        posicaoInsercao = (posicaoInsercao + 1) % tamanhoBuffer;  

        sem_post(&mutex);
        sem_post(&slotsOcupados);
    }

    fclose(arquivo);  


    // Insere o marcador de fim de dados no buffer para todas as threads consumidoras
    for (int i = 0; i < numeroConsumidores; i++) {
        sem_wait(&slotsVazios);
        sem_wait(&mutex);

        bufferCompartilhado[posicaoInsercao] = -1;  
        posicaoInsercao = (posicaoInsercao + 1) % tamanhoBuffer;  

        sem_post(&mutex);
        sem_post(&slotsOcupados);
    }
}


// Função executada pelas threads consumidoras
void *consumidor(void *arg) {
    long id = (long)arg;
    long long int numero;
    int primosEncontrados = 0;
    static int posicaoRemocao = 0;  

    while (1) {
        sem_wait(&slotsOcupados);
        sem_wait(&mutex);

        numero = bufferCompartilhado[posicaoRemocao]; 
        posicaoRemocao = (posicaoRemocao + 1) % tamanhoBuffer; 

        sem_post(&mutex);
        sem_post(&slotsVazios);

        if (numero == -1) {  
            primosPorThread[id] = primosEncontrados;
            pthread_exit(NULL);
        }

        if (ehPrimo(numero)) {
            primosEncontrados++;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Uso correto: %s <arquivo.bin> <tamanho do buffer> <numero de threads consumidoras>\n", argv[0]);
        return 1;
    }


    char *nomeArquivo = argv[1];
    tamanhoBuffer = atoi(argv[2]);
    numeroConsumidores = atoi(argv[3]);


    bufferCompartilhado = (long long int *)malloc(tamanhoBuffer * sizeof(long long int));
    if (bufferCompartilhado == NULL) {
        perror("Erro ao alocar memória para o buffer");
        return 1;
    }


    primosPorThread = (int *)malloc(numeroConsumidores * sizeof(int));
    if (primosPorThread == NULL) {
        perror("Erro ao alocar memória para o array de primos por thread");
        free(bufferCompartilhado);
        return 1;
    }
    memset(primosPorThread, 0, numeroConsumidores * sizeof(int));

    sem_init(&slotsOcupados, 0, 0);
    sem_init(&slotsVazios, 0, tamanhoBuffer);
    sem_init(&mutex, 0, 1);

    pthread_t threadsConsumidoras[numeroConsumidores];
    int i;

    for (i = 0; i < numeroConsumidores; i++) {
        pthread_create(&threadsConsumidoras[i], NULL, consumidor, (void *)(long)i);
    }

    produtor(nomeArquivo);
    
    for (i = 0; i < numeroConsumidores; i++) {
        pthread_join(threadsConsumidoras[i], NULL);
    }

    
    int threadVencedora = 0;

    for (i = 1; i < numeroConsumidores; i++) {
        if (primosPorThread[i] > primosPorThread[threadVencedora]) {
            threadVencedora = i;
        }
    }


    for (i = 0; i < numeroConsumidores; i++) {
        totalPrimos += primosPorThread[i];
    }


    printf("Total de primos encontrados: %d\n", totalPrimos);
    printf("Thread vencedora: %d com %d primos\n", threadVencedora, primosPorThread[threadVencedora]);

    free(bufferCompartilhado);  
    free(primosPorThread);  

    sem_destroy(&slotsOcupados);
    sem_destroy(&slotsVazios);
    sem_destroy(&mutex);

    return 0;
    
}
