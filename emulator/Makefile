SHELL:=/usr/bin/bash
.SHELLFLAGS:=-eu -o pipefail -c

BUILDDIR := build
TARGET ?= emulator

SRCEXT := .c
INCEXT := .h
OBJEXT := .o

SRCDIR := src

CC := gcc
LD := gcc
LDFLAGS := 
CFLAGS := -std=gnu17 -O3 -Wall -Wextra -Wpedantic -Wno-sign-compare 

SRCS = $(shell find $(SRCDIR) -type f -name '*$(SRCEXT)')
OBJS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS:.c=.o))

_SETUP_COMMAND:=$(shell if [ ! -d $(BUILDDIR) ]; then mkdir $(BUILDDIR); fi)

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -L$(BUILDDIR) -o $@ $^


$(OBJS): $(BUILDDIR)/%$(OBJEXT) : $(SRCDIR)/%$(SRCEXT) | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $?

run:
	@./$(BUILDDIR)/$(TARGET) $(ARGUMENTS)

clean:
	rm $(OBJS)
	rm $(BUILDDIR)/$(TARGET)


.PHONY: all run clean 

