#Directories
INC_DIR =../src/
SRC_DIR =../src/
OBJ_DIR =../obj/
BIN_DIR =../bin/

#Compiler options
COMPILER	=g++
CFLAGS		=-c -Wall -std=c++11
FLAGS		=-fexceptions -lbcm2835 -pthread

#Output options
BIN_NAME =attendance

#Finds header files
HEADERS := $(shell find $(INC_DIR) -name '*.h')
SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(subst $(SRC_DIR), $(OBJ_DIR), $(SOURCES:%.cpp=%.o))

all: $(BIN_NAME)

#Compiles object files
$(OBJ_DIR)%.o: %.cpp $(HEADERS)
	$(COMPILER) -o $@ $(CFLAGS) $(FLAGS) $<

#Compiles the main binary
$(BIN_NAME): $(OBJECTS)
	$(COMPILER) $(OBJECTS) $(FLAGS) -o $(BIN_DIR)$@