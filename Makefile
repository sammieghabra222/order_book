CXX = clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -pedantic
INCLUDES = -Isrc

SRC = src/main.cpp src/parser/order_parser.cpp
OUT = order_book

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
