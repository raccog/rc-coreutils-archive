CFLAGS ?= -g -O3 -Wall -Wextra -std=c17

BUILD_DIR ?= build
BIN_FILES := cat
BINARY := $(patsubst %, $(BUILD_DIR)/%, $(BIN_FILES))

.PHONY: all
all: $(BINARY)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%: src/%.c
	mkdir -p $(@D)
	gcc $(CFLAGS) $< -o $@


