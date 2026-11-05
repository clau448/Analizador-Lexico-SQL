CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic

.PHONY: all test clean

all: sql_parser

grammar_tests: Grammar.cpp tests/grammar_tests.cpp ../Lexer.cpp
	$(CXX) $(CXXFLAGS) Grammar.cpp tests/grammar_tests.cpp ../Lexer.cpp -o $@

sql_parser: Grammar.cpp Parser.cpp main.cpp ../Lexer.cpp
	$(CXX) $(CXXFLAGS) Grammar.cpp Parser.cpp main.cpp ../Lexer.cpp -o $@

parser_ll1_tests: Grammar.cpp Parser.cpp tests/parser_tests.cpp ../Lexer.cpp
	$(CXX) $(CXXFLAGS) Grammar.cpp Parser.cpp tests/parser_tests.cpp ../Lexer.cpp -o $@

validation_tests: Grammar.cpp Parser.cpp tests/validation_tests.cpp ../Lexer.cpp
	$(CXX) $(CXXFLAGS) Grammar.cpp Parser.cpp tests/validation_tests.cpp ../Lexer.cpp -o $@

test: grammar_tests parser_ll1_tests validation_tests
	./grammar_tests
	./parser_ll1_tests
	./validation_tests

clean:
	rm -f grammar_tests parser_ll1_tests validation_tests sql_parser
