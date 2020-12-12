
#include "main.h"

int nr_users;
Jogador lista_jogadores[30];
Helper helper;
Arbitro arbitro;
Jogo jogo;


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

void arbitroCommands (const char* cmd){

    if (strcmp(cmd, "players") == 0){
        // TODO: listar jogadores (nome e jogo atribuido)
        listar_jogadores();
    }
    else if (strcmp(cmd, "games") == 0) {
        // TODO: listar jogos disponiveis
    }
    else if (cmd[0] == 'k') {
        // TODO: kick user (e.g: krui - remove jogador 'rui') 
        // dar feedBack ao user
    }
    else if (cmd[0] == 's') {
        // TODO: mensagens jogador-jogo ficam suspensas (e.g: srui ) 
    }
    else if (cmd[0] == 'r') {
        // TODO: retomar comunicação jogador-jogo (e.g: rrui ) 
    }
    else if (strcmp(cmd, "end") == 0) {
        // TODO: Encerrar o campeonato
    }

    // #DEBUG
    printf("Comando: %s \n", cmd);
}


int main (int argc, char *argv[])
{   
   

    int fd[2];
    char cmd[50];

    setbuf(stdout, NULL);

    if (argc != 3) {
        perro("Missing arguments!\n");
        exit(EXIT_ERROR_ARGUMENTS);
    }

   
    
// ################# VALIDATE ARGS ####################
    if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0){
        perro("Invalid Arguments!\n");
        exit(EXIT_INVALID_ARGUMENTS);
    }

    arbitro.duracao_campeonato = atoi(argv[1]);
    arbitro.tempo_espera = atoi(argv[2]);

// ################# END VALIDATE ARGS ####################

// ################# ENVIRONMENT VARS ####################
    getEnvironmentVars(&arbitro, &helper);
// ################# END ENVIRONMENT VARS ####################
    
     // ######### Arbitro PIPE ################
    if (access(ARBITRO_PIPE, F_OK) == 0)
    {
        perro("Arbitro already in execution!\n");
        exit(EXIT_ERROR_PIPE);
    }

     // mkfifo(3)
    // S_IRWXU - read,  write,  and  execute permission
    if (mkfifo(ARBITRO_PIPE, S_IRWXU) == -1)
    {
        perro("Error: Creating Pipe! \n");
        exit(EXIT_ERROR_PIPE);
    }

    printf("Nº args : %d GameDir: %s max players %d\n"
     "Duracao campeonato: %d Tempo espera: %d\n", argc, arbitro.gamedir, arbitro.maxplayers,
     arbitro.duracao_campeonato, arbitro.tempo_espera);
    fflush(stdout);
    

    // ############################ DEBUG ######################
   
    adicionarJogador(1234, "joao");
    
    listar_jogadores();
    // ############################ DEBUG ######################

    //ler comandos
    do
    {
        printf("> ");
        scanf(" %50[^\n]s", cmd);
        arbitroCommands(cmd);

    } while (strcmp(cmd, "exit") != 0);
    
/*

    if ((jogo.pid = fork()) == -1) {
        perro("fork");
        exit(EXIT_ERROR_CREATE_PROCESS);
    }

    if (jogo.pid == 0) {    // Child 
        close(fd[0]);          // Close unused read, not need
        close(1); // stdout
        dup(fd[1]); // Duplicate. (stdout -> fd[1] - write)
        close(fd[1]);

        execl(jogo.nome, jogo.nome, NULL);

        exit(EXIT_SUCCESS);
    }

    close (fd[1]); // Close unused write, not need


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
  
*/
    

    if (unlink(ARBITRO_PIPE) == -1){
        perro("Error: Closing Pipe! \n");
        exit(EXIT_ERROR_PIPE);    
    }

    return (OK);
}