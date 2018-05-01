CC = gcc

CFLAGS = -std=c99 -g -Wall -Isrc -Werror
LDFLAGS = -lm
TARGET = bin/neural.a

TEST_SRCS=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRCS))

all: $(TARGET) bin/a.out tests

bin/a.out: build/main.o $(TARGET)
	$(CC) $(LDFLAGS) $^ -o $@

build/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): build/synapse.o build/neuron.o build/ebpn.o build/tools.o
	ar rcs $@ $^
	ranlib $@

build/tools.o: src/tools.c src/tools.h
	$(CC) $(CFLAGS) -c $< -o $@	

build/ebpn.o: src/ebpn.c src/ebpn.h
	$(CC) $(CFLAGS) -c $< -o $@

build/synapse.o: src/synapse.c src/synapse.h
	$(CC) $(CFLAGS) -c $< -o $@

build/neuron.o: src/neuron.c src/neuron.h
	$(CC) $(CFLAGS) -c $< -o $@	


$(TESTS): $(TARGET)

tests: $(TESTS)
	sh ./tests/runtests.sh

clean:
	rm -f build/* bin/*
	rm -f $(TESTS)

.PHONY: all tests clean