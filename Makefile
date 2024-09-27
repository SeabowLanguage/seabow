# Project Variables
COMMAND :=	zig build-exe -fstrip src/main.zig -O ReleaseSafe -target
PROJECT :=	seabow
BUILD :=	build
OUTPUT :=	-femit-bin=$(BUILD)
ARGS :=		# seabow command-line arguments

ifeq ($(OS),Windows_NT)
	RM = del /Q
	FIX_PATH = $(subst /,\,$1)
else
	RM = rm -f
	FIX_PATH = $1
endif

# Project Compilation
cmp-w:
	$(COMMAND) x86_64-windows $(OUTPUT)/windows/x64/$(PROJECT).exe
	$(RM) $(call FIX_PATH, "$(BUILD)/windows/x64/$(PROJECT).exe.obj")

	$(COMMAND) aarch64-windows $(OUTPUT)/windows/arm64/$(PROJECT).exe
	$(RM) $(call FIX_PATH, "$(BUILD)/windows/arm64/$(PROJECT).exe.obj")

cmp-l:
	$(COMMAND) x86_64-linux $(OUTPUT)/linux/x64/$(PROJECT)
	$(RM) $(call FIX_PATH, "$(BUILD)/linux/x64/$(PROJECT).o")

	$(COMMAND) aarch64-linux $(OUTPUT)/linux/arm64/$(PROJECT)
	$(RM) $(call FIX_PATH, "$(BUILD)/linux/arm64/$(PROJECT).o")

cmp-m:
	$(COMMAND) x86_64-macos $(OUTPUT)/macos/x64/$(PROJECT)
	$(RM) $(call FIX_PATH, "$(BUILD)/macos/x64/$(PROJECT).o")
	
	$(COMMAND) aarch64-macos $(OUTPUT)/macos/arm64/$(PROJECT)
	$(RM) $(call FIX_PATH, "$(BUILD)/macos/arm64/$(PROJECT).o")

cmp-web:
	$(COMMAND) wasm64-wasi $(OUTPUT)/web/wasm64/$(PROJECT).wasm
	$(RM) $(call FIX_PATH, "$(BUILD)/web/wasm64/$(PROJECT).wasm.o")

	$(COMMAND) wasm32-wasi $(OUTPUT)/web/wasm32/$(PROJECT).wasm
	$(RM) $(call FIX_PATH, "$(BUILD)/web/wasm32/$(PROJECT).wasm.o")

cmp-all:
	$(MAKE) cmp-w cmp-l cmp-m cmp-web


# Project Execution
run-w:
	./build/windows/x64/$(PROJECT).exe $(ARGS)