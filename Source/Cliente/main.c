
#include "main.h"


int logged = 0;
// Condição de paragem de thread
int stop = 0;

ClientStruct champ;



int ServerExecution() {
    
    if (access(ARBITRO_PIPE, F_OK) != 0)
    {
        return 0;
    }

   return 1;
}

void forced_shutdown() {
  char pipe[11];

  sprintf(pipe, "pipe-%d", getpid());
  unlink(pipe);
  exit(SHUTDOWN);
}



void shutdown() {
    char pipe[11];
    int fd;
    ClientStruct shut;

    shut.jogador.pid = getpid();
    shut.action = LOGOUT;

    sprintf(pipe, "pipe-%d", getpid());

    fd = open(ARBITRO_PIPE, O_WRONLY, 0600);
    
    write(fd, &shut, sizeof(shut));
    unlink(pipe);
    exit(SHUTDOWN);
}




void userCommands (const char* comm) {

    if (strcmp(comm, "#mygame") == 0)
    {
        // TODO:
    }
    else if (strcmp(comm, "#quit") == 0) {
        shutdown();
    }

    printf("Comando: %s \n", comm);
}


void login (int *fd_arbitro) {

    *fd_arbitro = open(ARBITRO_PIPE, O_RDWR);

    if (*fd_arbitro == -1)
    {
        //perro("Erro a abrir o Arbitro Pipe!!\n A terminar...\n");
        exit(EXIT_ERROR_PIPE);
    }

    champ.action = LOGIN;
    write(*fd_arbitro, &champ, sizeof(champ));
}

// THREAD
void *receiver(void *arg)
{
    ClientStruct receive;
    char pipe[11];
    int fd_pipe;
    int nBytes;

    sprintf(pipe, "pipe-%d", getpid());
    fd_pipe = open(pipe, O_RDWR);

    do
    {
        nBytes = read(fd_pipe, &receive, sizeof(receive));

        if (nBytes == 0)
            stop = 1;
        printf("Li %d bytes\n", nBytes);
        printf("%d", receive.action);
        switch (receive.action)
        {
            case KICK: /* kick */
                printf("O Arbitro kickou-o\nA terminar... \n");
                forced_shutdown();
                break;

        }

    } while (stop == 0);

    close(fd_pipe);
    unlink(pipe);
    pthread_exit(NULL);
}



int main (int argc, char *argv[]) {

    pthread_t thread;

    char car, comm[50];
    
    // nome do pipe
    char pipe[11];

    int fd_arbitro, fd_user;

    setbuf(stdout, NULL);
    
    if (argc != 2 ) {
        fprintf(stderr, "Missing arguments!\n");
        exit(EXIT_ERROR_ARGUMENTS);
    }

    // TODO: Verify if server is already Running
    if (!ServerExecution()){
        fprintf(stderr, "Arbitro is not in execution!\n");
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
    login (&fd_arbitro);

    fd_user = open(pipe, O_RDWR);

    if (fd_user == -1)
    {
        //perro("Erro a abrir o Cliente Pipe!!\n A terminar...\n");
        unlink(pipe);
        exit(EXIT_ERROR_PIPE);
    }

    read(fd_user, &champ, sizeof(champ));
    
    //////////// DEBUG
    printf("%d\n", champ.action);

    if (champ.action == LOGGED)
        logged = 1;
    else if (champ.action == FAIL_LOGIN)
        logged = 0;
    else if (champ.action == CHAMPIONSHIP_ALREADY_STARTED) 
    {
        fprintf(stderr, "O campeonato já começou.. Não é possivel fazer login\n");
        forced_shutdown();
    }
    
    
    // ########### END LOGIN ###############



    if (logged) {

        // Create Thread
        if (pthread_create(&thread, NULL, receiver, NULL) != 0)
        {
            printf("Erro a criar a thread!!!\nA terminar..\n");
            unlink(pipe);
            exit(EXIT_ERROR_CREATE_THREAD);
        }
        //ler comandos
        while(1) {
            printf("> ");
            scanf(" %50[^\n]s", comm);
            userCommands(comm);
        } 
    } 
    else {
        fprintf(stderr, "Já existe Jogador com esse nome!!\n");
        forced_shutdown();
    }
    

    printf("O meu nome e: %s \n", champ.jogador.username);
    // fflush(stdout);
    write(fd_user, &champ, sizeof(champ));

    // #### WAIT FINISH THREAD
    pthread_join(thread, NULL);

    // ### Close pipes #####
    close(fd_arbitro);
    unlink(pipe);
    exit(OK);
}