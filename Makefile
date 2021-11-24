CC=gcc
DEBUG_CFLAGS=-Wall -g -fsanitize=address
CFLAGS=-Wall
LDFLAGS=-lcurl
OBJDIR=build
SRCS=$(wildcard src/*.c)
OBJECTS=$(patsubst src/%.c, $(OBJDIR)/%.o, $(SRCS))
BIN=nog

UNAME := $(shell uname)


all: $(BIN)

debug: CFLAGS += -g -fsanitize=address
debug: $(BIN)


$(BIN): $(OBJECTS) | $(OBJDIR)
	make dependencies
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)


$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(OBJDIR):
	mkdir -p $(OBJDIR)

install: $(BIN)
	sudo cp $(BIN) /usr/local/bin/


dependencies:
ifeq ($(UNAME),Linux)
	sudo apt update
	sudo apt-get install libcurl4-gnutls-dev
else ifeq ($(UNAME), Darwin)
	brew install curl
else
	@echo "eggnog may not work properly or at all"
endif


.PHONY: tests
tests:
	./tests/testrunner


clean:
	rm -rf $(OBJDIR)
	rm $(BIN)
	rm -rf *.dSYM

