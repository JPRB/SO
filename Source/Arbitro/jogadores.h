#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pJogador adicionarJogador(pJogador lista);

pJogador removerJogador(pJogador lista, char* nome);

int verificarExiste(pJogador lista, char* nome);

pJogador getJogadorByName(pJogador lista, char* nome);

void listaDeJogadores(pJogador lista);

