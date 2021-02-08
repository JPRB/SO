
#include "main.h"
#include <dirent.h>


//unsigned int nr_users;
unsigned int nr_jogos=0;
int can_login = 1;
int stopThread = 0, stop_ThreadGame = 0;
Jogador lista_jogadores[MAXPLAYERS];
Arbitro arbitro;
char lista_jogos[30][MAXCHARS];


void write_Tojogador(ClientStruct client){
    char pipe[15];

    sprintf(pipe, "pipe-%d", client.pid);
    int fd_player = open(pipe, O_WRONLY);

    if (fd_player == -1)
    {
        perror("Erro a abrir o Cliente Pipe!!\n");
        unlink(ARBITRO_PIPE);
        exit(EXIT_ERROR_PIPE);
    }
    // dar feedBack ao user
    write(fd_player, &client, sizeof(client));
}

void *game_thread(void *arg) {

    Jogador player = *((Jogador *)(arg));
    ClientStruct send;

    #ifdef DEBUG
        printf("[JOGADOR] nome: %s Pid %d\n[JOGO] nome: %s PID: %d \n", player.username, player.pid, player.jogo.nome, player.jogo.pid);
        printf("Stdin: %d -- stdout %d\n", player.jogo.stdin, player.jogo.stdout);
    #endif

    send.pid = player.pid;
   
    do {
        if (read(player.jogo.stdout, send.str, sizeof(send.str)) > 0) {

            #ifdef DEBUG
                printf("Eu li do pipe do Jogo: \n%s\n--- Tamanho: [%ld] bytes\n",  send.str, strlen(send.str));
            #endif

            write_Tojogador(send);
        }
        
    } while (!stop_ThreadGame);

    
}


void finish_champion()
{
    ClientStruct send;
    // Campeonato END 
    // send.action = ;
    for (int i=0; i < arbitro.nr_users; i++) {
        Jogador player;
        strcpy(player.username ,lista_jogadores[i].username);
        player.jogo = lista_jogadores[i].jogo;

        // Enviar signal SIGUSR1 to Jogador a dizer que campeonato terminou

        // Enviar Singla SIGUSR1 to kill game
        //union sigval sigval1;
        if(sigqueue(player.jogo.pid, SIGUSR1, NULL) != -1)
        {
        
        int game_PID = wait(&player.jogo.pontuacao);

        // #DEBUG
        #ifdef DEBUG
            fprintf(stdout, "\n PID JOGO %d EQUALS PID GAME WAIT? %d\n", player.jogo.pid, game_PID);
        #endif

        if (WIFEXITED(player.jogo.pontuacao)) {
            printf("Jogador %s --> Jogo Pontuacao: %d\n", player.username, WEXITSTATUS(player.jogo.pontuacao));
        }
        }
        // Enviar a pontuação para o Jogador pelo pipe
        
    }
    // Campeonato Over!!!
    printf("------ Acabou Campeonato!! ---------\n");
}


int getEnvironmentVars () {

    char *smaxplayers;
    if (getenv("GAMEDIR") == NULL) {
        strcpy(arbitro.gamedir, GAMEDIR);
    }
    else
        strcpy(arbitro.gamedir, getenv("GAMEDIR"));


    smaxplayers = getenv("MAXPLAYERS");
    
    if (smaxplayers != NULL)  {
        arbitro.maxplayers = atoi(smaxplayers);
    }
    else
    {
        arbitro.maxplayers = MAXPLAYERS;
    }
}

int kick_user (const char *name) {
    int fd_user, pid, val=1;
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
        val=0;
    }
    return val;
}

void kick_all_users() {

    for (int i=arbitro.nr_users; arbitro.nr_users > 0; i--) {
        char nome[MAXCHARS] = "\0";
        strcpy(nome, lista_jogadores[i].username);
        int game_pid = lista_jogadores[i].jogo.pid;
     
        if (kick_user(nome) == 1){
            // Enviar SIGUSR1 to kill game if exist
            // kill(game_pid, SIGUSR1);
        }
    }

    // shutdown();
}


void user_login(int pid, const char * username) {
    ClientStruct send;
    send.pid = pid;

    if (can_login && (arbitro.nr_users < arbitro.maxplayers))
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

    write_Tojogador(send);
}


void readGameDir(){
    char * dirname;
    DIR *dir;

    struct dirent * entrada;
    dirname = arbitro.gamedir;

    if ((dir = opendir(dirname)) == NULL) {
        perror("\nErro em opendir");
        exit(EXIT_ERROR_ARGUMENTS);
    }
    else {
        while ((entrada = readdir(dir)) != NULL)
        {
            if ((entrada->d_name[0] == 'G' || entrada->d_name[0] == 'g') && entrada->d_name[1] == '_')
            {
                
                strcpy(lista_jogos[arbitro.nr_jogos++], entrada->d_name);
                // # DEBUG
                #ifdef DEBUG
                    printf("Total: %d -- Nome: %s -- Nome saved : %s\n", arbitro.nr_jogos, entrada->d_name, lista_jogos[arbitro.nr_jogos-1]);
                #endif
            }
        }
        closedir(dir);
    }

    // # DEBUG
    #ifdef DEBUG
        printf("Nr. Jogos: %d\n", arbitro.nr_jogos);
        for (int i=0; i < arbitro.nr_jogos; i++) { 
            printf("[Nome do Jogo %d]: %s\n", i+1, lista_jogos[i]);
        }
    #endif
}


