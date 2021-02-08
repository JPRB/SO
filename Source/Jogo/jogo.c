#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>

int continua=1;

void trataSinalExit (int sigNum) {
  signal(sigNum, SIG_IGN);
  continua=0;
}


int main(int argc, char *argv[])
{
  int pontos = 0;
  char buffer[10] = "\0";
  srand(time(NULL));

  setbuf(stdout, NULL);
  struct sigaction act;
	
	// Function handler 
	act.sa_handler = &trataSinalExit;
	sigfillset(&act.sa_mask);
	act.sa_flags = SA_RESTART | SA_NOCLDSTOP | SA_NOCLDWAIT;
	
	if(sigaction(SIGUSR1, &act, NULL) == -1)
	{
		fprintf(stderr, "sigaction error\n");
		exit(EXIT_FAILURE);
	}
  
  printf("BEM VINDO AO JOGO!");
  fflush(stdout);
  printf("\nO objetivo deste jogo e' apenas testar as funcionalidades do sistema CHAMPION.");
  fflush(stdout);
  printf("\nTente acertar no numero que o computador vai gerar entre 0 e 5. ");
  fflush(stdout);
  printf("BOA SORTE!\n");
  fflush(stdout);
  while(continua)
  {
    int randomNUM = rand() % 6;
    /*do {
      printf("\n\nAdivinhe o numero gerado:");
      fflush(stdout);
      scanf("%1[^\n]", buffer);
      
    }
    while (!isdigit(buffer));*/
    //while (fgetc(stdin) != '\0');

    fprintf(stdout, "\n\nAdivinhe o numero gerado: ");
    fflush(stdout);
    scanf(" %[^\n]s", buffer);

    int n = atoi(buffer);
 
    if (n == randomNUM) {
      printf("Bingo!!!\n");
      fflush(stdout);
      pontos ++;
    }
    else {
      printf("Errou ... o numero era o: %d",randomNUM);
      fflush(stdout);
      if (pontos > 0)
        pontos--;
    }
  }
  printf("\n\nAcabou com %d pontos\n",pontos);
  //fflush(stdout);
  exit(pontos);
}
