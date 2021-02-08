#ifndef JOGADORES_H
#define JOGADORES_H

#include "../structs.h"
#include "main.h"

void adicionarJogador(int pid, const char *username);

void listar_jogadores();

int get_pid_By_username(const char *username);

int delete_user_by_PID(int pid);

int delete_user_by_name(const char * username);

int existe_jogador(const char* username);

Jogador *get_jogador_by_pid(int pid);

#endif