CC = gcc
EXEC = bin/portscan
SRC = src/main.c 
OBJ = $(SRC:.c=.o)
CFLAGS = -I include -lws2_32

all: $(EXEC)

%.o : %.c
	$(CC) -o $@ -c $< 

$(EXEC) : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)