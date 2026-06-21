CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread
SRC = src/server.cpp src/server_impl.cpp
TARGET = server

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
