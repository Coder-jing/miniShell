.PHONY:clean
CC=gcc
CFLAGS=-Wall -g
BIN=minishell
OBJS=main.o parse.o init.o excute.o buildin.o
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.o $(BIN)