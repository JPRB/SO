#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

int pontos = 0;


void trataSinalExit (int sigNum) {
  signal(sigNum, SIG_IGN);
  exit(pontos);
}


int main()
{
  int n = 0;
  char num[255];

  signal(SIGUSR1, trataSinalExit);
  srand(time(NULL));
  setbuf(stdout, NULL);

  printf("BEM VINDO AO JOGO!\nO objetivo deste jogo e' apenas testar as funcionalidades do sistema CHAMPION.\nTente acertar no numero que o computador vai gerar entre 0 e 5\nBOA SORTE!\n");

  for (int i=0; i < 5; i++)
  {
    int randomNUM = rand() % 6;
    printf("\nAdivinhe o numero gerado: \n");
    scanf("%1[0123456789]", num);
     
    n = atoi(num);

    if (n == randomNUM) {
      printf("Bingo!!!\n");
      pontos ++;
    }
    else {
      printf("Errou ... o numero era o: %d",randomNUM);
      if (pontos > 0)
        pontos--;
    }
  }
  printf("\n\nAcabou com %d pontos\n",pontos);
  fflush(stdout);
  exit(pontos);
}
