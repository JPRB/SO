subDirs = $(Arbitro) $(Jogo) $(Cliente) 
Arbitro = Source/Arbitro
Cliente = Source/Cliente 
Jogo = Source/Jogo

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

.PHONY: clean

#clean
clean: 
	$(MAKE) -C $(Arbitro) clean
	$(MAKE) -C $(Jogo) clean
	$(MAKE) -C $(Cliente) clean

# clean: $(subDirs)
	#$(foreach i,$^,$(MAKE) -C  $i clean $(\n))