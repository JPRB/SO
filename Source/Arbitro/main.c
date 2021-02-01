
#include "main.h"
#include <dirent.h>

int nr_users;
int nr_jogos=0;
int can_login = 1;
int stopThread = 0;
Jogador lista_jogadores[30];
Arbitro arbitro;
char lista_jogos[30][MAXCHARS];

int getEnvironmentVars () {

    char *smaxplayers;
    if ((arbitro.gamedir = getenv("GAMEDIR")) == NULL) {
        arbitro.gamedir = GAMEDIR;
    }


    smaxplayers = getenv("MAXPLAYERS");
    
    if (smaxplayers != NULL)  {
        arbitro.maxplayers = atoi(smaxplayers);
    }
    else
    {
        arbitro.maxplayers = MAXPLAYERS;
    }
}

void kick_user (const char *name) {
    int fd_user, pid;
    char pipe[11];
    ClientStruct send;

    if((pid = get_pid_By_username(name)) != -1)
    {
        delete_user_by_name(name);
    
        sprintf(pipe, "pipe-%d", pid);
        fd_user = open(pipe, O_WRONLY);

        if (fd_user == -1)
        {
            perro("Erro a abrir o Cliente Pipe!!\n");
            exit(EXIT_ERROR_PIPE);
        }

        // dar feedBack ao user
        send.action = KICK;
        write(fd_user, &send, sizeof(send));

        close(fd_user);
    }
    else
    {
        printf("Jogador inexistente.\n");
    }
    
}

void kick_all_users() {

    for (int i=nr_users; nr_users != 0; i--) {
        char *nome = lista_jogadores[i].username;
        kick_user(nome);
    }
}


void user_login(int pid, const char * username) {
    int fd_user;
    char pipe[11];
    ClientStruct send;
    
    sprintf(pipe, "pipe-%d", pid);

    fd_user = open(pipe, O_RDWR);
    if (fd_user == -1)
    {
        perro("Erro a abrir o Cliente Pipe!!\n");
        //exit(EXIT_ERROR_PIPE);
    }

    if (can_login)
    {
        if (!existe_jogador(username))
        {
            adicionarJogador(pid, username);
        
            send.action = LOGGED;
            strcpy(send.str, username);
        }
        else 
        {   
            send.action = FAIL_LOGIN;
        } 
    }
    else {
        send.action = CHAMPIONSHIP_ALREADY_STARTED;
    }

    write(fd_user, &send, sizeof(send));
        
    close(fd_user);
}


void readGameDir(){
    char * dirname;
    DIR *dir;

    struct dirent * entrada;
    dirname = arbitro.gamedir;

    if ((dir = opendir(dirname)) == NULL)
        perror("\nErro em opendir");
    else {
        while ((entrada = readdir(dir)) != NULL)
        {
            if ((entrada->d_name[0] == 'G' || entrada->d_name[0] == 'g') && entrada->d_name[1] == '_')
            {
                
                strcpy(lista_jogos[nr_jogos++], entrada->d_name);
                // # DEBUG
                printf("Total: %d -- Nome: %s -- Nome saved : %s\n", 
                nr_jogos, entrada->d_name, lista_jogos[nr_jogos-1]);
            }   
        }
        closedir(dir);
    }

    // # DEBUG
    printf("%d\n\n", nr_jogos);
    for (int i=0; i < nr_jogos; i++) { 
        printf("%s\n", lista_jogos[i]);
    }

}


void init_campeonato() {
    // Atribuir a cada jogador, um jogo
        // 1. numero aleatorio até nr_jogos

        // 2. Executar o processo do jogo do nr aletorio
            
            // 2.1 Fork, fechar e duplicar descritores stdin e stdout.
            
        // 3.
    
}

