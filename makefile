.PHONY: run

CXX=g++
GCC=gcc
CXXFLAG:=-std=c++20 -Wall
LINGC=-lgmp -lm
CFLAG:=-Wall
APP=./gsp
APPTEST:=./test/$(APP)_test
## Vemos si bash_version esta definida, si lo esta
## entonces compilamos para linux, sino para windows.
OS_SYSTEM:=$(if $(shell uname),$(shell uname),Windows)
SRC=./src
SRC_OPERATOR=$(SRC)/operators
INCLUDE=$(SRC)/header
BASE=./build
BIN_O=$(BASE)/obj
BIN_OPT=$(BIN_O)/operator
LOG_APP=$(BASE)/log
AUTO_DIRS=$(BASE) $(BIN_O) $(BIN_O)/operator $(BIN_O)/test $(LOG_APP) $(LOG_APP)/operator $(LOG_APP)/test

MAKE=make
#Main:
MAIN_O=$(BIN_O)/main.o
MAIN_SRC=$(SRC)/main.c
#*.c
C_FILES:= $(filter-out $(SRC)/main.c,$(foreach dir,$(SRC),$(wildcard $(dir)/*.c)))
C_OPERATOR:=$(SRC_OPERATOR)/operators.c
MAIN_OPT=$(BIN_OPT)/operators.o
C_OPERATORS:=$(wildcard $(SRC_OPERATOR)/**/*.c)
#*.o
O_OPERATOR:=$(foreach file,$(C_OPERATORS),$(BIN_OPT)/$(notdir $(file:.c=.o)))
O_FILES :=$(foreach file,$(C_FILES),$(BIN_O)/$(notdir $(file:.c=.o))) $(MAIN_OPT) $(O_OPERATOR)

#*.h
FILE_H:=$(foreach file,$(C_FILES),$(INCLUDE)/$(notdir $(file:.c=.h)))
# If anything is modified, all files will be affected.
ALL_DEPENDENCIES=$(INCLUDE)/define.h
# If windows then BIN_EXT=.exe
BIN_EXT=
SET_LOG=$(LOG_APP)/$(patsubst %.o,%,$(patsubst %.c,%,$(notdir $@))).log
SET_LOG_OPT=$(LOG_APP)/operator/$(patsubst %.o,%,$(patsubst %.c,%,$(notdir $@))).log
SHOW_LOG=|| ("$(VIEW_LOG)" "$(SET_LOG)" && exit 1)
SHOW_LOG_OPT=|| ("$(VIEW_LOG)" "$(SET_LOG_OPT)" && exit 1)
DELETE_FUNC=DELETE

# Mostrar los logs.
VIEW_LOG_C=./tools/view_log.c
VIEW_LOG=$(BIN_O)/view_log

DELETE_V_LOG_C=./tools/delete_void_logs.c
DELETE_V_LOG=$(BIN_O)/delete_void_logs

CLEAR_BUILD_C=./tools/clear_build.c
CLEAR_BUILD=$(BIN_O)/clear_build

ifeq ($(DEBUG),1)
	CXXFLAG:=-D DEBUG $(CXXFLAG) -g
	CFLAG:=-D DEBUG $(CFLAG) -g
	O_FILES:=$(O_FILES) $(BIN_O)/memory.o
	FILE_H:=$(FILE_H) ./test/header/memory.h
endif

ifeq ($(OS_SYSTEM),Windows)
	APP:=$(APP).exe
	BIN_EXT=.exe
	MAKE=Mingw32-make

	VIEW_LOG:=$(VIEW_LOG).exe
	DELETE_V_LOG:=$(DELETE_V_LOG).exe
	CLEAR_BUILD:=$(CLEAR_BUILD).exe
	SET_LOG=$(LOG_APP)/$(patsubst %$(BIN_EXT),%,$(patsubst %.o,%,$(patsubst %.c,%,$(notdir $@)))).log
	SHOW_LOG=|| ("$(VIEW_LOG)" "$(SET_LOG)" && exit /b 1)
	# Es necesario para mkdir de windows:
	AUTO_DIRS:=$(subst $e/,\,$(AUTO_DIRS))
	WIN_START=@start 
endif

ifneq ($(TEST),1)
all: $(BIN_O) $(LOG_APP) $(VIEW_LOG) $(DELETE_V_LOG) $(CLEAR_BUILD) $(APP)
else
all: $(BIN_O) $(LOG_APP) $(VIEW_LOG) $(DELETE_V_LOG) $(CLEAR_BUILD) $(APPTEST) $(STACK_TEST)
CXXFLAG:=$(CXXFLAG) -g "-D TEST_=1" "-D __MAIN__=1" -static-libgcc -static-libstdc++ -ggdb
CFLAG:=$(CFLAG) -g "-D TEST_=1" "-D __MAIN__=1" -static-libgcc -static-libstdc++ -ggdb
BIN_O:=$(BIN_O)/test
LOG_APP:=$(LOG_APP)/test
O_FILES :=$(foreach file,$(C_FILES),$(BIN_O)/$(notdir $(file:.c=.o)))
STACK_TEST=stack.exe
include ./test/makefile.mk
endif

$(BIN_O):
	-@mkdir $(AUTO_DIRS)

$(LOG_APP):
	-@mkdir $(AUTO_DIRS)

$(VIEW_LOG): $(VIEW_LOG_C)
	@echo Compilando herramienta de logs...
	@$(GCC) -g -o $(VIEW_LOG) $(VIEW_LOG_C) 2>$(SET_LOG)

$(DELETE_V_LOG): $(DELETE_V_LOG_C) $(VIEW_LOG_C)
	@echo Compilando herramienta para limpiar logs...
	@$(GCC) -g -o $(DELETE_V_LOG) $(DELETE_V_LOG_C) 2>$(SET_LOG)

$(CLEAR_BUILD): $(CLEAR_BUILD_C) $(VIEW_LOG_C)
	@echo Compilando herramienta para limpiar logs...
	@$(GCC) -g -o $(CLEAR_BUILD) $(CLEAR_BUILD_C) 2>$(SET_LOG)

$(APP): $(O_FILES) $(MAIN_O)
	@echo Uniendo los archivos compilados en "$(APP)"...
	@$(GCC) $(CFLAG) $(MAIN_O) $(O_FILES) -o $(APP) $(LINGC) 2>$(SET_LOG) $(SHOW_LOG)
	@$(DELETE_V_LOG) $(SET_LOG)

$(MAIN_O): $(MAIN_SRC) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	@$(GCC) -c $(CFLAG) $< -o $@ 2>$(SET_LOG) $(SHOW_LOG)
	@$(DELETE_V_LOG) $(SET_LOG)

$(BIN_O)/%.o:	$(SRC)/%.c $(ALL_DEPENDENCIES)
	@echo Compilando el archivo objeto de $<...
	@$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2>$(SET_LOG) $(SHOW_LOG)
	@$(DELETE_V_LOG) $(SET_LOG)

$(MAIN_OPT): $(C_OPERATOR) $(ALL_DEPENDENCIES)
	@echo Compilando el operador principal: $<...
	@$(GCC) -c $(CFLAG) $(C_OPERATOR) -o $(MAIN_OPT) $(LINGC) 2>$(SET_LOG_OPT) $(SHOW_LOG_OPT)
	@$(DELETE_V_LOG) $(SET_LOG_OPT)


# Solo funciona en linux
include ./src/operators/makefile.mk


$(BIN_O)/memory.o:./test/memory.c
	@$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) -DMAIN=1 2>$(SET_LOG) $(SHOW_LOG)
	@$(DELETE_V_LOG) $(SET_LOG)

clean: $(CLEAR_BUILD)
	@echo "Eliminando archivos..."
	$(WIN_START)$(CLEAR_BUILD) "$(LOG_APP)" "$(LOG_APP)/test"
	$(WIN_START)$(CLEAR_BUILD) "$(BIN_O)" "$(BIN_O)/test" "$(BIN_OPT)"
	$(WIN_START)$(CLEAR_BUILD) "$(APP)"

gdb:
	@echo Abriendo GDB
	@gdb $(APP)

run:
	@echo Ejecutando $(APP)
	@$(APP)
