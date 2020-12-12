
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"


int logIn = 0;


void userCommands (const char* comm){

    switch (*comm)
    {
        case '#mygame':
        
            break;

        default:
            break;
    }

    printf("Comando: %s \n", comm);
}


void login (int *fd_arbitro, Champ *send) {

    fd_arbitro = open(ARBITRO_PIPE, O_RDWR);

    if (fd_arbitro == -1)
    {
        perro("Erro a abrir o Arbitro Pipe!!\n A terminar...\n");
        exit(EXIT_ERROR_PIPE);
    }

    send->action = LOGIN;
    write(fd_arbitro, &send, sizeof(send));
}


int main (int argc, char *argv[]) {

    Champ champ;
    char car, comm[50];

    // nome do pipe
    char pipe[11];

    int fd_arbitro;

    setbuf(stdout, NULL);
    
    if (argc != 2 ) {
        perro("Missing arguments!\n");
        exit(EXIT_ERROR_ARGUMENTS);
    }

    // TODO: Verify if server is already Running


    //Create client pipe 
    champ.jogador.pid = getpid();
    sprintf(pipe, "pipe-%d", champ.jogador.pid);

    if (mkfifo(pipe, S_IRWXU) < 0)
    {
        perro("Erro ao criar pipe. A sair...\n");
        exit(EXIT_ERROR_PIPE);
    }

    // ########### LOGIN ###############
    strcpy(champ.jogador.username, argv[1]);
    //login (&fd_arbitro, &champ);
    

  

    //ler comandos
    do
    {
        printf("> ");
        scanf(" %50[^\n]s", comm);
        userCommands(comm);

    } while (strcmp(comm, "#quit") != 0);
    

    printf("O meu nome e: %s \n", champ.jogador.username);
    // fflush(stdout);

    // ### Close pipes #####
    close(fd_arbitro);
    unlink(pipe);


    exit(OK);
}