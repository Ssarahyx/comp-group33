CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = hku_gpa_escape
SOURCES = main.cpp game.cpp map.cpp question.cpp save.cpp entity.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) hku_gpa_escape_save.txt

.PHONY: clean
