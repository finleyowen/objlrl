CC = g++
CFLAGS = -Wall -g

SRC = tests.cpp token.cpp lexer.cpp
OBJ = $(SRC:.cpp=.o)
EXE = tests

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean: $(EXE) $(OBJ)
	rm $(EXE) $(OBJ)