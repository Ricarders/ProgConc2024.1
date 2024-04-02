# Repositório Soma Concorrente

Este repositório contém os seguintes arquivos:

- gera_vet_rand.c: Gera um vetor de números aleatórios.
- soma.c: Soma os elementos de um vetor usando threads.
- 6.txt: Arquivo de exemplo com 6 elementos.
- 100.txt: Arquivo de exemplo com 100 elementos.
- 1000.txt: Arquivo de exemplo com 1000 elementos.

## Como usar

### Gerar um vetor de números aleatórios:

    ./gera_vet_rand N Nome

Onde:

- N é o número de elementos do vetor.
- Nome é o nome do arquivo .txt que armazenará o vetor.

### Somar os elementos do vetor:

    ./soma nome T

Onde:

- Nome é o nome do arquivo .txt que contém o vetor.
- T é o número de threads a serem usadas.

## Exemplos:

Gerar um vetor de 10 elementos aleatórios e armazená-lo em 10.txt:

    ./gera_vet_rand 10 10

Somar os elementos de 10.txt usando 4 threads:

    ./soma 10 4

## Observações:

- Os arquivos de exemplo 6.txt, 100.txt e 1000.txt podem ser usados para testar o programa sem precisar gerar um novo vetor.
- O número de threads deve ser menor ou igual ao número de elementos do vetor.
