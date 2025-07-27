BUILD_DIR := build/
BUILD_FLAGS := 
FORMAT_STYLE := Microsoft

SOURCE_FILES := main.c $(wildcard src/*.c) $(wildcard src/*.h)

.PHONY: all compile generate clean format

all: clean generate

run: compile
	@$(BUILD_DIR)/stupidserver

compile: generate
	@echo "--- Compiling Sources ---"
	@cmake --build $(BUILD_DIR) $(BUILD_FLAGS)

generate:
	mkdir -p $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR)
	@cp $(BUILD_DIR)/compile_commands.json ./compile_commands.json

format:
	clang-format -i -style=$(FORMAT_STYLE) $(ALL_SOURCE_FILES)

clean:
	@rm -rf $(BUILD_DIR)
