TARGET_EXEC ?= jvm.out

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
HDR_DIR ?= ./include

CC := gcc

SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *.asm)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(HDR_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

all: $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.asm.o: %.asm
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: debug
.PHONY: clean

debug: CPPFLAGS += -g -O0
debug: $(BUILD_DIR)/$(TARGET_EXEC)

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
