CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Iinclude

TARGET = program

SRC_DIR = src
INC_DIR = include

SRCS = main.c $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) out.bmp

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

.PHONY: all run clean rebuild
