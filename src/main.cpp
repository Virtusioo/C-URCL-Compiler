
#include "lexer.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    Lexer lexer;
    auto tokens = lexer.Tokenize("int main");

    for (Token& t: tokens) {
        std::cout << (int)t.type << ' ' << t.value << '\n';
    }
}