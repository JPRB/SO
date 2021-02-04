#ifndef MAIN_H
#define MAIN_H

#include "../utils.h"
#include "../structs.h"
#include "jogadores.h"


// ENVIRONMENT VARS
#define GAMEDIR "../GameDir/" //"../Build/"

#define MAXPLAYERS 30 // <= 30

extern unsigned int nr_users;
extern Jogador lista_jogadores[30];
extern Arbitro arbitro;
extern Jogo jogo;


#endif
