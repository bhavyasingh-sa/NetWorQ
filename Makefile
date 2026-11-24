CXX := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
TARGET := networq
SOURCES := src/main.cpp

$(TARGET): $(SOURCES) include/*.h include/*.tpp
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
.PHONY: clean
