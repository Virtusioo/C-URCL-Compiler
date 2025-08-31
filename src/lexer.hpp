
#pragma once

#include <string>
#include <vector>

enum class TokenType
{
    IDENT,
    STRING,
    INT_LITERAL,
    FLOAT_LITERAL,
    DOUBLE_LITERAL,
    CHAR_LITERAL,
    END_OF_FILE,
    INVALID,

    UNSIGNED,
    INT,
    FLOAT,
    CHAR,
    LONG,
    DOUBLE,
    VOID,

    STRUCT,
    UNION,
    TYPEDEF,

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

struct Token
{
    TokenType type;
    std::string value;
    size_t line, col;
};

class Lexer
{
public:
    std::vector<Token> Tokenize(const char* source);

private:
    std::vector<Token> tokens;
    const char* src;
    std::string buffer;
    size_t pos;
    size_t line;
    size_t lastCol;
    size_t col;

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
    std::string&& GetValue() { return std::move(buffer); }

    bool Appendable(char c) {
        if (At() == c) {
            AppendChar();
            return true;
        }
        return false;
    }
    void PushToken(TokenType type)
    {
        tokens.emplace_back(Token{type, std::move(buffer), line, lastCol});
    }
};