# Repositório Multiplicação de Matrizes

Este repositório contém os seguintes arquivos:

- GeraMatrizBinario.c: Código fornecido pela professora Silvana.
- leMatrizBinario.c: Código fornecido pela professora Silvana.
- timer.h: Código fornecido pela professora Silvana com uma pequena modificação.
- multiplica.c: Multiplica duas matrizes de maneira sequencial.
- mult_conc.c: Multiplica duas matrizes de maneira concorrente.
- comparador.c: Compara dois arquivos e diz se são idênticos.
- Relatorio.pdf: Relatório de desempenho do programa.

## Como usar

### Gerar uma matriz de valores aleatórios:

    ./GeraMatrizBinario M N arq

Onde:

- M e N representam as dimensões da matriz a ser gerada.
- arq é o nome do arquivo que será criado.



### Ler uma matriz:

    ./leMatrizBinario arq

Onde:

- arq é o nome do arquivo que será lido.



### Multiplicação sequencial:

    ./multiplica A B X

Onde:

- A é o nome do arquivo que contém a primeira matriz.
- B é o nome do arquivo que contém a segunda matriz.
- X é o nome do arquivo que será criado para armazenar o resultado da multiplicação.



### Multiplicação concorrente:

    ./mult_conc A B X N

Onde:

- A é o nome do arquivo que contém a primeira matriz.
- B é o nome do arquivo que contém a segunda matriz.
- X é o nome do arquivo que será criado para armazenar o resultado da multiplicação.
- N é a quantidade de threads criadas pelo programa.



### Comparar dois arquivos:

    ./comparador A B

Onde:

- A é o nome do primeiro arquivo.
- B é o nome do segundo arquivo.




