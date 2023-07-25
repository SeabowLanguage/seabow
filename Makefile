# MAKEFILE'S VARIABLES
OUTPUT			= seabow
SOURCES			= sources/*.cpp sources/core/*.cpp
SOURCES_TEST	= sources/core/*.cpp tests/*.cpp
INCLUDES		= includes
COMMON_OPTS		= -O3 -Wall -fuse-ld=lld -std=c++17
ARGS			= # seabow args


# PROJECT COMPILATION
cmp-l:
	x86_64-linux-gnu-g++ $(SOURCES) -o build/linux/$(OUTPUT) -I$(INCLUDES) $(COMMON_OPTS) -pthread

cmp-w:
	x86_64-w64-mingw32-g++-win32 $(SOURCES) -o build/windows/$(OUTPUT) -I$(INCLUDES) $(COMMON_OPTS) -static-libstdc++ -static-libgcc

cmp-d:
	x86_64-apple-darwin15-c++ $(SOURCES) -o build/darwin/$(OUTPUT) -I$(INCLUDES) -I/usr/local/osx-ndk-x86/SDK/MacOSX10.11.sdk/usr/include -L/usr/local/osx-ndk-x86/SDK/MacOSX10.11.sdk/usr/lib -I/usr/local/osx-ndk-x86/SDK/MacOSX10.11.sdk/usr/include/c++/4.2.1 $(COMMON_OPTS)

cmp-all:
	$(MAKE) cmp-l cmp-w cmp-d


# PROJECT EXECUTION
run-l:
	./build/linux/$(OUTPUT) $(ARGS)

run-w:
	wine build/windows/$(OUTPUT).exe $(ARGS)

run-d:
	./build/darwin/$(OUTPUT) $(ARGS)

run-all:
	$(MAKE) run-l run-w run-d


# PROJECT COMPILATION AND EXECUTION
load-l:
	$(MAKE) cmp-l run-l

load-w:
	$(MAKE) cmp-w run-w

load-d:
	$(MAKE) cmp-d run-d

load-all:
	$(MAKE) load-l load-w load-d


# PROJECT TEST (LINUX)
test-l-lexer:
	clang++ --target=x86_64-linux-gnu $(SOURCES_TEST) -o tests/test_lexer.out -L/usr/local/lib/ -lgtest -lgtest_main -pthread -I$(INCLUDES) $(COMMON_OPTS)
	./tests/test_lexer.out

test-l-all:
	$(MAKE) test-l-lexer


# PROJECT TEST (WINDOWS)