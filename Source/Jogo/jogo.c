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
  int num = 0;

  signal(SIGUSR1, trataSinalExit);
  setbuf(stdout, NULL);

  printf("BEM VINDO AO JOGO!\n");
  printf("O objetivo deste jogo e' apenas testar as funcionalidades do sistema CHAMPION.\nTente acertar no numero que o computador vai gerar entre 0 e 5\nBOA SORTE!\n");

  for (int i=0; i < 5; i++)
  {
    int randomNUM = rand()%6;
    srand(time(NULL));
    printf("\n\nAdivinhe o numero gerado: ");
    scanf("%d",&num );

    if (num == randomNUM) {
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

  exit(pontos);
}
