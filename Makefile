CC=gcc
DEBUG_CFLAGS=-lcurl -Wall -g -fsanitize=address
CFLAGS=-lcurl -Wall
SRCS=$(wildcard src/*.c)
BIN=nog
OBJDIR=build
OBJ=$(wildcard $(ODJDIR)/*.o)
OBJECTS=$(patsubst src/%.c, $(OBJDIR)/%.o, $(SRCS))

all=$(BIN)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)


$(OBJDIR):
	mkdir -p $(OBJDIR)

install: $(BIN)
	cp $(BIN) /usr/local/bin/

.PHONY: tests
tests:
	./tests/testrunner


clean:
	rm $(BIN)
	rm -rf $(OBJDIR)
	rm -rf *.dSYM

