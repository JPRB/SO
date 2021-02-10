# $@ - Current Target
# $< - Dependency filename

CC = gcc
SOURCES = jogadores.c main.c  
CFLAGS = -std=c99 -D_XOPEN_SOURCE
LDFLAGS= -pthread
#  Macro - For each (Transform each source.c in source.o)
OBJ = $(SOURCES:.c=.o)
EXEC=../../Build/Arbitro

#arbitro
Arbitro: $(SOURCES) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f *.o $(EXEC)