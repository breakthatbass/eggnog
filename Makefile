CC=gcc
CFLAGS=-lcurl -Wall -g -fsanitize=address
FILES=main.c aoc.c
BIN=xmas
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
	sudo cp $< /usr/local/bin/$(BIN)

clean:
	rm $(BIN)
