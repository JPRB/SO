# $@ - Current Target
# $< - Dependency filename

CC = gcc
SOURCES = ../utils.c jogadores.c main.c  
CFLAGS = -std=c99
#  Macro - For each (Transform each source.c in source.o)
OBJ = $(SOURCES:.c=.o)
EXEC=../../Build/Arbitro

#arbitro
Arbitro: $(SOURCES) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f *.o $(EXEC)