# Determine the operating system
ifdef SystemRoot  # Check if SystemRoot environment variable exists (Windows)
	OS := Windows
else
	OS := $(shell uname)
endif

# Compiler
CXX = g++  # Change this to the compiler you are using (g++ for GCC, clang++ for Clang, etc.)

# Target
TARGET = solve

# Objects
OBJECTS = solve.o parser.o bottom-left.o globals.o heuristic-exec.o

# Stack size
STACK_SIZE_LINUX = 1073741824  # 1GB in bytes
STACK_SIZE_WINDOWS = 1073741824  # 1GB in bytes

# Binary
solve: $(OBJECTS)
ifeq ($(OS), Linux)
	$(CXX) -Wall -Wconversion -Wl,--stack,$(STACK_SIZE_LINUX) $^ -o $@
else ifeq ($(OS), Windows)
	$(CXX) -Wall -Wconversion $^ -o $@
endif

# Pattern rule for object-source dependencies
%.o: bottom-left-heuristic-master/%.cpp 
	$(CXX) -Wall -Wconversion -I. -c $<

# Automatic object-header dependencies
%.d: %.cpp
	$(CXX) -Wall -Wconversion -I. -MM -MT '$(<:.cpp=.o) $@' $< >$@

# Cleaning without removing the executable
clean:
ifeq ($(OS), Linux)
	rm -f *.o *~ \#* *.d solution.* *.tex adding_*.* NFP_*.*
else ifeq ($(OS), Windows)
	del *.o *~ \#* *.d solution.* *.tex adding_*.* NFP_*.*
endif
# Cleaning with removing the executable
cleanall: clean
ifeq ($(OS), Linux)
	rm -f $(TARGET)
else ifeq ($(OS), Windows)
	del $(TARGET).exe
endif