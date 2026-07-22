TARGET := lab08_map_analysis
BUILD_DIR ?= build
LINKER_SCRIPT := memory.ld

CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size
NM := arm-none-eabi-nm

CPU_FLAGS := -mcpu=cortex-m3 -mthumb

CFLAGS := $(CPU_FLAGS) \
	-std=c11 \
	-Og \
	-g3 \
	-Wall \
	-Wextra \
	-Werror \
	-Wshadow \
	-Wundef \
	-Wconversion \
	-Wsign-conversion \
	-ffreestanding \
	-fno-builtin \
	-fno-common \
	-ffunction-sections \
	-fdata-sections \
	-MMD \
	-MP \
	-I.

LDFLAGS := $(CPU_FLAGS) \
	-nostdlib \
	-nostartfiles \
	-T$(LINKER_SCRIPT) \
	-Wl,--gc-sections \
	-Wl,--print-memory-usage \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map

SOURCES := \
	startup.c \
	main.c

OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
LST := $(BUILD_DIR)/$(TARGET).lst

.PHONY: all clean rebuild size sections symbols report flash-stlink flash-openocd

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
	$(NM) -S --size-sort $<

report: $(ELF)
	@mkdir -p $(BUILD_DIR)
	$(SIZE) $(ELF) > $(BUILD_DIR)/size-report.txt
	$(OBJDUMP) -h $(ELF) > $(BUILD_DIR)/section-report.txt
	$(NM) -S --size-sort $(ELF) > $(BUILD_DIR)/symbol-report.txt
	@echo "Reports generated in $(BUILD_DIR)"

flash-stlink: $(BIN)
	st-flash write $(BIN) 0x08000000

flash-openocd: $(ELF)
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "program $(ELF) verify reset exit"

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPENDENCIES)
