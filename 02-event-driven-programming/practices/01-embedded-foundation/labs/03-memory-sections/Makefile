TARGET := lab03_memory_sections
BUILD_DIR := build
CC ?= gcc
NM ?= nm
OBJDUMP ?= objdump
READELF ?= readelf

CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow \
	-Wconversion -Wsign-conversion

SOURCE := main.c
BINARY := $(BUILD_DIR)/$(TARGET)

.PHONY: all run sections symbols elf-sections report clean rebuild help

all: $(BINARY)

$(BINARY): $(SOURCE)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

run: $(BINARY)
	./$(BINARY)

sections: $(BINARY)
	$(OBJDUMP) -h $(BINARY)

symbols: $(BINARY)
	$(NM) -S --size-sort $(BINARY)

elf-sections: $(BINARY)
	$(READELF) -S $(BINARY)

report: $(BINARY)
	@mkdir -p $(BUILD_DIR)
	$(OBJDUMP) -h $(BINARY) > $(BUILD_DIR)/section-report.txt
	$(NM) -S --size-sort $(BINARY) > $(BUILD_DIR)/symbol-report.txt
	$(READELF) -S $(BINARY) > $(BUILD_DIR)/elf-section-report.txt
	@echo "Reports generated in $(BUILD_DIR)"

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "make              Build Lab 03"
	@echo "make run          Build and run Lab 03"
	@echo "make sections     Show section headers"
	@echo "make symbols      Show symbols sorted by size"
	@echo "make elf-sections Show ELF section table"
	@echo "make report       Generate analysis reports"
	@echo "make clean        Remove Lab 03 build output"
