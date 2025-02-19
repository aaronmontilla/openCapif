CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./include -I./cJSON
TARGET = opencapif_process

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ cJSON/cJSON.c -lcurl

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean