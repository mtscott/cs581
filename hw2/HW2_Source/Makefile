CXX = mpicxx
CXXFLAGS = -O0 -Wall -std=c++17
TARGET = foo.ex
SRCS = main.cpp parallel_sort.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp parallel_sort.cpp parallel_sort.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean