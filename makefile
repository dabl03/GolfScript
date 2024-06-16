.PHONY: run

CXX=g++
GCC=gcc
CXXFLAG:=-std=c++20 -Wall
LINGC=-lgmp
CFLAG:=-Wall
IF_DEBUG=1
APP=gsp
APPTEST:=./test/$(APP)_test
#Sistema Operativo. Otro valor para Linux.
SYSTEM_OS:=$(SYSTEM_OS)WINDOWS

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
C_OPERATOR:=$(foreach dir,$(SRC_OPERATOR),$(wildcard $(dir)/*.c))
#*.o
O_FILES :=$(foreach file,$(C_FILES),$(BIN_O)/$(notdir $(file:.c=.o)))
O_OPERATOR:=$(foreach file,$(C_OPERATOR),$(BIN_O)/$(notdir $(file:.c=.o)))
#*.h
FILE_H:=$(foreach file,$(C_FILES),$(INCLUDE)/$(notdir $(file:.c=.h)))

ifeq ($(SYSTEM_OS),WINDOWS)
APP:=$(APP).exe
MAKE=Mingw32-make
APPTEST:=$(APPTEST).exe
define delete_obj
	cd $(BIN_O) && del "*.o"
	del "$(APP)"
endef
else
define delete_obj
	rm $(BIN_O)/*.o
	rm -rf "$(APP)"
endef
endif

define append_log
//Leer los archivos logs y agregarlo en un archivo log final.
//Borrar este archivo cuando se compile la app y crearlo cuando se compile.
//Leerlo cuando se llame mingw32-make logs.
endef

ifeq ($(TEST),1)
	CXXFLAG:=$(CXXFLAG) "-D TEST_=1"
	CFLAG:=$(CFLAG) "-D TEST_=1"
	BIN_O:=$(BIN_O)/test
	LOG_APP:=$(LOG_APP)/test
	O_FILES :=$(foreach file,$(C_FILES),$(BIN_O)/$(notdir $(file:.c=.o)))
	O_OPERATOR:=$(foreach file,$(C_OPERATOR),$(BIN_O)/$(notdir $(file:.c=.o)))
endif

ifneq ($(TEST),1)
all: $(APP)
else
all: $(APPTEST)
include ./test/makefile.mk
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

clean:
	$(call delete_obj)

gdb:
	gdb $(APP)

run:
	$(APP)

log:
	$(call log)
