
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

void kick_user (const char *name) {
    int fd_user, pid;
    char pipe[11];
    Champ send;

    
    pid = get_pid_By_username(name);
    if(existe_jogador(name) == 1)
    {
        delete_user_by_name(name);
    
        sprintf(pipe, "pipe-%d", pid);

        fd_user = open(pipe, O_WRONLY);

        if (fd_user == -1)
        {
            perro("Erro a abrir o Arbitro Pipe!!\n A terminar...\n");
            exit(EXIT_ERROR_PIPE);
        }

        send.action = KICK;
        write(fd_user, &send, sizeof(send));
    }
    else
    {
        printf("Jogador inexistente.\n");
    }
    
}
/*char tokenK(){
    char str[]= "krui";

    char* token = strtok(str, "k");
    printf("%s\n", token); //token é o nome do jogador a ser k
    return token;

}
*/

void arbitroCommands (const char* cmd){
    char *aux;

    if (strcmp(cmd, "players") == 0){
        // TODO: listar jogadores (nome e jogo atribuido)
        listar_jogadores();
    }
    else if (strcmp(cmd, "games") == 0) {
        // TODO: listar jogos disponiveis
    }
    else if (cmd[0] == 'k'){
        aux = strchr(cmd, 'k'); //1*parametro string a separar. 2* separador. dá return da string depois do separador aparecer 
        
        kick_user(aux);
        // TODO: kick user (e.g: krui - remove jogador 'rui') 
        // dar feedBack ao user
    }
    else if (cmd[0] == 's') {
        aux = strchr(cmd, 's');
        // TODO: mensagens jogador-jogo ficam suspensas (e.g: srui ) 
    }
    else if (cmd[0] == 'r') {
        aux = strchr(cmd, 'r');
        // TODO: retomar comunicação jogador-jogo (e.g: rrui ) 
    }
    else if (strcmp(cmd, "exit") == 0) {
        exit(0);
        // TODO: Encerrar o campeonato
    }
    else if(strcmp(cmd, "end") == 0){
        //
    }

    // #DEBUG
    printf("Comando: %s \n", cmd);
}


void init_campeonato() {
    // Atribuir a cada jogador, um jogo
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

    // ######## Aguardar por pelo menos 2 jogadores ############
    while (nr_users < 2);    

    // Wait tempo Max por mais jogadores 
    sleep(arbitro.tempo_espera);

    // ################## Inicio do Campeonato #################
    init_campeonato();

    //ler comandos
    do
    {

        printf("> ");
        scanf(" %50[^\n]s", cmd);
        cmd[strlen(cmd)-1] = '\0'; 
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