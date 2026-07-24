TARGET := lab02_initial_task_stack
BUILD_DIR ?= build
CC ?= cc

CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS := -fsanitize=address,undefined

LIB_SOURCES := $(filter-out src/main.c,$(wildcard src/*.c))
TEST_SOURCES := $(wildcard tests/*.c)
DEMO_SOURCES := $(wildcard src/*.c)

TEST_BIN := $(BUILD_DIR)/$(TARGET)-tests
DEMO_BIN := $(BUILD_DIR)/$(TARGET)-demo

.PHONY: all test run clean rebuild help

all: test

test: $(TEST_BIN)
	ASAN_OPTIONS=detect_leaks=1 ./$(TEST_BIN)

$(TEST_BIN): $(LIB_SOURCES) $(TEST_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(DEMO_BIN): $(DEMO_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ $(LDFLAGS) -o $@

run: $(DEMO_BIN)
	ASAN_OPTIONS=detect_leaks=1 ./$(DEMO_BIN)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "make test   Build and run this lab's host tests"
	@echo "make run    Build and run this lab's demo"
	@echo "make clean  Remove this lab's build output"
