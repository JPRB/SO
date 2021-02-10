#ifndef MAIN_H
#define MAIN_H

#include "../structs.h"

void login (int *fd_arbitro, char name[MAXCHARS]);
void userCommands (const char* comm);

extern int logged;

#endif