#include "jogadores.h"


void adicionarJogador(int pid, const char *username) {
	
	lista_jogadores[arbitro.nr_users].pid = pid;
    lista_jogadores[arbitro.nr_users].sus_comunicacao = 0;
	strcpy(lista_jogadores[arbitro.nr_users].username, username);
  	arbitro.nr_users++;

	pthread_mutex_init(&lista_jogadores[arbitro.nr_users].mutex, NULL);
}


void listar_jogadores() {
	char nome_jogo[MAXCHARS]= "\0";
	for (int i = 0; i < arbitro.nr_users; i++) {
		strcpy(nome_jogo, lista_jogadores[i].jogo.nome);
		printf("Nome: %s ", lista_jogadores[i].username);
		if (strcmp(nome_jogo, "") != 0)
			printf("Jogo %s\n", nome_jogo);
		else
		{
			printf("\n");
		}
		
	}
}

/*  1 - Removed 
	0 - Not found*/
int delete_user_by_PID(int pid) {
	Jogador null_users = {0};

	if (get_jogador_by_pid(pid) != NULL)
	{
		if (arbitro.nr_users > 0) {
			if (arbitro.nr_users == 1) {
    			lista_jogadores[0] = null_users;
    		} else {
	    		int i;
    			for (i = 0; i < arbitro.nr_users && lista_jogadores[i].pid != pid; i++);
      			lista_jogadores[i] = lista_jogadores[arbitro.nr_users - 1];
    		}
			arbitro.nr_users--;
			return 1;
  		}
	}
	return 0;
}

/*  
	1 - Removed 
	0 - Not found
*/
int delete_user_by_name(const char * username ) { 
	for (int i = 0; i < arbitro.nr_users; i++)
	{
		if (strcmp(lista_jogadores[i].username, username) == 0) {
			delete_user_by_PID(lista_jogadores[i].pid);
			return 1;
		}
	}
	return 0;
}

/* @return
	1 - Found 
	0 - Not found
*/
int existe_jogador(const char* username) {
	for (int i = 0; i < arbitro.nr_users; i++)
	{
		if (strcmp(lista_jogadores[i].username, username) == 0){
			return 1;
		}
	}
	return 0;	
}


/* @return
	pid - Found 
	-1 - Not found
*/
int get_pid_By_username(const char *username) {
	
	for (int i = 0; i < arbitro.nr_users; i++)
	{
		if (strcmp(lista_jogadores[i].username, username) == 0){
			return lista_jogadores[i].pid;
		}
	}
	return -1;	

}

/* @return
	jogador - FOUND
	Null - Not FOUND
*/
Jogador *get_jogador_by_pid(int pid) {
	for (int i = 0; i < arbitro.nr_users; i++)
	{	
		if (lista_jogadores[i].pid == pid){
			return &lista_jogadores[i];
		}
	}
	return NULL;
}