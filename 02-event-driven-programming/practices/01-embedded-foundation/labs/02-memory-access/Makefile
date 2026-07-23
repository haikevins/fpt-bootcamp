TARGET := lab02_memory_access
BUILD_DIR := build
CC ?= gcc

CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow \
	-Wconversion -Wsign-conversion

SOURCE := main.c
BINARY := $(BUILD_DIR)/$(TARGET)

.PHONY: all run clean rebuild help

all: $(BINARY)

$(BINARY): $(SOURCE)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

run: $(BINARY)
	./$(BINARY)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "make         Build Lab 02"
	@echo "make run     Build and run Lab 02"
	@echo "make rebuild Clean and rebuild Lab 02"
	@echo "make clean   Remove Lab 02 build output"
