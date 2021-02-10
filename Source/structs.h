
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>

// #define DEBUG

#define OK 0
#define EXIT_ERROR_ARGUMENTS 1
#define EXIT_INVALID_ARGUMENTS 2
#define EXIT_ERROR_PIPE 3
#define EXIT_ERROR_CREATE_PROCESS 4
#define EXIT_ERROR_CREATE_THREAD 5
#define EXIT_ERROR_NO_GAMES 6
#define EXIT_ERROR_EXEC_GAME 7
#define SHUTDOWN 10


// ######## LIST OF ACTIONS ########

#define LOGIN 100
#define LOGOUT 101
#define LOGGED 102
#define FAIL_LOGIN 103
#define KICK 104
#define SUS_MSG_GAME_2_PLAYER 403
#define RET_MSG_GAME_2_PLAYER 404
#define CHAMPIONSHIP_ALREADY_STARTED 405
#define GAME_NAME 406
#define GAME 410


// Arbitro NAMED PIPE 
#define ARBITRO_PIPE "arbitro_pipe"

#define MAXCHARS 50

typedef struct
{
    char nome[MAXCHARS];
    pid_t pid;
    int pontuacao;
    int stdin;
    int stdout;
    pthread_t gameThread;
} Jogo;


typedef struct
{
    int pid;
    char username[MAXCHARS];
    int sus_comunicacao;
    Jogo jogo;
    pthread_mutex_t mutex;
} Jogador;


typedef struct
{
    char gamedir[50];
    int maxplayers;
    int duracao_campeonato;
    int tempo_espera; // Tempo máximo de espera apoś 2 jogadores (segundos)
    unsigned int nr_jogos;
    unsigned int nr_users;
    int campeonato_on;
} Arbitro;


typedef struct 
{
    int action;
    int pid;
    char cmd_1[100];
    char str[100];
} ClientStruct;


#endif