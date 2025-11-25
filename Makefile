# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = 

# Source files
SRCS = main.cpp game.cpp entity.cpp map.cpp question.cpp save.cpp
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = hku_gpa_escape

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Object file dependencies
main.o: main.cpp game.h
	$(CXX) $(CXXFLAGS) -c main.cpp

game.o: game.cpp game.h map.h question.h save.h entity.h
	$(CXX) $(CXXFLAGS) -c game.cpp

entity.o: entity.cpp entity.h save.h
	$(CXX) $(CXXFLAGS) -c entity.cpp

map.o: map.cpp map.h entity.h
	$(CXX) $(CXXFLAGS) -c map.cpp

question.o: question.cpp question.h
	$(CXX) $(CXXFLAGS) -c question.cpp

save.o: save.cpp save.h
	$(CXX) $(CXXFLAGS) -c save.cpp

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
