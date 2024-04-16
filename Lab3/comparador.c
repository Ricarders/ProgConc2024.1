// Nome - Ricardo P. Silva Filho
// DRE - 118068055

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso correto: %s <arquivo1> <arquivo2>\n", argv[0]);
        exit(1);
    }

    char comando[100];
    sprintf(comando, "diff %s %s", argv[1], argv[2]);

    int status = system(comando);

    if (status == 0) {
        printf("Os arquivos %s e %s são iguais.\n", argv[1], argv[2]);
    } else {
        printf("Os arquivos %s e %s são diferentes.\n", argv[1], argv[2]);
    }

    return 0;
}
