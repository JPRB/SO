#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>

int pontos;

void trataSinalExit (int sigNum) {
  signal(sigNum, SIG_IGN);
  exit(pontos);
}


int main()
{
  pontos = 999;
  char buffer[2] = {0};
  srand(time(NULL));

  signal(SIGUSR1, trataSinalExit);

  printf("BEM VINDO AO JOGO!\nO objetivo deste jogo e' apenas testar as funcionalidades do sistema CHAMPION.\nTente acertar no numero que o computador vai gerar entre 0 e 5\nBOA SORTE!");
  fflush(stdout);
  for (int i=0; i < 5; i++)
  {
    int randomNUM = rand() % 6;
    do {
      printf("\n\nAdivinhe o numero gerado:");
      fflush(stdout);
      scanf(" %[^\n]", buffer);
      while (fgetc(stdin) != '\n');
    }
    while (!isdigit(buffer[0]));

    int n = atoi((char) buffer[0]);
 
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
