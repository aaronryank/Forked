CC := gcc
CFLAGS := -g -O3
OBJ := main.o forked.o

EXEC := interp

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)
