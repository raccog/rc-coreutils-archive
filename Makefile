CFLAGS ?= -g -Os
override INTERNAL_CFLAGS := -Wall -Wextra -std=c17

BUILD_DIR ?= build
override BIN_FILES := cat
override BINARY := $(patsubst %, $(BUILD_DIR)/%, $(BIN_FILES))

.PHONY: all
all: $(BINARY)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%: src/%.c
	mkdir -p $(@D)
	gcc $(CFLAGS) $(INTERNAL_CFLAGS) $< -o $@


