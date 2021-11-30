CC=gcc
DEBUG_CFLAGS=-Wall -g -fsanitize=address
CFLAGS=-Wall
LDFLAGS=-lcurl
OBJDIR=build
SRCS=$(wildcard src/*.c)
OBJECTS=$(patsubst src/%.c, $(OBJDIR)/%.o, $(SRCS))
BIN=nog


all: $(BIN)

debug: CFLAGS += -g -fsanitize=address
debug: $(BIN)


$(BIN): $(OBJECTS) | $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)


$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(OBJDIR):
	mkdir -p $(OBJDIR)

install: $(BIN)
	cp $(BIN) $(NOG_INSTALL_PATH)

.PHONY: tests
tests:
	./tests/testrunner


clean:
	rm -rf $(OBJDIR)
	rm $(BIN)
	rm -rf *.dSYM