void listar_jogos() {
    for (unsigned int i = 0; i < arbitro.nr_jogos; i++) {
		printf("\n Jogo [%d] : %s\n", i+1, lista_jogos[i]);
	}
}

// Return : num random from 0 to num
unsigned int random_num(unsigned int num) {
    srand(time(NULL));
    return rand() % num;
}

Jogo create_game_process (const char* game_name) {
    Jogo jogo;

    // pipe father to child (Arbitro -> Game)
    int pipeF[2];
    // pipe child to father (Game -> Arbitro)
    int pipeC[2];
    char path[50] = "\0";


    strcpy(jogo.nome, game_name);
    strcpy(path, arbitro.gamedir);

     // Unnamed pipe father to child || Unnamed pipe child to Father
    if (pipe(pipeF) == -1 || pipe(pipeC) == -1)
    {
        fprintf(stderr, "Cannot execute Game! [1]\n");
        exit(EXIT_ERROR_PIPE);
    }

    jogo.pid = fork();
    #ifdef DEBUG
        fprintf(stdout, "my pid is %d\n",jogo.pid);
    #endif

    switch (jogo.pid)
    {
        // Erro
        case -1:
            perro("Error on fork");
            exit(EXIT_ERROR_CREATE_PROCESS);
            break;

        // CHILD
        case 0:

            // Close unused write on pipe Father, not need
            close(pipeF[1]);
            // Close unused read on pipe Child, not need
            close(pipeC[0]);

            close(0); // stdin
            // Duplicate. (stdin (0) -> pipeF[0] - read)
            dup(pipeF[0]);


            close(1); // stdout
            // Duplicate. (stdout (1) -> pipeC[1] - write)
            dup(pipeC[1]);

           

            close(pipeC[1]);
            close(pipeF[0]);

            #ifdef DEBUG
                fprintf(stderr, "GAME DIR: %s\n", arbitro.gamedir);
                fprintf(stderr, "PATH: %s\n", path);
            #endif

            strcat(path, game_name);
            
            #ifdef DEBUG
                fprintf(stderr, "PATH After Cat: %s\n", path);
            #endif

            if (execl(path, jogo.nome, NULL) == -1)
            {
                fprintf(stderr, "Error execution game");
                exit(EXIT_ERROR_EXEC_GAME);
            }

            fprintf(stderr, "Não devia chegar aqui");

            exit(EXIT_SUCCESS);
            break;

        default:
            close (pipeF[0]); // Close unused father read, not need
            close (pipeC[1]); // Close unused child write, not need

            jogo.stdin = pipeF[1];
            jogo.stdout = pipeC[0];

            break;
    }

    


/*
    printf("Introduza o PID do processo a eliminar: ");
    scanf("%d", &jogo.pid);

    if (kill(jogo.pid, SIGUSR1) == -1) {
        perro("Erro a enviar signal\n");
    }

    waitpid(jogo.pid, &jogo.pontuacao, 0);

    // wait(&jogo.pontuacao);
    if (WEXITSTATUS(jogo.pontuacao)) {
        printf("Jogo Pontuação: %d", WEXITSTATUS(jogo.pontuacao));
    }*/

    return jogo;
}

void send_gameName_to_jogador(int pid, const char* game_name) {
    ClientStruct jogador;

    jogador.pid = pid;
    jogador.action = GAME_NAME;
    strcpy(jogador.str, game_name);

    write_Tojogador(jogador);
}

void init_game(Jogador *jogador){

    // Atribuir a cada jogador, um jogo
        // 1. numero aleatorio até nr_jogos
    unsigned int nr_random = random_num(arbitro.nr_jogos);
    #ifdef DEBUG
        printf("Nr random -- calhou: %d\n\n", nr_random);
    #endif
        // 2. Executar o processo do jogo do nr aletorio
    strcpy(jogador->jogo.nome, lista_jogos[nr_random]);

       

        // 3. Executar o processo do jogo
            // 3.1 Fork, fechar e duplicar descritores, guardar stdin e stdout.
        jogador->jogo = create_game_process(jogador->jogo.nome);
        #ifdef DEBUG
            printf("[JOGO] nome: %s PID: %d pontuacao: %d\n", jogador->jogo.nome, jogador->jogo.pid, jogador->jogo.pontuacao);
            printf("stdout -> %d -- stdin -> %d\n", jogador->jogo.stdout, jogador->jogo.stdin);
        #endif       
       
    // 4. Indicar o jogo atribuido ao Jogador
    send_gameName_to_jogador(jogador->pid, jogador->jogo.nome);


    // Create Thread Game
    if (pthread_create(&jogador->jogo.gameThread, NULL, game_thread, (void *) jogador) != 0)
    {   
        printf("Erro a criar a thread!!!\nA terminar..\n");
        unlink(ARBITRO_PIPE);
        exit(EXIT_ERROR_CREATE_THREAD);
    }
}


