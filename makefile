.PHONY: run

CXX=g++
GCC=gcc
CXXFLAG:=-std=c++20 -Wall
LINGC=-lgmp
CFLAG:=-Wall
APP=./gsp
APPTEST:=./test/$(APP)_test
## Vemos si bash_version esta definida, si lo esta
## entonces compilamos para linux, sino para windows.
OS_SYSTEM:=$(if $(shell uname),$(shell uname),Windows)
SRC=./src
SRC_OPERATOR=$(SRC)/operators
INCLUDE=$(SRC)/header
BIN_O=./build/obj
LOG_APP=./build/log
MAKE=make
#Main:
MAIN_O=$(BIN_O)/main.o
MAIN_SRC=$(SRC)/main.c
#*.c
C_FILES:= $(filter-out $(SRC)/main.c,$(foreach dir,$(SRC),$(wildcard $(dir)/*.c)))
C_OPERATOR:=$(SRC_OPERATOR)/operators.c $(foreach dir,$(SRC_OPERATOR),$(wildcard $(dir)/**/*.c))
#*.o
O_FILES :=$(foreach file,$(C_FILES),$(BIN_O)/$(notdir $(file:.c=.o)))
O_OPERATOR:=$(foreach file,$(C_OPERATOR),$(BIN_O)/$(notdir $(file:.c=.o)))
#*.h
FILE_H:=$(foreach file,$(C_FILES),$(INCLUDE)/$(notdir $(file:.c=.h)))
# If anything is modified, all files will be affected.
ALL_DEPENDENCIES=$(INCLUDE)/define.h
# If windows then BIN_EXT=.exe
BIN_EXT=
DELETE_FUNC=DELETE
SHOW_LOG=cat -n

ifeq ($(DEBUG),1)
	CXXFLAG:=-D DEBUG $(CXXFLAG) -g
	CFLAG:=-D DEBUG $(CFLAG) -g
	O_FILES:=$(O_FILES) $(BIN_O)/memory.o
	FILE_H:=$(FILE_H) ./test/include/memory.h
endif

ifeq ($(OS_SYSTEM),Windows)
	APP:=$(APP).exe
	BIN_EXT=.exe
	MAKE=Mingw32-make
	DELETE_FUNC=DELETE_WINDOWS
endif

define DELETE_WINDOWS
del /s *.o
del /s *.log
del /s *_test.exe
del /s *.exe
endef

define DELETE
rm -f -d $(BIN_O)/*.o
rm -f -d $(BIN_O)/test/*.o
rm -f -d $(LOG_APP)/*.log
rm -f -d $(LOG_APP)/test/*.log
rm -f -d ./*_test
rm -f -d ./test/*_test
rm -f -d "$(APP)"
endef

ifneq ($(TEST),1)
all: $(APP)
else
all: $(APPTEST) $(STACK_TEST)
CXXFLAG:=$(CXXFLAG) -g "-D TEST_=1" "-D __MAIN__=1" -static-libgcc -static-libstdc++ -ggdb
CFLAG:=$(CFLAG) -g "-D TEST_=1" "-D __MAIN__=1" -static-libgcc -static-libstdc++ -ggdb
BIN_O:=$(BIN_O)/test
LOG_APP:=$(LOG_APP)/test
O_FILES :=$(foreach file,$(C_FILES),$(BIN_O)/$(notdir $(file:.c=.o)))
O_OPERATOR:=$(foreach file,$(C_OPERATOR),$(BIN_O)/$(notdir $(file:.c=.o)))
STACK_TEST=stack.exe
include ./test/makefile.mk
endif

$(APP): $(O_FILES) $(O_OPERATOR) $(MAIN_O)
	@echo Uniendo los archivos compilados en "$(APP)"...
	@$(GCC) $(CFLAG) $(MAIN_O) $(O_FILES) $(O_OPERATOR) -o $(APP) $(LINGC) 2> $(LOG_APP)/app.exe.log

$(MAIN_O): $(MAIN_SRC) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	@$(GCC) -c $(CFLAG) $< -o $@ 2>$(LOG_APP)/main.c.log

$(BIN_O)/%.o:	$(SRC)/%.c $(ALL_DEPENDENCIES)
	@echo Compilando el archivo objeto de $<...
	@$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2> $(LOG_APP)/$(notdir $<.log)

$(BIN_O)/%.o:	$(SRC_OPERATOR)/%.c $(ALL_DEPENDENCIES)
	@echo Compilando el operador: $<...
	@$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2> $(LOG_APP)/$(notdir $<.log)

$(BIN_O)/%.o:	$(SRC_OPERATOR)/**/%.c $(ALL_DEPENDENCIES)
	@echo Compilando el operador: $<...
	@$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2> $(LOG_APP)/$(notdir $<.log)

$(BIN_O)/memory.o:./test/memory.c
	@$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2> $(LOG_APP)/memory.log

clean:
	@echo "Eliminando archivos..."
	@$(call $(DELETE_FUNC))

gdb:
	@echo Abriendo GDB
	@gdb $(APP)

run:
	@echo Ejecutando $(APP)
	@$(APP)

