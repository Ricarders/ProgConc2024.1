# Repositório Produtor Consumidor

Este repositório contém os seguintes arquivos:

- 1mil.bin: arquivo binário com 1000 valores aleatórios
- 10mil.bin: arquivo binário com 10000 valores aleatórios
- 100mil.bin: arquivo binário com 100000 valores aleatórios
- gerador.c: programa que gera um arquivo binário com N valores aleatórios e a quantidade de valores primos gerados
- contador.c: programa implementando o padrão produtor consumidor para contar a quantidade de números primos no arquivo gerado

## Como usar

### Gerar um arquivo com N valores aleatórios:

    ./gerador N arq

Onde:

- N representa quantidade de valores a serem gerados.
- arq é o nome do arquivo que será criado.



### Contar a quantidade de numeros primos em um arquivo binário:

    ./contador arq M T

Onde:

- arq é o nome do arquivo que será lido.
- M é o tamanho do buffer compartilhado.
- T é o numero de threads consumidoras que serão criadas.
