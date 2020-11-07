#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
  int pontos=0, i=0, corretas = 0, bull = 0;

  printf("BEM VINDO AO JOGO\n");
  printf("O objetivo deste jogo e apenas para testar as funcionalidades do sistema CHAMPION\nTente acertar no numero que o computador vai gerar entre 0 e 5\nBOA SORTE!\n");

  while(i<5)
  {
    int randomNUM = rand()%6;
    srand(time(NULL));
    printf("\n\n Adivinhe o numero gerado ...\n");
    scanf("%d",&bull );

    if (bull == randomNUM) {
              printf("Bingo!!!\n");
              corretas ++;
              pontos ++;
              i++;
    }
    else{
              printf("Errou ... o numero era %d",randomNUM);
              i++;
    }
  }
  printf("\n\nParabens... acabou com %d pontos\n",pontos);

exit(pontos);
}
