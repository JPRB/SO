#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "jogadores.h"

pJogador adicionarJogador(pJogador lista /*mais paramentros*/) {
	pJogador q = malloc(sizeof(Jogador));
	memset(q, '\0', sizeof(Jogador));

	if (q == NULL) {
		printf("mensagem de erro\n");
		return NULL;
	}
	//adicionar contador de jogadores
	memset(q->pid, '\0', sizeof(q->pid));
	strcpy(q->pid, pid);
	strcpy(q->username, username);

	if (lista == NULL) {
		q->prox = lista;
		lista = q;
		
		return lista;
	}
	else {
		pJogador a = lista;
		while (t->prox !=NULL)
		{
			t = t->prox;
		}
		t->prox = q;
		return lista;
	}

}
pJogador removerJogador(pJogador lista, char* nome) {
	pJogador aux = lista;
	pJogador p = aux;

	while (aux != NULL)
	{
		if (strcmp(aux->nome, nome) == 0) {
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
int verificarExiste(pJogador lista, char* nome) {
	pJogador p = lista;
	while (p != NULL)
	{
		if (strcmp(p->nome, nome) == 0) {
			printf("jogador existente\n");
			return true;
		}
		p = p->prox;
	}
	return 0;
}
pJogador getJogadorByName(pJogador lista, char* nome) {
	pJogador p = lista;
	while (p != NULL)
	{
		if (strcmp(nome, p->nome) == 0) {
			return p;
		}

		p = p->prox;
	}
	return NULL;
}
void listaDeJogadores(pJogador lista) {
	pJogador p = lista;
	if (p == NULL)
	{
		printf("lista vazia.");
	}
	while (p =! NULL)
	{
		printf("NOME: %s\n", p->username);
		p = p ->prox;
	}
}