#ifndef MAIN_H
#define MAIN_H

#include "../utils.h"
#include "../structs.h"
#include "jogadores.h"


// ENVIRONMENT VARS
#define GAMEDIR "../GameDir/"

#define MAXPLAYERS 10 // < 30

extern int nr_users;
extern Jogador lista_jogadores[30];
extern Arbitro arbitro;
extern Jogo jogo;


#endif
