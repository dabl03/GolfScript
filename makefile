.PHONY: run

CXX=g++
CXXFLAG=-std=c++20 -Wall
APP=./GolfScript_pirata.exe

DELETED_OBJS=DEL "$(APP)"

SRC=./include
BIN_O=./build/obj

MAIN_SRC=./main.cpp
MAIN_O=$(BIN_O)/main.o

STR=$(SRC)/str.cpp
STACK=$(SRC)/stack.cpp
RUN=$(SRC)/run.cpp

STR_O=$(BIN_O)/str.o
STACK_O=$(BIN_O)/stack.o
RUN_O=$(BIN_O)/run.o

OBJ_S=$(STR_O) $(RUN_O) $(STACK_O)

LOG_OBJ=./build/log/log_obj.txt
LOG_APP=./build/log/log_app.txt

ifeq ($(strip $(OS)),Linux)
	APP=./GolfScript_pirata
	DELETED_OBJS=rm -rf ./BIN_O/*.o
endif

ifdef $(IF_DEBUG)
	CXXFLAG=$(CXXFLAG) -g
endif


$(APP): $(OBJ_S) $(MAIN_O)
	@echo compilando...
	$(CXX) $(CXXFLAG) $(MAIN_O) $(OBJ_S) -o $(APP) 2> $(LOG_APP)

$(MAIN_O): $(MAIN_SRC)
	@echo "Compilando el archivo objeto de main."
	$(CXX) -c $(CXXFLAG) $< -o $@ 2>$(LOG_OBJ)

$(STR_O): $(STR)
	@echo "Compilando el archivo objeto de main."
	$(CXX) -c $(CXXFLAG) $< -o $@ 2>$(LOG_OBJ)

$(STACK_O): $(STACK)
	@echo "Compilando el archivo objeto de main."
	$(CXX) -c $(CXXFLAG) $< -o $@ 2>$(LOG_OBJ)

$(RUN_O): $(RUN)
	@echo "Compilando el archivo objeto de main."
	$(CXX) -c $(CXXFLAG) $< -o $@ 2>$(LOG_OBJ)

clear:
	$(DELETED_OBJS) 