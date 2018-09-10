cc = clang++
cflags = -std=c++14
debug_flags = -Wall -pedantic -Wextra -g
release_flags = -O2
lflags = -pthread -lpersonal

bin = ./bin
src = ./src
tmp = ./tmp
inc = ./src/include
test = ./test
doc = ./doc

files = main rules bot helper
components = $(patsubst %,$(tmp)/%.o,$(files))
components_test = $(patsubst %, $(test)/test_%.o,$(files))
components_debug = $(patsubst %.o, %_debug.o, $(components))
debug_target = $(bin)/main_debug.exe
release_target = $(bin)/main.exe

all: $(debug_target)

final: $(release_target)

$(debug_target): $(components_debug)
	$(cc) -o $@ $^ $(lflags)

$(release_target): $(components)
	$(cc) -o $@ $^ $(lflags)

$(tmp)/%_debug.o: $(src)/%.cpp $(inc)/%.hpp
	$(cc) $(debug_flags) $(cflags) -c -o $@ $<

$(tmp)/%.o: $(src)/%.cpp $(inc)/%.hpp
	$(cc) $(release_flags) $(cflags) -c -o $@ $<

# Testing
test: $(test)/test
	$<

$(test)/test : $(components_test) $(filter-out $(tmp)/main_debug.o, $(components_debug))
	$(cc) -o $@ $^ $(lflags)

$(test)/%.o : $(test)/%.cpp
	$(cc) $(release_flags) $(cflags) -c -o $@ $^

# Clean
clean:
	rm -f $(tmp)/*.o $(test)/*.o $(bin)/*

.phony: clean test
