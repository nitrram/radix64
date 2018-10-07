DEFINES:=

HEADERS:= -I/usr/local/browndeer/include
CFLAGS:=-O3 -Wall -std=c99 -march=native $(DEFINES) $(HELLO_FLAGS)
LDFLAGS:= -L/usr/local/browndeer/lib -lcoprthr -lOpenCL
CC=gcc

OBJECTS_C = $(patsubst %.c,%.o,$(wildcard *.c))
OBJECTS += $(OBJECTS_C)

all: base64.x

base64.x: $(OBJECTS)
	@echo $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -g -o $@ $(LDFLAGS)

.c.o:
	@echo "c compile"
	$(CC) $(CFLAGS) -c $<

.cl.o: .cl
	@echo "cl compile"
	$(CPP) -c $<

.PHONY: clean

clean:
	rm -f *.o *~ *.x
