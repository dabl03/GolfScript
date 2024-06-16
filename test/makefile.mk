# Aquí podemos compilar lo que queramos. Dentro de la carpeta test. Mas libertad.
LINGC:=$(LINGC) -I"./src/"

$(APPTEST): ./test/main.c $(O_FILES) $(O_OPERATOR) $(MAIN_O)
	@echo Dependencias compiladas...
	@echo Compilamos el test...
	$(GCC) ./test/main.c $(MAIN_O) $(O_FILES) $(O_OPERATOR) -o $(APPTEST) $(LINGC) 2> $(LOG_APP)/app.exe.mk