# Makefile for QR Code generator (C++)

# Compiler settings
CXX ?= g++
CXXFLAGS += -std=c++11 -O2 -Wall -Wextra `pkg-config --cflags libpng`
LDFLAGS += `pkg-config --libs libpng`

# Source files
SOURCES = qrcodegen-impl.cpp qrcodegen-main.cpp
HEADERS = qrcodegen.hpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = qrcodegen

# Clear default suffix rules
.SUFFIXES:

# Don't delete object files
.SECONDARY: $(OBJECTS)

# Default target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
