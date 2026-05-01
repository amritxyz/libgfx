UNAME_S := $(shell uname -s)
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -fPIC
LDFLAGS = -lm

ifeq ($(UNAME_S), Linux)
    CFLAGS += -D_LINUX
    LDFLAGS += -lglfw -lGL -lX11 -lpthread -ldl
    SO_EXT = so
endif

ifeq ($(UNAME_S), Darwin)
    CFLAGS += -D_MACOS
    LDFLAGS += -lglfw -framework OpenGL -framework Cocoa -framework IOKit
    SO_EXT = dylib
endif

ifeq ($(OS), Windows_NT)
    CFLAGS += -D_WINDOWS
    LDFLAGS += -lglfw3 -lopengl32 -lgdi32 -lws2_32
    SO_EXT = dll
    CC = x86_64-w64-mingw32-gcc
endif

SRCDIR = src
OBJDIR = obj
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = libgraphics.$(SO_EXT)

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o $@ $(LDFLAGS)

example: $(TARGET)
	$(CC) $(CFLAGS) -o demo examples/demo.c -L. -lgraphics $(LDFLAGS)

clean:
	rm -rf $(OBJDIR) $(TARGET) demo

run: $(TARGET)
	$(CC) $(CFLAGS) -o demo examples/demo.c -L. -lgraphics $(LDFLAGS)
	LD_LIBRARY_PATH=. ./demo

.PHONY: all clean example
