
#pragma once

#include <string>
#include <vector>

enum class TokenType: char
{
    IDENT,
    STRING,
    INT_LITERAL,
    FLOAT_LITERAL,
    CHAR_LITERAL,
    END_OF_FILE,
    INVALID,

    TYPE_NAME,

    UNION,
    CLASS,
    STATIC,

    STAR,
    SLASH,
    PLUS,
    MINUS,
    PERCENT,

    ELLIPSIS,
    ARROW,
    HASHTAG,
    INCREMENT,
    DECREMENT,
    DOT,
    COMMA,

    OPENPAREN,
    CLOSEPAREN,
    OPENBRACE,
    CLOSEBRACE,
    OPENBRACKET,
    CLOSEBRACKET,

    A_INCREMENT,
    A_DECREMENT,
    ASSIGN
};

struct Span
{
    size_t line;
    size_t col;
};

struct Token
{
    TokenType type;
    std::string value;
    Span span;
    size_t colEnd;
};

class Lexer
{
public:
    std::vector<Token> Tokenize(const char* source);

private:
    std::vector<Token> tokens;
    std::string buffer;
    const char* src;
    size_t pos;
    size_t col;
    Span span;

    // Source Methods
    char At() const { return src[pos]; }
    char Next(int offset) const { return src[pos + offset]; }
    char Eat();
    void Advance();

    // Lexer Methods
    void AppendEscape();
    void LexString();
    void LexChar();
    void LexIdent();
    void LexNumber();
    TokenType LexSymbol();

    void AppendChar() { buffer += Eat(); }

    bool Appendable(char c) {
        if (At() == c) {
            AppendChar();
            return true;
        }
        return false;
    }
    void PushToken(TokenType type)
    {
        tokens.emplace_back(Token{type, std::move(buffer), span, col});
    }
};