CC=gcc
CFLAGS=-lcurl -Wall -g #-fsanitize=address
FILES=main.c aoc.c
BIN=nog
# get the os
UNAME_S := $(shell uname)

all: $(BIN)

$(BIN): $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o $(BIN)

install: $(BIN)
	@echo 'installing dependencies: libcurl'
ifeq ($(UNAME_S), Linux)
	sudo apt-get update
	sudo apt-get install curl
else
	brew install curl
endif
# sorry windows
	mkdir -p /usr/local/bin
	cp $< /usr/local/bin/$(BIN)

uninstall: $(BIN)
	rm /usr/local/bin/$(BIN)
	rm -rf ~/.xmas

.PHONY: tests
tests:
	sh test.sh

clean:
	rm $(BIN)
