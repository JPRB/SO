CC = gcc
SOURCES = jogo.c
CFFLAGS = -std=c99
#  Macro - For each (Transform each source.c in source.o)
OBJ = $(SOURCES:.c=.o)
EXEC=../../GameDir/G_Jogo1

#jogo
Jogo: $(SOURCES) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXEC)