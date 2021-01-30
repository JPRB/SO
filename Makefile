
Arbitro=Source/Arbitro
Cliente=Source/Cliente 
Jogo=Source/Jogo

#all 
all: cliente arbitro jogo

#cliente
cliente: $(Cliente)
	$(MAKE) -C $<

#arbitro
arbitro: $(Arbitro)
	$(MAKE) -C $<

#jogo
jogo: $(Jogo)
	$(MAKE) -C $<

.PHONY: all clean

#clean
clean: 
	$(MAKE) -C $(Arbitro) clean
	$(MAKE) -C $(Jogo) clean
	$(MAKE) -C $(Cliente) clean

# clean: $(subDirs)
	#$(foreach i,$^,$(MAKE) -C  $i clean $(\n))