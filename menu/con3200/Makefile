.PHONY: all clean

all: con3200

CFLAGS=-Wall -g -I.
LDFLAGS=-lXext -lX11 -lm

con3200: misc.o con3200.o gogol.o
	  $(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o con3200
