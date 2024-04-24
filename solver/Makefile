# Determine the operating system
ifdef SystemRoot  # Check if SystemRoot environment variable exists (Windows)
	OS := Windows
else ifeq ($(shell test -d /usr/bin && echo true),true) # Check for MSYS environment
	OS := MSYS
else ifeq ($(shell test -d /mingw64/bin && echo true),true) # Check for MinGW environment (64-bit)
	OS := MinGW
else ifeq ($(shell test -d /mingw32/bin && echo true),true) # Check for MinGW environment (32-bit)
	OS := MinGW
else
	OS := $(shell uname)
endif



# Determine the operating system for RM
ifdef SystemRoot  # Check if SystemRoot environment variable exists (Windows)
	OSRM := Windows
else
	OSRM := $(shell uname)
endif

$(info OS is $(OS)) # Debug output to print the value of OS
$(info OS is $(OSRM)) # Debug output to print the value of OS

# Define deletion command based on the operating system
ifeq ($(OSRM), Windows)
	RM = del /Q
else
	RM = rm -f
endif

# Compiler
CXX = g++  # Change this to the compiler you are using (g++ for GCC, clang++ for Clang, etc.)

# Target
TARGET = solve

# Objects
OBJECTS = solve.o parser.o bottom-left.o globals.o heuristic-exec.o

# Stack size
STACK_SIZE = 1073741824 # 1GB in bytes

# Binary
solve: $(OBJECTS)
ifeq ($(OS), Windows)
	$(CXX) -Wall -Wconversion $^ -o $@ -lstdc++
else ifeq ($(OS), MinGW)
	$(CXX) -Wall -Wconversion $^ -o $@ -lstdc++
else ifeq ($(OS), MSYS)
	$(CXX) -Wall -Wconversion $^ -o $@ -lstdc++
else
	$(CXX) -Wall -Wconversion -Wl,--stack=$(STACK_SIZE) $^ -o $@ -lstdc++
endif


# Pattern rule for object-source dependencies
%.o: bottom-left-heuristic-master/%.cpp 
	$(CXX) -Wall -Wconversion -I. -c $<

# Automatic object-header dependencies
%.d: %.cpp
	$(CXX) -Wall -Wconversion -I. -MM -MT '$(<:.cpp=.o) $@' $< >$@

# Cleaning without removing the executable
clean:
	$(RM) *.o *~ \#* *.d solution.* *.tex adding_*.* NFP_*.*

# Cleaning with removing the executable
cleanall: clean
	$(RM) $(TARGET) $(TARGET).exe
