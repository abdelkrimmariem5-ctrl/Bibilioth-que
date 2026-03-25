CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET   = main
SRCS     = main.cpp book.cpp library.cpp config.cpp menu.cpp
OBJS     = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
