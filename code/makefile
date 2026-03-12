CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -march=native
TARGETDIR = target
SRCDIR = src
INCLUDEDIR = include

.PHONY: all clean run run_tests test_lexer

all:
	@echo "Usage: make <target_name> to compile src/<target_name>.cpp"
	@echo "       make run_tests to run the lexer tests"

# Generic rule for compiling any .cpp file in src/
%: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGETDIR)/$@ $<

clean:
	rm -f $(TARGETDIR)/*

run: 
	@echo "Usage: make <target_name> to compile and run src/<target_name>.cpp"