
CXX=g++
CXXFLAG=-std=c++20 -Wall
APP=./GolfScript_pirata.exe
MAIN_SRC=./main.cpp

ifeq ($(strip $(OS)),Linux)
	APP=./GolfScript_pirata
endif

ifdef $(IF_DEBUG)
	CXXFLAG=$(CXXFLAG) -g
endif

all: $(APP)
$(APP): $(MAIN_SRC)
	@echo compilando...
	$(CXX) $(CXXFLAG) -o $(APP) $(MAIN_SRC) 2> logs.txt
