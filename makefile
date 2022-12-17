.PHONY: run

CXX=g++
CXXFLAG:=-std=c++20 -Wall
IF_DEBUG=1
APP=./GolfScript_pirata.exe

SRC=./src
BIN_O=./build/obj

MAIN_SRC=$(SRC)/main.cpp
MAIN_O=$(BIN_O)/main.o

STR=$(SRC)/str.cpp
STACK=$(SRC)/stack.cpp
RUN=$(SRC)/run.cpp

STR_O=$(BIN_O)/str.o
STACK_O=$(BIN_O)/stack.o
RUN_O=$(BIN_O)/run.o

OBJ_S=$(RUN_O) $(STACK_O) $(STR_O)

LOG_OBJ=./build/log/log_obj.txt
LOG_APP=./build/log/log_app.txt

FILE_H=$(SRC)/include/run.h $(SRC)/include/define.h $(SRC)/include/str.h $(SRC)/include/stack.h
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

clear:
	$(call delete_obj)

gdb:
	gdb $(APP)

run:
	$(APP)