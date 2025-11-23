CXX = clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -pedantic
INCLUDES = -Isrc -Itests $(GTEST_INC)
GTEST_DIR ?= /opt/homebrew
GTEST_INC ?= -I$(GTEST_DIR)/include
GTEST_LIB ?= -L$(GTEST_DIR)/lib -lgtest -lgtest_main -pthread

CORE_SRC = src/parser/order_parser.cpp \
           src/engine/order_book.cpp \
           src/engine/order_router.cpp \
           src/repl.cpp
APP_SRC = src/main.cpp $(CORE_SRC)
OUT = order_book
TEST_SRC = tests/test_order_parser.cpp \
           tests/test_order_book.cpp \
           tests/test_order_router.cpp \
           tests/test_main.cpp
TEST_OUT = order_book_tests

$(OUT): $(APP_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(APP_SRC) -o $(OUT)

$(TEST_OUT): $(TEST_SRC) $(CORE_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(CORE_SRC) $(TEST_SRC) $(GTEST_LIB) -o $(TEST_OUT)

test: $(TEST_OUT)
	./$(TEST_OUT) $(TEST_ARGS)

clean:
	rm -f $(OUT) $(TEST_OUT)

.PHONY: clean test
