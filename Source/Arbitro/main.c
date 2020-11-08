#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "main.h"


int main (int argc, char *argv[])
{   
    Helper helper;
    Arbitro arbitro;

    if (argc != 3) {
        perro("Missing arguments!\n");
        exit(EXIT_ERROR_ARGUMENTS);
    }
    
// ################# CMD LINE ARGS ####################
    if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0){
        perro("Invalid Arguments!\n");
        exit(EXIT_INVALID_ARGUMENTS);
    }

    arbitro.duracao_campeonato = atoi(argv[1]);
    arbitro.tempo_espera = atoi(argv[2]);

// ################# END CMD LINE ARGS ####################




    // ################# ENVIRONMENT VARS ####################
    if ((arbitro.gamedir = getenv("GAMEDIR")) == NULL) {
        arbitro.gamedir = GAMEDIR;
    }


    helper.smaxplayers = getenv("MAXPLAYERS");
    
    if (helper.smaxplayers != NULL)  {
        arbitro.maxplayers = atoi(helper.smaxplayers);
    }
    else
    {
        arbitro.maxplayers = MAXPLAYERS;
    }
    
    // ################# END ENVIRONMENT VARS ####################

    printf("Nº args : %d GameDir: %s max players: %d\n"
    "Duracao campeonato: %d Tempo espera: %d\n", argc, arbitro.gamedir, arbitro.maxplayers,
     arbitro.duracao_campeonato, arbitro.tempo_espera);
    fflush(stdout);

    // Verify if exist arbitro in exec, by principal PIPE
    if (access(ARBITRO_PIPE, F_OK) == 0)
    {
        perro("Server already in execution!\n");
        exit(EXIT_ERROR_PIPE);
    }

    // mkfifo(3)
    // S_IRWXU - read,  write,  and  execute permission
    if (mkfifo(ARBITRO_PIPE, S_IRWXU) == -1)
    {
        perro("Error: Creating Pipe! \n");
        exit(EXIT_ERROR_PIPE);
    }

    
    getchar();

    /*
    int c;
  while ((c = getopt (argc, argv, "")) != -1) {
    printf("Argc : %d ARGV:", argc);
    fflush(stdout);

    for (int i=0; i < argc; i++)
        printf("%s", argv[i]);
        fflush(stdout);
  }
*/


    if (unlink(ARBITRO_PIPE) == -1){
        perro("Error: Closing Pipe! \n");
        exit(EXIT_ERROR_PIPE);    
    }

    return (OK);
}