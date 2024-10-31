# Makefile

CC = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g3


SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

all: main

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/enums.o: $(INCLUDE_DIR)/enums.h $(SRC_DIR)/enums.cpp | $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -c $(SRC_DIR)/enums.cpp -o $(BUILD_DIR)/enums.o

$(BUILD_DIR)/lexer.o: $(INCLUDE_DIR)/lexer.h $(SRC_DIR)/lexer.cpp | $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -c $(SRC_DIR)/lexer.cpp -o $(BUILD_DIR)/lexer.o

$(BUILD_DIR)/parser.o: $(INCLUDE_DIR)/parser.h $(SRC_DIR)/parser.cpp | $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -c $(SRC_DIR)/parser.cpp -o $(BUILD_DIR)/parser.o

$(BUILD_DIR)/main.o: main.cpp | $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -c main.cpp -o $(BUILD_DIR)/main.o

main: $(BUILD_DIR)/enums.o $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o
	$(CC) $(CXXFLAGS) -o main $(BUILD_DIR)/enums.o $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/main.o

clean:
	rm -rf $(BUILD_DIR) main