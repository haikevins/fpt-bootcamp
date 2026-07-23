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

.PHONY: all clean rebuild size sections symbols symbols-size disasm \
	flash-stlink flash-openocd erase help

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

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "Project build : make | make rebuild | make clean"
	@echo "Inspection    : make size | make sections | make symbols | make symbols-size | make disasm"
	@echo "Flash         : make flash-stlink | make flash-openocd | make erase"
	@echo "Labs          : enter a lab directory and run its local Makefile"

-include $(DEPENDENCIES)
