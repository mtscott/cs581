CXX = /opt/homebrew/Cellar/gcc/14.2.0_1/bin/g++-14
CXXFLAGS = -std=c++17 -fopenmp -pthread -O0
INCLUDES = -I.

SRCS = main.cpp load_model.cpp model_pthreads.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = foo.ex

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 