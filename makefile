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
LOG_APP=./build/log
MAKE=mingw32-make
#Main:
MAIN_O=$(BIN_O)/main.o
MAIN_SRC=$(SRC)/main.c
#*.c
C_FILES:= $(filter-out $(SRC)/main.c,$(foreach dir,$(SRC),$(wildcard $(dir)/*.c)))
#*.o
O_FILES := $(foreach file,$(C_FILES),$(BIN_O)/$(notdir $(file:.c=.o)))
#*.h
FILE_H:=$(foreach file,$(C_FILES),$(INCLUDE)/$(notdir $(file:.c=.h)))
define delete_obj
	cd $(BIN_O) && del "*.o"
	del "$(APP)"
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

$(APP): $(O_FILES) $(MAIN_O)
	@echo compilando APP...
	$(GCC) $(CFLAG) $(MAIN_O) $(O_FILES) -o $(APP) $(LINGC) 2> $(LOG_APP)/app.exe.md

$(MAIN_O): $(MAIN_SRC) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	$(GCC) -c $(CFLAG) $< -o $@ 2>$(LOG_APP)/main.c.md

$(BIN_O)/%.o:	$(SRC)/%.c
	@echo Compilando el archivo objeto de $<...
	$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2> $(LOG_APP)/$(notdir $<.mk)

clear:
	$(call delete_obj)

gdb:
	gdb $(APP)

run:
	$(APP)

log:
	$(call log)