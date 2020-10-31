#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "main.h"

int main (int argc, char *argv[])
{
    char *gamedir = '\0';
    char *smaxplayers = '\0'; 
    int maxplayers = 0;


    if (argc < 3) {
        perror("Missing arguments!\n");
        exit(EXIT_ERROR_ARGUMENTS);
    }
    
    // ################# ENVIRONMENT VARS ####################
    if ((gamedir = getenv("GAMEDIR")) == NULL) {
        gamedir = GAMEDIR;
    }


    smaxplayers = getenv("MAXPLAYERS");
    
    if (smaxplayers != NULL)  {
        maxplayers = atoi(smaxplayers);
    }
    else
    {
        maxplayers = MAXPLAYERS;
    }
    
    // ################# END ENVIRONMENT VARS ####################

    printf("Argc : %d GameDir: %s max players: %d\n", argc, gamedir, maxplayers);
    fflush(stdout);

    // Verify if exist arbitro in exec, by principal PIPE
    if (access(ARBITRO_PIPE, F_OK) == 0)
    {
        perror("Server already in execution!\n");
        exit(EXIT_ERROR_PIPE);
    }

    // mkfifo(3)
    // S_IRWXU - read,  write,  and  execute permission
    if (mkfifo(ARBITRO_PIPE, S_IRWXU) == -1)
    {
        perror("Error: Creating Pipe! \n");
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
        perror("Error: Closing Pipe! \n");
        exit(EXIT_ERROR_PIPE);    
    }

    return (OK);
}