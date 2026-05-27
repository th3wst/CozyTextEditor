CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g
LDFLAGS = -lncurses

SRCS = main.c editor.c fileio.c render.c input.c
OBJS = $(SRCS:.c=.o)
EXEC = cte

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)