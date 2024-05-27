// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Função que verifica se um número é primo
int verificarPrimo(long long int numero) {
    if (numero <= 1) return 0;
    if (numero == 2) return 1;
    if (numero % 2 == 0) return 0;
    for (long long int i = 3; i <= sqrt(numero); i += 2) {
        if (numero % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    FILE *arquivo;
    long long int quantidade;
    long long int *valores;
    int contagemPrimos = 0;

    if (argc != 3) {
        printf("Uso correto: %s <quantidade de números> <nome do arquivo>\n", argv[0]);
        return 1;
    }

    quantidade = atoll(argv[1]); // Convertendo a quantidade para long long int

    valores = (long long int *)malloc(quantidade * sizeof(long long int));
    if (valores == NULL) {
        perror("Erro ao alocar memória");
        return 1;
    }

    srand(time(NULL));

    for (long long int i = 0; i < quantidade; i++) {
        valores[i] = rand() % 100000; 
        if (verificarPrimo(valores[i])) {
            contagemPrimos++;
        }
    }

    arquivo = fopen(argv[2], "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        free(valores);
        return 1;
    }

    // Escrevendo os números gerados no arquivo
    if (fwrite(valores, sizeof(long long int), quantidade, arquivo) != quantidade) {
        perror("Erro ao escrever no arquivo");
        fclose(arquivo);
        free(valores);
        return 1;
    }

    // Escrevendo a contagem de números primos no final do arquivo
    if (fwrite(&contagemPrimos, sizeof(int), 1, arquivo) != 1) {
        perror("Erro ao escrever no arquivo");
        fclose(arquivo);
        free(valores);
        return 1;
    }

    fclose(arquivo);
    free(valores);

    printf("Total de números primos gerados no arquivo %s: %d\n", argv[2], contagemPrimos);
    return 0;
}
