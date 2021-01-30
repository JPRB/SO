#ifndef MAIN_H
#define MAIN_H

#include "../structs.h"
#include "../utils.h"

#include <pthread.h>

void login (int *fd_arbitro);
void userCommands (const char* comm);

extern int logged;

#endif