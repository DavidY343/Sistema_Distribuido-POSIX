CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-ldinamica -L.
LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):.

all: libdinamica.so main

libdinamica.so: lib_hola.o
	$(CC) -shared -Wl,-soname,libdinamica.so -o libdinamica.so.1.0 lib_hola.o
	ln -s libdinamica.so.1.0 libdinamica.so

lib_hola.o: lib_hola.c
	$(CC) $(CFLAGS) -fPIC -c lib_hola.c -o lib_hola.o

main: main.o
	$(CC) $(CFLAGS) -o main main.o $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f *.o *.so* main


#
#BIN_FILES  = ejemplo_lista

#CC = gcc

#CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

#LDFLAGS = -L$(INSTALL_PATH)/lib/
#LDLIBS = -lpthread -lm


#all: $(BIN_FILES)
.PHONY : all


ejemplo_lista: ejemplo_lista.o list.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(BIN_FILES) *.o

.SUFFIXES:
.PHONY : clean
//