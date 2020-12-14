# $@ - Current Target
# $< - Dependency filename

CC = gcc
SOURCES = main.c
CFLAGS = -std=c99
LDFLAGS= -pthread 
#  Macro - For each (Transform each source.c in source.o)
OBJ = $(SOURCES:.c=.o)
EXEC=../../Build/Cliente

#cliente
Cliente: $(SOURCES) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f *.o $(EXEC)