void arbitroCommands (const char * cmd) {
    char sub_cmd[MAXCHARS] = "";
    char *p = (char *)  cmd + 1;

	strcpy(sub_cmd, p);
    //printf("-- sub comand -- %s \n", sub_cmd);

    if (strcmp(cmd, "players") == 0){
        // TODO: listar jogadores (nome e jogo atribuido)
        listar_jogadores();
    }
    else if (strcmp(cmd, "games") == 0) {
        // TODO: listar jogos disponiveis
    }
    else if (cmd[0] == 'k') {
        // kick user (e.g: krui - remove jogador 'rui') 
        kick_user(sub_cmd);
        
    }
    else if (cmd[0] == 's') {
        // TODO: mensagens jogador-jogo ficam suspensas (e.g: srui ) 
    }
    else if (cmd[0] == 'r') {
        // TODO: retomar comunicação jogador-jogo (e.g: rrui ) 
    }
    else if(strcmp(cmd, "end") == 0) {
        // TODO: Encerrar o campeonato
    }
    else if(strcmp(cmd, "exit") == 0) {
        kick_all_users();
    }
    else if(strcmp(cmd, "init") == 0) {
        init_campeonato();
    }
    
    
    printf("- users nr. %d - \n", nr_users);

    // #DEBUG
    printf("Comando: %s \n", cmd);
}



void *thread_func(void *arg) {
    ClientStruct receive;

    int fd, fd_send;
    char pipe[11];

    mkfifo(ARBITRO_PIPE, 0600);
    // OPEN ARBITRO PIPE
    fd = open(ARBITRO_PIPE, O_RDWR);

    if (fd == -1)
    {
        perro("Erro a abrir o Arbitro Pipe!!\n A terminar...\n");
        exit(EXIT_ERROR_PIPE);
    }

     do {
        read(fd, &receive, sizeof(receive));

        sprintf(pipe, "pipe-%d", receive.pid);
        fd_send = open(pipe, O_WRONLY, 0600);

        switch (receive.action) {
            case LOGIN: 
                user_login(receive.pid, receive.str);
                break;
            case LOGOUT:
                delete_user_by_PID(receive.pid);
                break;
        }
    } while (!stopThread);
  
}


void *campeonato(void *arg) {
    Arbitro *arbitro = (Arbitro *) arg;

    // ######## Aguardar por pelo menos 2 jogadores ############
    while (nr_users < 2);

    sleep(arbitro->tempo_espera);

    // Var de controlo para não poderem logar...
    can_login = 0;

    // ################## Inicio do Campeonato #################
    init_campeonato();
}


int main (int argc, char *argv[])
{   
    Jogo jogo;
    ClientStruct champ;

    pthread_t thread, threadCampeonato;

    int fd_game[2], fd;
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
    getEnvironmentVars();
    // ################# END ENVIRONMENT VARS ####################
    

    // TODO : Ler o diretorio de jogos, criar array estático de jogos [nome]
    readGameDir();
    getchar();
getchar();
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
    
   
    // CREATE THREAD To RECEIVE FROM USERS
    if (pthread_create(&thread, NULL, thread_func, NULL) != 0)
    {
        printf("Erro a criar a thread!!!\nA terminar..\n");
        unlink(ARBITRO_PIPE);
        exit(EXIT_ERROR_CREATE_THREAD);
    }

    // CREATE THREAD To Init CHAMPIONSHIP
    /*if (pthread_create(&threadCampeonato, NULL, campeonato, (void *) &arbitro) != 0)
    {
        printf("Erro a criar a thread!!!\nA terminar..\n");
        unlink(ARBITRO_PIPE);
        exit(EXIT_ERROR_CREATE_THREAD);
    }*/

    //READ commands
    do
    {
        printf("> ");
        scanf(" %50[^\n]s", cmd);
        arbitroCommands(cmd);

    } while (strcmp(cmd, "exit") != 0);
    

    /*do {
        
        read(fd, &champ, sizeof(champ));
        if (champ.action == LOGIN) {
            user_login(champ.jogador.pid, champ.jogador.username);
        }
        // DEBUG
        //printf("username %s pid : %d\n Action: %d\n\n", champ.jogador.username, champ.jogador.pid, champ.action);
        printf("nr users : %d\n", nr_users);
    } while (nr_users < 2);

    // Wait tempo Max por mais jogadores 
    while (sleep(arbitro.tempo_espera) > 0) {
        read(fd, &champ, sizeof(champ));
        if (champ.action == LOGIN) {
            user_login(champ.jogador.pid, champ.jogador.username);
        }
    }*/

    // ################## Inicio do Campeonato #################
    //init_campeonato();


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