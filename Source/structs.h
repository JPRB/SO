
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>


#define OK 0
#define EXIT_ERROR_ARGUMENTS 1
#define EXIT_INVALID_ARGUMENTS 2
#define EXIT_ERROR_PIPE 3
#define EXIT_ERROR_CREATE_PROCESS 4

// Arbitro NAMED PIPE 
#define ARBITRO_PIPE "arbitro_pipe"



#define MAXCHARS 50

typedef struct
{
    char username[MAXCHARS];
    int pid;
    int pontuacao;
    int *stdin;
    int *stdout;
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
    char nome[MAXCHARS];
    pid_t pid;
    int pontuacao;
    int *stdin;
    int *stdout;
} Jogo;



typedef struct
{
    char *smaxplayers;
} Helper;


void perro(const char* str_error){
    fprintf(stderr, "%s", str_error);
}

#endif