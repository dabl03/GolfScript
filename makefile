.PHONY: run

CXX=g++
GCC=gcc
CXXFLAG:=-std=c++20 -Wall
LINGC=-lgmp
CFLAG:=-Wall
IF_DEBUG=1
APP=./GolfScript_pirata.exe

SRC=./src
INCLUDE=$(SRC)/include
BIN_O=./build/obj
#BIN_O_INCLUDE estará en un lugar a parte porque los archivos que se guarden ahi solo será necesario compilarlo una vez.
BIN_O_INCLUDE=./build/include

MAIN_SRC=$(SRC)/main.c
MAIN_O=$(BIN_O)/main.o

STR=$(SRC)/str.c
STACK=$(SRC)/stack.c
RUN=$(SRC)/run.c
GLOBAL_VARS=$(SRC)/global_vars.c

STR_O=$(BIN_O)/str.o
STACK_O=$(BIN_O)/stack.o
RUN_O=$(BIN_O)/run.o
GLOBAL_VARS_O=$(BIN_O)/global_vars.o

OBJ_S=$(RUN_O) $(STACK_O) $(STR_O) $(GLOBAL_VARS_O)
LOG=./build/log
LOG_OBJ=./build/log/log_obj.txt
LOG_APP=./build/log/log_app.txt

FILE_H=$(INCLUDE)/run.h $(INCLUDE)/define.h $(INCLUDE)/str.h $(INCLUDE)/stack.h $(INCLUDE)/global_vars.h
MAKE=mingw32-make

define delete_obj
	cd $(BIN_O) && del "*.o"
	del "$(APP)"
endef
define out_log
	@echo sucess
	ifeq $(error_code) 0
		@echo la compilacion termino con error codes: $(error_code)
		@cat < $(log)
	endif
endef
define append_log
//Leer los archivos logs y agregarlo en un archivo log final.
//Borrar este archivo cuando se compile la app y crearlo cuando se compile.
//Leerlo cuando se llame mingw32-make logs.
endef
ifeq ($(strip $(OS)),Linux)
	APP=./GolfScript_pirata
	define delete_obj
		rm -rf "$(BIN_O)/*.o"
		rm -rf "$(APP)"
	endef
	MAKE=make
endif

ifeq ($(IF_DEBUG),1)
	CXXFLAG:=$(CXXFLAG) -g
	CFLAG:=$(CFLAG) -g
endif

$(APP): $(OBJ_S) $(MAIN_O)
	@echo compilando APP...
	$(GCC) $(CFLAG) $(MAIN_O) $(OBJ_S) -o $(APP) $(LINGC) 2> $(LOG_APP)

$(MAIN_O): $(MAIN_SRC) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	$(GCC) -c $(CFLAG) $< -o $@ 2>$(LOG)/main.log

$(STR_O): $(STR) $(FILE_H)
	@echo "Compilando el archivo objeto de $(STR)."
	$(GCC) -c $(CFLAG) $< -o $@ 2>$(LOG)/str.log

$(STACK_O): $(STACK) $(FILE_H)
	@echo "Compilando el archivo objeto de $(STACK)."
	$(GCC) -c $(CFLAG) $< -o $@ 2>$(LOG)/stack.log

$(RUN_O): $(RUN) $(FILE_H)
	@echo "Compilando el archivo objeto de $(RUN)."
	$(GCC) -c $(CFLAG) $< -o $@ 2>$(LOG)/run.log

$(GLOBAL_VARS_O): $(GLOBAL_VARS) $(FILE_H)
	@echo "Compilando el archivo objeto de $(GLOBAL_VARS)."
	$(GCC) -c $(CFLAG) $< -o $@ 2>$(LOG)/global_vars.log

clear:
	$(call delete_obj)

gdb:
	gdb $(APP)

run:
	$(APP)

log:
	$(call log)