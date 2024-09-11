# Aquí podemos compilar lo que queramos. Dentro de la carpeta test. Mas libertad.
LINGC:=$(LINGC) -I"./src/"
TEST_MEMORY=$(BIN_O)/memory.o

$(APPTEST): ./test/main.c $(MAIN_O) $(TEST_MEMORY)  $(O_FILES) $(O_OPERATOR)
	@echo Dependencias compiladas...
	@echo Compilamos el test...
	$(GCC) $(CFLAG) ./test/main.c $(MAIN_O) $(O_FILES) $(O_OPERATOR) $(TEST_MEMORY) -o $(APPTEST)$(BIN_EXT) $(LINGC) 2> $(LOG_APP)/app.exe.log

$(STACK_TEST): test/stack.c $(O_FILES) $(O_OPERATOR) $(MAIN_O) $(TEST_MEMORY)
	@echo Compilando el test de stack...
	$(GCC) $(CFLAG) ./test/stack.c $(MAIN_O) $(O_FILES) $(O_OPERATOR) $(TEST_MEMORY) -o stack_test$(BIN_EXT) $(LINGC)

$(TEST_MEMORY): test/memory.c
	echo $(TEST_MEMORY)
	$(GCC) -g -c -o $(TEST_MEMORY) test/memory.c