void init_campeonato() {

    if (arbitro.nr_users > 0) {

        for(unsigned int i=0; i < arbitro.nr_users; i++){
            init_game(&lista_jogadores[i]);
            #ifdef DEBUG
                printf("Game name: %s\n\n", lista_jogadores[i].jogo.nome);
            #endif
        }
    }
    else
    {
        printf("Não é possivel inical Campeonato Sem Jogadores\n");
    }
    
}

void game_interact (ClientStruct jogador) {
    Jogador *j = get_jogador_by_pid(jogador.pid);

    strcat(jogador.str, "\n");
    write(j->jogo.stdin, jogador.str, strlen(jogador.str));
    #ifdef DEBUG
        printf("\nEu escrevi para o pipe do jogo: %s\n", jogador.str);
    #endif
}

void arbitroCommands (const char * cmd) {
    char sub_cmd[MAXCHARS] = "";
    char *p = (char *)  cmd + 1;

	strcpy(sub_cmd, p);
    //printf("-- sub comand -- %s \n", sub_cmd);    

    if (strcmp(cmd, "players") == 0){
        // listar jogadores (nome e jogo atribuido)
        listar_jogadores();
    }
    else if (strcmp(cmd, "games") == 0) {
        // listar jogos disponiveis
        listar_jogos();
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
        // Encerrar o campeonato
        finish_champion();
    }
    else if(strcmp(cmd, "exit") == 0) {
        kick_all_users();
    }
    else if(strcmp(cmd, "init") == 0) {
        init_campeonato();
    } else {
        printf("\n-- Comando nao reconhecido --\n");    
    }
   
    
    // #DEBUG
    #ifdef DEBUG
        printf("- users nr. %d - \n", arbitro.nr_users);
        printf("Comando: %s \n", cmd);
    #endif
}


void *thread_func(void *arg) {
    ClientStruct receive;

    int fd, fd_send;
    //char pipe[15]11];

    //mkfifo(ARBITRO_PIPE, 0600);
    // OPEN ARBITRO PIPE
    fd = open(ARBITRO_PIPE, O_RDONLY);

    if (fd == -1)
    {
        perro("Erro a abrir o Arbitro Pipe!!\n A terminar...\n");
        exit(EXIT_ERROR_PIPE);
    }

     do {
        int n = read(fd, &receive, sizeof(receive));
            
        //sprintf(pipe, "pipe-%d", receive.pid);
        //fd_send = open(pipe, O_WRONLY, 0600);
        #ifdef DEBUG
            printf("received %d -- [%d]\n", receive.action, n);
        #endif
        switch (receive.action) {
            case LOGIN:
                #ifdef DEBUG
                    printf("Login\n");
                #endif     
                user_login(receive.pid, receive.str);
                break;
            case LOGOUT:
                delete_user_by_PID(receive.pid);
                break;
            default:
                game_interact(receive);
                break;
        }
    } while (!stopThread);
    
    close(fd);
}


void *campeonato(void *arg) {
    Arbitro arbitro = *((Arbitro *) arg);

    // ######## Aguardar por pelo menos 2 jogadores ############
    while (arbitro.nr_users < 2);

    sleep(arbitro.tempo_espera);

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

    arbitro.nr_users = 0;
    arbitro.nr_jogos = 0;
    arbitro.duracao_campeonato = atoi(argv[1]);
    arbitro.tempo_espera = atoi(argv[2]);

    // ################# END VALIDATE ARGS ####################

    // ################# ENVIRONMENT VARS ####################
    getEnvironmentVars();
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


    // Ler o diretorio de jogos, criar array estático de jogos [nome]
    readGameDir();
    
    #ifdef DEBUG
        printf("Nr Jogos: %d\n", arbitro.nr_jogos);
    #endif
    
    // Verificar nr jogos > 0 in GAMEDIR
    if (arbitro.nr_jogos < 1)
    {
        perro("Não existem jogos na pasta!\n A terminar..");
        exit(EXIT_ERROR_NO_GAMES);
    }

    #ifdef DEBUG
        printf("Nº args : %d | GameDir: %s | Max players %d\n"
        "Duracao total campeonato: %d | Tempo espera por 2+ jogadores: %d\n", argc, arbitro.gamedir, arbitro.maxplayers,
        arbitro.duracao_campeonato, arbitro.tempo_espera);
        fflush(stdout);
    #endif
    
   
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