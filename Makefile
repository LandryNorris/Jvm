TARGET_EXEC ?= jvm.out

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
HDR_DIR ?= ./include

CC := gcc

SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *.asm | grep -vE '*/executables/*')
DEPS := $(OBJS:.o=.d)

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(HDR_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

LDFLAGS := $(LDFLAGS) -lm

all: javaparser javarunner

javaparser: $(OBJS)
	$(eval SRCS := $(SRCS) $(shell find $(SRC_DIRS) -name $@.c -or -name $@.o))
	$(MKDIR_P) build/src/executables
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/executables/$@.c -o build/src/executables/$@.c.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/executables/$@.c -o build/src/executables/$@.c.o $(LDFLAGS)
	$(CC) $(OBJS) build/src/executables/$@.c.o -o build/$@ $(LDFLAGS) $(CPPFLAGS)

javarunner: $(OBJS)
	$(eval SRCS := $(SRCS) $(shell find $(SRC_DIRS) -name $@.c -or -name $@.o))
	$(MKDIR_P) build/src/executables
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/executables/$@.c -o build/src/executables/$@.c.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/executables/$@.c -o build/src/executables/$@.c.o $(LDFLAGS)
	$(CC) $(OBJS) build/src/executables/$@.c.o -o build/$@ $(LDFLAGS) $(CPPFLAGS)

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
debug: all

debugParser: CPPFLAGS += -g -O0
debugParser: javaparser

debugRunner: CPPFLAGS += -g -O0
debugRunner: javarunner

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
