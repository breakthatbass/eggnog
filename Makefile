CC=gcc
CFLAGS=-lcurl -Wall -g -fsanitize=address
FILES=main.c
BIN=santa

all: $(BIN)

$(BIN): $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o $(BIN)

clean:
	rm $(BIN)
