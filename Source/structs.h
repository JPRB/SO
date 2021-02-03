
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>


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
#define CHAMPIONSHIP_ALREADY_STARTED 104
#define KICK 105
#define GAME_NAME 406
/*
#define LOGIN
#define LOGIN
#define LOGIN
#define LOGIN
#define LOGIN
#define LOGIN
#define LOGIN
#define LOGIN
#define LOGIN
#define LOGIN
#define LOGIN
#define LOGIN
*/

// Arbitro NAMED PIPE 
#define ARBITRO_PIPE "arbitro_pipe"

#define MAXCHARS 50

typedef struct
{
    char nome[MAXCHARS];
    pid_t pid;
    int pontuacao;
    int *stdin;
    int *stdout;
} Jogo;


typedef struct
{
    int pid;
    char username[MAXCHARS];
    Jogo jogo;
} Jogador;


typedef struct
{
    char *gamedir;
    int maxplayers;
    int duracao_campeonato;
    int tempo_espera; // Tempo máximo de espera apoś 2 jogadores (segundos) 
} Arbitro;


typedef struct 
{
    int action;
    int pid;
    char cmd_1[255];
    char str[255];
    Jogador jogador;
} ClientStruct;


#endif