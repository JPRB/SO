
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

int main (int argc, char *argv[]) {

    Jogador jogador;

    setbuf(stdout, NULL);
    
    if (argc != 2 ) {
        perro("Missing arguments!\n");
        exit(EXIT_ERROR_ARGUMENTS);
    }

    strcpy(jogador.username, argv[1]);

//    sscanf(argv[1], " %50[^\n]s", jogador.username);   


    printf("O meu nome e: %s \n", jogador.username);
    // fflush(stdout);

    exit(OK);
}