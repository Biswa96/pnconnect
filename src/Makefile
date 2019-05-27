# Makefile for pnconnect project

NAME := pnconnect
BINDIR = ../bin
CFLAGS := -O2 -std=c99 -m64 -Wall -Wextra
LIBS := -lNtdll -lOle32 -lp9np -L"../lib"
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.obj)
BINS := $(patsubst %.obj,$(BINDIR)/%.obj,$(OBJECTS))

EXECUTABLE=$(BINDIR)/$(NAME)

all: $(BINDIR) $(EXECUTABLE)

$(EXECUTABLE): $(BINS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(BINS): $(BINDIR)/%.obj: %.c
	$(CC) -c $(CFLAGS) $< -o $(BINDIR)/$*.obj

$(BINDIR):
	@mkdir ..\bin

#END