
#include "jogadores.h"
#include "main.c"

void adicionarJogador(int pid, const char *username) {

	lista_jogadores[nr_users].pid = pid;
    strcpy(lista_jogadores[nr_users].username, username);
  	nr_users++;
}

/*
pJogador remove_Jogador(pJogador lista, const char* nome) {
	pJogador aux = lista;
	pJogador p = aux;

	while (aux != NULL)
	{
		if (strcmp(aux->username, nome) == 0) {
			if (aux == lista) {
				lista = aux->prox;
				free(aux);
				return lista;
			}
			else
			{
				p->prox = aux->prox;
				free(aux);
				return lista;
			}
		}
		p = aux;
		aux = aux->prox;

	}
	return lista;

}

int existe_jogador(pJogador lista, const char* nome) {
	if (getJogadorByName(lista, nome) != NULL)
		return 1;

	return 0;	
}

pJogador getJogadorByName(pJogador lista, const char* nome) {
	if (lista != NULL) {
		pJogador p = lista;
		while (p != NULL)
		{
			if (strcmp(nome, p->username) == 0) {
				return p;
			}

			p = p->prox;
		}
	}
	return NULL;
}
*/

void listar_jogadores() {
	for (int i = 0; i < nr_users; i++) {
		printf("Nome: %s Jogo: %s\n", lista_jogadores[i].username, lista_jogadores[i].nome_jogo);
	
	}
}