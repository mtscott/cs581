CXX = CXX = /opt/homebrew/Cellar/gcc/14.2.0_1/bin/g++-14
CXXFLAGS = -fopenmp -O0
TARGET = main
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)