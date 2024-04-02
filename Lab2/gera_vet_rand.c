#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX 1000  // valor maximo dos elementos do vetor

int main(int argc, char *argv[]) {
    long int n;      // qtde de elementos do vetor que serao gerados
    float elem;     // valor gerado para incluir no vetor
    int fator = 1;  // fator multiplicador para gerar números negativos
    char nome_arquivo[100]; // nome do arquivo a ser gerado

    // valida e recebe os valores de entrada
    if (argc < 3) {
        printf("Use: %s <numero de elementos (maior que 0)> <nome do arquivo>\n", argv[0]);
        return 1;
    }
    n = atol(argv[1]);
    if (n < 1) {
        printf("ERRO: o numero de elementos deve ser maior que 0 \n");
        return 2;
    }
    strcpy(nome_arquivo, argv[2]);

    // semente para gerar numeros randomicos
    srand((unsigned)time(NULL));

    // abre o arquivo para escrita
    FILE *arq_saida = fopen(nome_arquivo, "w");
    if (arq_saida == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo %s para escrita\n", nome_arquivo);
        return 3;
    }


    // gera e imprime os elementos separado por espaco em branco
    for (int i = 0; i < n; i++) {
        elem = (rand() % MAX) / 3.0 * fator;
        fprintf(arq_saida, "%f ", elem);
        fator *= -1;
    }

    fclose(arq_saida); // fecha o arquivo

    return 0;
}
