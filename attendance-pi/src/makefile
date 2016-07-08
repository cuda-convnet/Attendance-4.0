#Directories
INC_DIR =../src/
SRC_DIR =../src/
OBJ_DIR =../obj/
BIN_DIR =../bin/

#Compiler options
COMPILER	=gcc
CFLAGS		=-c -Wall

#Output options
BIN_NAME =attendance

#Finds header files
HEADERS := $(shell find $(INC_DIR) -name '*.h')
SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(subst $(SRC_DIR), $(OBJ_DIR), $(SOURCES:%.cpp=%.o))

all: $(BIN_NAME)

#Compiles object files
$(OBJ_DIR)%.o: %.cpp $(HEADERS)
	$(COMPILER) -o $@ $(CFLAGS) $<

#Compiles the main binary
$(BIN_NAME): $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o $(BIN_DIR)$@