CC := gcc
CFLAGS := -g -Os
OBJ := main.o forked.o

EXEC := interp

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

clean:
	rm -f $(OBJ)
