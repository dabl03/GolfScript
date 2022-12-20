.PHONY: run

CXX=g++
CXXFLAG:=-std=c++20 -Wall
IF_DEBUG=1
APP=./GolfScript_pirata.exe

SRC=./src
INCLUDE=$(SRC)/include
BIN_O=./build/obj
#BIN_O_INCLUDE estará en un lugar a parte porque los archivos que se guarden ahi solo será necesario compilarlo una vez.
BIN_O_INCLUDE=./build/include

MAIN_SRC=$(SRC)/main.cpp
MAIN_O=$(BIN_O)/main.o

STR=$(SRC)/str.cpp
STACK=$(SRC)/stack.cpp
RUN=$(SRC)/run.cpp
BIGINT=$(SRC)/BigInt.cpp

STR_O=$(BIN_O)/str.o
STACK_O=$(BIN_O)/stack.o
RUN_O=$(BIN_O)/run.o
BIGINT_O=$(BIN_O_INCLUDE)/BigInt.o

OBJ_S=$(RUN_O) $(STACK_O) $(STR_O) $(BIGINT_O)

LOG_OBJ=./build/log/log_obj.txt
LOG_APP=./build/log/log_app.txt

FILE_H=$(INCLUDE)/run.h $(INCLUDE)/define.h $(INCLUDE)/str.h $(INCLUDE)/stack.h $(INCLUDE)/BigInt.hpp
define delete_obj
	cd $(BIN_O) && del "*.o"
	del "$(APP)"
endef
ifeq ($(strip $(OS)),Linux)
	APP=./GolfScript_pirata
	define delete_obj
		rm -rf "$(BIN_O)/*.o"
		rm -rf "$(APP)"
	endef
endif

ifeq ($(IF_DEBUG),1)
	CXXFLAG:=$(CXXFLAG) -g
endif

$(APP): $(OBJ_S) $(MAIN_O)
	@echo compilando...
	$(CXX) $(CXXFLAG) $(MAIN_O) $(OBJ_S) -o $(APP) 2> $(LOG_APP)

$(MAIN_O): $(MAIN_SRC) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	$(CXX) -c $(CXXFLAG) $< -o $@ 2>$(LOG_OBJ)

$(STR_O): $(STR) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	$(CXX) -c $(CXXFLAG) $< -o $@ 2>$(LOG_OBJ)

$(STACK_O): $(STACK) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	$(CXX) -c $(CXXFLAG) $< -o $@ 2>$(LOG_OBJ)

$(RUN_O): $(RUN) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	$(CXX) -c $(CXXFLAG) $< -o $@ 2>$(LOG_OBJ)

$(BIGINT_O):$(BIGINT)
	@echo "Compilando la biblioteca BigInt"
	$(CXX) -c $(CXXFLAG) $< -o $@ 2>$(LOG_OBJ)
clear:
	$(call delete_obj)

gdb:
	gdb $(APP)

run:
	$(APP)