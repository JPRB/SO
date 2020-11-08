subDirs = $(Arbitro) $(Cliente) $(Jogo)
Arbitro = Source/Arbitro
Cliente = Source/Cliente 
Jogo = Source/Jogo
Build = Build/* GameDir/*
Sources = Source/Arbitro/*.o Source/Cliente/*.o Source/Jogo/*.o

#all 
all: $(subDirs)

$(subDirs):
	$(MAKE) -C $@

#cliente
cliente: $(Cliente)
	$(MAKE) -C $<

#arbitro
arbitro: $(Arbitro)
	$(MAKE) -C $<

#jogo
jogo: $(Jogo)
	$(MAKE) -C $<

.PHONY: all $(subDirs)

#clean
clean:
	rm -f $(Build) $(Sources)