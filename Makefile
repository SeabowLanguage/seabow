# Project Variables
COMMAND :=	zig build-exe -fstrip src/main.zig -O ReleaseSafe -target
PROJECT :=	seabow
OUTPUT :=	-femit-bin=build
ARGS :=		# seabow command-line arguments


# Project Compilation
cmp-w:
	$(COMMAND) x86_64-windows $(OUTPUT)/windows/x64/$(PROJECT).exe
	$(COMMAND) aarch64-windows $(OUTPUT)/windows/arm64/$(PROJECT).exe

cmp-l:
	$(COMMAND) x86_64-linux $(OUTPUT)/linux/x64/$(PROJECT)
	$(COMMAND) aarch64-linux $(OUTPUT)/linux/arm64/$(PROJECT)

cmp-m:
	$(COMMAND) x86_64-macos $(OUTPUT)/macos/x64/$(PROJECT)
	$(COMMAND) aarch64-macos $(OUTPUT)/macos/arm64/$(PROJECT)

cmp-web:
	$(COMMAND) wasm64-wasi $(OUTPUT)/web/wasm64/$(PROJECT).wasm
	$(COMMAND) wasm32-wasi $(OUTPUT)/web/wasm32/$(PROJECT).wasm

cmp-all:
	$(MAKE) cmp-w cmp-l cmp-m cmp-web


# Project Execution
run-w:
	./build/windows/x64/$(PROJECT).exe $(ARGS)