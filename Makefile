CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude -O3
LDFLAGS = -lm

SRC_DIR = src
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
TARGET = $(BIN_DIR)/nanostream

all: $(TARGET)

$(TARGET): $(SRCS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR) output.fasta

.PHONY: all clean
