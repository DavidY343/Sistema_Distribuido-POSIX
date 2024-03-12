CC = gcc
CFLAGS = -fPIC -Wall -g
LDFLAGS =
LDLIBS =

PROXY_SOURCES = proxy.c

PROXY_OBJECTS = $(PROXY_SOURCES:.c=.o)

PROXY = libproxy.so
SERVER = server
CLIENT = cliente

all: $(PROXY) $(SERVER) $(CLIENT)
	


$(PROXY): $(PROXY_OBJECTS)
	$(CC) -shared -o $(PROXY) $(PROXY_OBJECTS) $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT): cliente.o
	$(CC) -o $(CLIENT) cliente.o $(LDFLAGS) $(LDLIBS) -lrt -L. -lproxy -Wl,-rpath,.

$(SERVER): servidor.o claves.o list.o
	$(CC) -g -o $(SERVER) servidor.o claves.o list.o -lrt

clean:
	rm -f $(PROXY) $(PROXY_OBJECTS) $(CLIENT) $(SERVER) *.o

re: clean all
