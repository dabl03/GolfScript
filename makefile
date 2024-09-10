.PHONY: run

CXX=g++
GCC=gcc
CXXFLAG:=-std=c++20 -Wall
LINGC=-lgmp
CFLAG:=-Wall
APP=./gsp
APPTEST:=./test/$(APP)_test
SYSTEM_OS:=$(shell powershell -command "$PSVersionTable.OS")
## Ver si usar: Platform - Platform that the operating system is running on. The value on Linux and macOS is Unix. See $IsMacOs and $IsLinux.
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
# If windows then BIN_EXT=.exe
BIN_EXT=
DELETE=rm -f -d
SHOW_LOG=cat -n

ifeq ($(DEBUG),1)
	CXXFLAG:=$(CXXFLAG) -g
	CFLAG:=$(CFLAG) -g
endif

ifeq ($(SYSTEM_OS),Windows)
APP:=$(APP).exe
BIN_EXT=.exe
MAKE=Mingw32-make
DELETE=del
endif

ifeq ($(TEST),1)
	CXXFLAG:=$(CXXFLAG) -g "-D TEST_=1" -static-libgcc -static-libstdc++ -ggdb
	CFLAG:=$(CFLAG) -g "-D TEST_=1" -static-libgcc -static-libstdc++ -ggdb
	BIN_O:=$(BIN_O)/test
	LOG_APP:=$(LOG_APP)/test
	O_FILES :=$(foreach file,$(C_FILES),$(BIN_O)/$(notdir $(file:.c=.o)))
	O_OPERATOR:=$(foreach file,$(C_OPERATOR),$(BIN_O)/$(notdir $(file:.c=.o)))
	STACK_TEST=stack.exe
endif

ifneq ($(TEST),1)
all: $(APP)
else
all: $(APPTEST) $(STACK_TEST)
include ./test/makefile.mk
endif

$(APP): $(O_FILES) $(O_OPERATOR) $(MAIN_O)
	@echo compilando APP...
	$(GCC) $(CFLAG) $(MAIN_O) $(O_FILES) $(O_OPERATOR) -o $(APP) $(LINGC) 2> $(LOG_APP)/app.exe.log

$(MAIN_O): $(MAIN_SRC) $(FILE_H)
	@echo "Compilando el archivo objeto de main."
	$(GCC) -c $(CFLAG) $< -o $@ 2>$(LOG_APP)/main.c.log

$(BIN_O)/%.o:	$(SRC)/%.c
	@echo Compilando el archivo objeto de $<...
	$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2> $(LOG_APP)/$(notdir $<.log)

$(BIN_O)/%.o:	$(SRC_OPERATOR)/%.c
	@echo Compilando el operador: $<...
	$(GCC) -c $(CFLAG) $< -o $@ $(LINGC) 2> $(LOG_APP)/$(notdir $<.log)

clean:
	$(DELETE) $(BIN_O)/*.o
	$(DELETE) $(BIN_O)/test/*.o
	$(DELETE) $(LOG_APP)/*.log
	$(DELETE) $(LOG_APP)/test/*.log
	$(DELETE) ./*_test$(BIN_EXT)
	$(DELETE) ./test/*_test$(BIN_EXT)
	$(DELETE) "$(APP)"

gdb:
	gdb $(APP)

run:
	$(APP)

