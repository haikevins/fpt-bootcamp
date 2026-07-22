TARGET := embedded_foundation

BUILD_DIR := build
SRC_DIR := src
STARTUP_DIR := startup
INCLUDE_DIR := include
LINKER_SCRIPT := linker/memory.ld

CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size
NM := arm-none-eabi-nm
HOST_CC ?= gcc

CPU_FLAGS := -mcpu=cortex-m3 -mthumb

CFLAGS := $(CPU_FLAGS) \
	-std=c11 -Og -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-ffreestanding -fno-builtin -fno-common \
	-fno-unwind-tables -fno-asynchronous-unwind-tables \
	-ffunction-sections -fdata-sections \
	-MMD -MP -I$(INCLUDE_DIR)

LDFLAGS := $(CPU_FLAGS) \
	-nostdlib -nostartfiles \
	-T$(LINKER_SCRIPT) \
	-Wl,--gc-sections \
	-Wl,--print-memory-usage \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map

SOURCES := \
	$(STARTUP_DIR)/startup.c \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/gpio.c \
	$(SRC_DIR)/uart.c

OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
LST := $(BUILD_DIR)/$(TARGET).lst

HOST_CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow \
	-Wconversion -Wsign-conversion

.PHONY: all clean rebuild size sections symbols symbols-size disasm \
	flash-stlink flash-openocd erase host-labs lab01 lab02 lab03 lab04 lab05 lab06 lab07 lab08 all-labs help

all: $(ELF) $(BIN) $(HEX) $(LST) size

$(ELF): $(OBJECTS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(LST): $(ELF)
	$(OBJDUMP) -d -S $< > $@

size: $(ELF)
	$(SIZE) $<

sections: $(ELF)
	$(OBJDUMP) -h $<

symbols: $(ELF)
	$(NM) -n $<

symbols-size: $(ELF)
	$(NM) -S --size-sort $<

disasm: $(LST)
	@echo "Generated $(LST)"

flash-stlink: $(BIN)
	st-flash write $(BIN) 0x08000000

flash-openocd: $(ELF)
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $(ELF) verify reset exit"

erase:
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "init; reset halt; stm32f1x mass_erase 0; reset run; exit"

lab01:
	@mkdir -p $(BUILD_DIR)/labs
	$(HOST_CC) $(HOST_CFLAGS) labs/01-endianness/main.c \
		-o $(BUILD_DIR)/labs/lab01-endianness

lab02:
	@mkdir -p $(BUILD_DIR)/labs
	$(HOST_CC) $(HOST_CFLAGS) labs/02-memory-access/main.c \
		-o $(BUILD_DIR)/labs/lab02-memory-access

lab03:
	@mkdir -p $(BUILD_DIR)/labs
	$(HOST_CC) $(HOST_CFLAGS) labs/03-memory-sections/main.c \
		-o $(BUILD_DIR)/labs/lab03-memory-sections

host-labs: lab01 lab02 lab03

lab04:
	$(MAKE) -C labs/04-startup-code \
		BUILD_DIR=../../build/labs/lab04

lab05:
	$(MAKE) -C labs/05-linker-script \
		BUILD_DIR=../../build/labs/lab05

lab06:
	$(MAKE) -C labs/06-gpio-register \
		BUILD_DIR=../../build/labs/lab06

lab07:
	$(MAKE) -C labs/07-uart-polling \
		BUILD_DIR=../../build/labs/lab07

lab08:
	$(MAKE) -C labs/08-map-analysis \
		BUILD_DIR=../../build/labs/lab08 report

all-labs: host-labs lab04 lab05 lab06 lab07 lab08

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "Firmware: make | make size | make sections | make symbols-size"
	@echo "Flash   : make flash-stlink | make flash-openocd | make erase"
	@echo "Labs    : make lab01 ... make lab08 | make all-labs"
	@echo "Clean   : make clean | make rebuild"

-include $(DEPENDENCIES)
