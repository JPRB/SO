
#include "main.h"


int logged = 0;


void userCommands (const char* comm) {

    switch (*comm)
    {
        case '#mygame':
        
            break;

        default:
            break;
    }

    printf("Comando: %s \n", comm);
}

int ServerExecution() {
    
    if (access(ARBITRO_PIPE, F_OK) != 0)
    {
        return 0;
    }

   return 1;

/*
     // mkfifo(3)
    // S_IRWXU - read,  write,  and  execute permission
    if (mkfifo(ARBITRO_PIPE, S_IRWXU) == -1)
    {
        //perro("Error: Creating Pipe! \n");
        exit(EXIT_ERROR_PIPE);
    }
*/

}


void login (int *fd_arbitro, Champ *send) {

    *fd_arbitro = open(ARBITRO_PIPE, O_RDWR);

    if (*fd_arbitro == -1)
    {
        //perro("Erro a abrir o Arbitro Pipe!!\n A terminar...\n");
        exit(EXIT_ERROR_PIPE);
    }

    send->action = LOGIN;
    write(*fd_arbitro, send, sizeof(*send));
}


int main (int argc, char *argv[]) {

    Champ champ;
    char car, comm[50];

    // nome do pipe
    char pipe[11];

    int fd_arbitro, fd_user;

    setbuf(stdout, NULL);
    
    if (argc != 2 ) {
        //perro("Missing arguments!\n");
        exit(EXIT_ERROR_ARGUMENTS);
    }

    // TODO: Verify if server is already Running
    if (!ServerExecution()){
        //perro("Arbitro is not in execution!\n");
        exit(EXIT_ERROR_PIPE);
    }

    //Create client pipe 
    champ.jogador.pid = getpid();
    sprintf(pipe, "pipe-%d", champ.jogador.pid);

    if (mkfifo(pipe, S_IRWXU) < 0)
    {
        //perro("Erro ao criar pipe. A sair...\n");
        exit(EXIT_ERROR_PIPE);
    }

   
    // ########### LOGIN ###############
    strcpy(champ.jogador.username, argv[1]);
    login (&fd_arbitro, &champ);

    fd_user = open(pipe, O_RDWR);

    if (fd_user == -1)
    {
        //perro("Erro a abrir o Cliente Pipe!!\n A terminar...\n");
        exit(EXIT_ERROR_PIPE);
    }

    read(fd_user, &champ, sizeof(champ));
    
    if (champ.action == LOGGED)
        logged = 1;


    if (logged) {
        //ler comandos
        do
        {
            printf("> ");
            scanf(" %50[^\n]s", comm);
            userCommands(comm);

        } while (strcmp(comm, "#quit") != 0);
    }

    printf("O meu nome e: %s \n", champ.jogador.username);
    // fflush(stdout);

    // ### Close pipes #####
    close(fd_arbitro);
    unlink(pipe);


    exit(OK);
}