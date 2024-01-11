.PHONY: run

CXX=g++
GCC=gcc
CXXFLAG:=-std=c++20 -Wall
LINGC=-lgmp
CFLAG:=-Wall
IF_DEBUG=1
APP=./gsp
#Por problemas que he tenido lo hago a parte.
SYSTEM_OS=WINDOWS

SRC=./src
SRC_OPERATOR=$(SRC)/operators
INCLUDE=$(SRC)/header
BIN_O=./build/obj
LOG_APP=./build/log
MAKE=mingw32-make
#Main:
MAIN_O=$(BIN_O)/main.o
MAIN_SRC=$(SRC)/main.c
#*.c
C_FILES:= $(filter-out $(SRC)/main.c,$(foreach dir,$(SRC),$(wildcard $(dir)/*.c)))
C_OPERATOR:=$(foreach dir,$(SRC_OPERATOR),$(wildcard $(dir)/*.c))
#*.o
O_FILES :=$(foreach file,$(C_FILES),$(BIN_O)/$(notdir $(file:.c=.o)))
O_OPERATOR:=$(foreach file,$(C_OPERATOR),$(BIN_O)/$(notdir $(file:.c=.o)))
#*.h
FILE_H:=$(foreach file,$(C_FILES),$(INCLUDE)/$(notdir $(file:.c=.h)))
define delete_obj
	rm -rf "$(BIN_O)/*.o"
	rm -rf "$(APP)"
endef
define append_log
//Leer los archivos logs y agregarlo en un archivo log final.
//Borrar este archivo cuando se compile la app y crearlo cuando se compile.
//Leerlo cuando se llame mingw32-make logs.
endef
ifeq (SYSTEM_OS,WINDOWS)
	APP=./gsp.exe
	define delete_obj
		cd $(BIN_O) && del "*.o"
		del "$(APP)"
	endef
endif

ifeq ($(IF_DEBUG),1)
	CXXFLAG:=$(CXXFLAG) -g
	CFLAG:=$(CFLAG) -g
endif

$(APP): $(O_FILES) $(O_OPERATOR) $(MAIN_O)
	@echo compilando APP...
	$(GCC) $(CFLAG) $(MAIN_O) $(O_FILES) $(O_OPERATOR) -o $(APP) $(LINGC) 2> $(LOG_APP)/app.exe.mk

$(MAIN_O): $(MAIN_SRC) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	$(GCC) -c $(CFLAG) $< -o $@ 2>$(LOG_APP)/main.c.mk

$(BIN_O)/%.o:	$(SRC)/%.c
	@echo Compilando el archivo objeto de $<...
	$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2> $(LOG_APP)/$(notdir $<.mk)

$(BIN_O)/%.o:	$(SRC_OPERATOR)/%.c
	@echo Compilando el operador: $<...
	$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2> $(LOG_APP)/$(notdir $<.mk)

clear:
	$(call delete_obj)

gdb:
	gdb $(APP)

run:
	$(APP)

log:
	$(call log)