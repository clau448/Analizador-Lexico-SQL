CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic

.PHONY: all test clean

all: grammar_tests

grammar_tests: Grammar.cpp tests/grammar_tests.cpp ../Lexer.cpp
	$(CXX) $(CXXFLAGS) Grammar.cpp tests/grammar_tests.cpp ../Lexer.cpp -o $@

test: grammar_tests
	./grammar_tests

clean:
	rm -f grammar_tests parser_ll1_tests sql_parser
