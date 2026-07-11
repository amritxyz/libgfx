UNAME_S := $(shell uname -s)
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -Iglad/include -fPIC
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
INCDIR = include
SOURCES = $(wildcard $(SRCDIR)/*.c) glad/src/gl.c
HEADERS = $(wildcard $(INCDIR)/*.h)
OBJECTS = $(SOURCES:.c=.o)
TARGET = libgfx.$(SO_EXT)
DEMO = demo

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o $@ $(LDFLAGS)

$(DEMO): $(TARGET) examples/demo.c $(HEADERS)
	$(CC) $(CFLAGS) -o $(DEMO) examples/demo.c -L. -lgfx $(LDFLAGS)

example: $(DEMO)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(DEMO) glad/src/gl.o src/*.o

run: $(DEMO)
	LD_LIBRARY_PATH=. ./$(DEMO)

.PHONY: all clean example run
