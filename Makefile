CC=gcc
CFLAGS=-lcurl -Wall -g -fsanitize=address
SRCS=$(wildcard src/*.c)
BIN=nog
OBJDIR=build
OBJ=$(wildcard $(ODJDIR)/*.o)
OBJECTS=$(patsubst src/%.cpp, $(OBJDIR)/%.o, $(SRCS))

all=$(BIN)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(BIN)


$(OBJDIR):
	mkdir -p $@

install: $(BIN)
	cp $(BIN) /usr/local/bin/

.PHONY: tests
tests:
	./tests/testrunner


clean:
	rm $(BIN)
	rm -rf $(OBJDIR)
	rm -rf *.dSYM

