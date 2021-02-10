#ifndef MAIN_H
#define MAIN_H

#include "../structs.h"
#include "jogadores.h"


// ENVIRONMENT VARS
#define GAMEDIR "../GameDir/" //"../Build/"

#define MAXPLAYERS 30 // <= 30

int can_login;
int stopThread;
extern Jogador lista_jogadores[MAXPLAYERS];
extern Arbitro arbitro;
extern char lista_jogos[30][MAXCHARS];

void *thread_func(void *arg);

void arbitroCommands (const char * cmd);

void comunicacao_jogo(int comunicacao, char playerName[MAXCHARS]);

void game_interact (ClientStruct jogador);

void init_campeonato(); 

void init_game(Jogador *jogador);

void send_gameName_to_jogador(int pid, const char* game_name) ;


Jogo create_game_process (const char* game_name);

unsigned int random_num(unsigned int num);

void readGameDir() ;

void user_login(int pid, const char * username);

void kick_all_users();

int kick_user (const char *name);

void finish_campeonato();

void write_game_To_jogador(ClientStruct client);

void *game_thread(void *arg);

#endif
