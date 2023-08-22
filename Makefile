# MAKEFILE'S VARIABLES
OUTPUT			= seabow
SOURCES			= sources/*.cpp sources/core/*.cpp sources/nodes/*.cpp sources/values/*.cpp
SOURCES_TESTS	= sources/core/*.cpp sources/nodes/*.cpp sources/values/*.cpp
INCLUDES		= includes
COMMON_OPTS		= -O3 -Wall -std=c++20 -m64
ARGS			= # seabow args


# PROJECT COMPILATION
cmp-l:
	x86_64-linux-gnu-g++ $(SOURCES) -o build/linux/$(OUTPUT) -I$(INCLUDES) $(COMMON_OPTS)

cmp-w:
	x86_64-w64-mingw32-g++ $(SOURCES) -o build/windows/$(OUTPUT) -I$(INCLUDES) $(COMMON_OPTS) -static-libstdc++ -static-libgcc

cmp-d:
	x86_64-apple-darwin15-c++ $(SOURCES) -o build/darwin/$(OUTPUT) -I$(INCLUDES) -I/usr/local/osx-ndk-x86/SDK/MacOSX10.11.sdk/usr/include -L/usr/local/osx-ndk-x86/SDK/MacOSX10.11.sdk/usr/lib -I/usr/local/osx-ndk-x86/SDK/MacOSX10.11.sdk/usr/include/c++/4.2.1 $(COMMON_OPTS)


# PROJECT EXECUTION
run-l:
	./build/linux/$(OUTPUT) $(ARGS)

run-w:
	build/windows/$(OUTPUT).exe $(ARGS)

run-d:
	./build/darwin/$(OUTPUT) $(ARGS)


# PROJECT COMPILATION AND EXECUTION
load-l:
	$(MAKE) cmp-l run-l

load-w:
	$(MAKE) cmp-w run-w

load-d:
	$(MAKE) cmp-d run-d


# PROJECT TEST (LINUX)
test-l-lexer:
	x86_64-linux-gnu-g++ tests/test_lexer.cpp $(SOURCES_TESTS) -o tests/test_lexer.out -I$(INCLUDES) -Itests $(COMMON_OPTS)
	./tests/test_lexer.out

test-l-all:
	$(MAKE) test-l-lexer


# PROJECT TEST (WINDOWS)
test-w-lexer:
	x86_64-w64-mingw32-g++ tests/test_lexer.cpp $(SOURCES_TESTS) -o tests/test_lexer.exe -I$(INCLUDES) -Itests $(COMMON_OPTS)
	./tests/test_lexer.exe

test-w-all:
	$(MAKE) test-w-lexer


# PROJECT TEST (MACOS)