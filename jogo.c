#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
  int pontos = 0, i = 0, pontos = 0, certas = 0, bull = 0;

  printf("BEM VINDO AO JOGO\n");
  printf("O objetivo deste jogo é apenas para testar as funcionalidades do sistema CHAMPION\nTente acertar no número que o computador vai gerar entre 0 e 5\n BOA SORTE!");

  while(i<5)
  {
    int randomNUM = rand();

    printf("Adivinhe o número gerado ...\n");
    scanf("%d\n",&bull );
    if (bull == randomNUM) {
              printf("Bingo!!!\n");
              pontos ++;
              i++;
    }
    else{
              printf("Errou ...\n o numero era %d"\n,&randomNUM);
              i++;
    }
  }
  printf("Parabens... acabou com %d pontos\n",&pontos); 

strand(time(NULL));
exit(pontos)
}
