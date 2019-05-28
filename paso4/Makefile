CC=gcc
CFLAGS=-Wall

all: procFork

include syscall-reporter.mk

procFork: procFork.o syscall-reporter.o

.PHONY: clean
clean:
	rm -f procFork procFork.o
