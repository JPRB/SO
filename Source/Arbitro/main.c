
#include "main.h"


int getEnvironmentVars (Arbitro *arbitro, Helper *helper) {
    if ((arbitro->gamedir = getenv("GAMEDIR")) == NULL) {
        arbitro->gamedir = GAMEDIR;
    }


    helper->smaxplayers = getenv("MAXPLAYERS");
    
    if (helper->smaxplayers != NULL)  {
        arbitro->maxplayers = atoi(helper->smaxplayers);
    }
    else
    {
        arbitro->maxplayers = MAXPLAYERS;
    }
}



int main (int argc, char *argv[])
{   
    Helper helper;
    Arbitro arbitro;
    Jogo jogo;

    int fd[2];


    setbuf(stdout, NULL);

    if (argc != 3) {
        perro("Missing arguments!\n");
        exit(EXIT_ERROR_ARGUMENTS);
    }
    
// ################# CMD LINE ARGS ####################
    if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0){
        perro("Invalid Arguments!\n");
        exit(EXIT_INVALID_ARGUMENTS);
    }

    arbitro.duracao_campeonato = atoi(argv[1]);
    arbitro.tempo_espera = atoi(argv[2]);

// ################# END CMD LINE ARGS ####################

// ################# ENVIRONMENT VARS ####################
    getEnvironmentVars(&arbitro, &helper);
// ################# END ENVIRONMENT VARS ####################
    
    

    printf("Nº args : %d GameDir: %s max players: %d\n"
    "Duracao campeonato: %d Tempo espera: %d\n", argc, arbitro.gamedir, arbitro.maxplayers,
     arbitro.duracao_campeonato, arbitro.tempo_espera);
    fflush(stdout);
    
    // 

    if ((jogo.pid = fork()) == -1) {
        perro("fork");
        exit(EXIT_ERROR_CREATE_PROCESS);
    }

    if (jogo.pid == 0) {    /* Child */
        close(fd[0]);          /* Close unused read, not need*/
        close(1); // stdout
        dup(fd[1]); // Duplicate. (stdout -> fd[1] - write)
        close(fd[1]);

        execl(jogo.nome, jogo.nome, NULL);

        exit(EXIT_SUCCESS);
    }

    close (fd[1]); /* Close unused write, not need*/


    printf("Introduza o PID do processo a eliminar: ");
    scanf("%d", &jogo.pid);

    if (kill(jogo.pid, SIGUSR1) == -1) {
        perro("Erro a enviar signal\n");
    }

    waitpid(jogo.pid, &jogo.pontuacao, 0);

    // wait(&jogo.pontuacao);
    if (WEXITSTATUS(jogo.pontuacao)) {
        printf("Jogo Pontuação: %d", WEXITSTATUS(jogo.pontuacao));
    }

    

    

    /*
    int c;
  while ((c = getopt (argc, argv, "")) != -1) {
    printf("Argc : %d ARGV:", argc);
    fflush(stdout);

    for (int i=0; i < argc; i++)
        printf("%s", argv[i]);
        fflush(stdout);
  }
*/


    if (unlink(ARBITRO_PIPE) == -1){
        perro("Error: Closing Pipe! \n");
        exit(EXIT_ERROR_PIPE);    
    }

    return (OK);
}