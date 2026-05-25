# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -lm -Iinclude
DEBUG_FLAGS = -g -DDEBUG
TEST_FLAGS = -DTEST_MODE

# Target names
TARGET = hash_table_demo
TEST_TARGET = run_tests

# Source files
SOURCES = src/hash_table.c
DEMO_SOURCES = examples/demo.c
TEST_SOURCES = tests/test.c

# Build main program
$(TARGET): $(SOURCES) $(DEMO_SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(DEMO_SOURCES)

# Build test runner
$(TEST_TARGET): $(SOURCES) $(TEST_SOURCES)
	$(CC) $(CFLAGS) $(TEST_FLAGS) -o $(TEST_TARGET) $(SOURCES) $(TEST_SOURCES)

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Run tests with valgrind (memory check)
test-valgrind: $(TEST_TARGET)
	valgrind --leak-check=full ./$(TEST_TARGET)

# Run tests with debug output
test-debug: $(TEST_TARGET)
	./$(TEST_TARGET) --verbose

# Clean build files
clean:
	rm -f $(TARGET) $(TEST_TARGET)

# Run main program
run: $(TARGET)
	./$(TARGET)

# Clean and test
check: clean test

# Help target
help:
	@echo "Available commands:"
	@echo "  make          - Build main program"
	@echo "  make run      - Build and run main program"
	@echo "  make test     - Build and run tests"
	@echo "  make test-valgrind - Run tests with memory checking"
	@echo "  make test-debug - Run tests with verbose output"
	@echo "  make check    - Clean and run tests"
	@echo "  make clean    - Remove compiled files"
	@echo "  make help     - Show this help"

.PHONY: test test-valgrind test-debug clean run check help