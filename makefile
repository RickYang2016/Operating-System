# The name of binary
NAME = cpu_scheduling_simulator

# Flags passed to the preprocessor.
CPPFLAGS += -Wall -MMD -MP -Isrc -g -std=c++11

# All cpp files.
SRC = $(shell find src -name '*.cpp')
OBJ = $(SRC:src/%.cpp=bin/%.o)
DEP = $(SRC:src/%.cpp=bin/%.d)

# Default target, which builds your binary.
$(NAME): $(OBJ)
	$(CXX) $(CPP_FLAGS) $^ -o $(NAME)

# Debug target, build with debug options
debug: $(OBJ)
	$(CXX) $(CPP_FLAGS) $^ -o $(NAME) -g

# Build and run the program.
run: $(NAME)
	./$(NAME) input_file

# Remove all generated files.
clean:
	rm -rf $(NAME)* bin/

# Ensure the bin/ directories are created.
$(SRC): | bin

# Mirror the directory structure of src/ under bin/
bin:
	mkdir -p $(shell find src -type d | sed "s/src/bin/")

# Build objects.
bin/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $< -c -o $@

# Auto dependency management.
-include $(DEP)