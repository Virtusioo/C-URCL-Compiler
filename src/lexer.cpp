
#include "lexer.hpp"

#include <unordered_map>

using TokenRecord = std::unordered_map<std::string, TokenType>;

// Token Record

static TokenRecord tokenRecord = {
    {"struct", TokenType::STRUCT},
    {"typedef", TokenType::TYPEDEF},
    {"float", TokenType::FLOAT},
    {"int", TokenType::INT},
    {"char", TokenType::CHAR},
    {"void", TokenType::VOID},
    {"long", TokenType::LONG},
    {"unsigned", TokenType::UNSIGNED}
};

static TokenType GetTokenType(const std::string& value)
{
    auto pair = tokenRecord.find(value);
    if (pair == tokenRecord.end()) 
        return TokenType::IDENT;
    return pair->second;
}

// Source Methods

void Lexer::Advance()
{
    if (src[pos] == '\n') {
        line += 1;
        col = 0;
    }
    col += 1;
    pos += 1;
}

char Lexer::Eat()
{
    char c = At();
    Advance();
    return c;
}

#define escape_char(ch, type) case ch: buffer += type; return

// This already expects the '\\' character
void Lexer::AppendEscape()
{
    Advance();
    switch (At()) {
        escape_char('n', '\n');
        escape_char('t', '\t');
        escape_char('r', '\r');
    }
    // Treat it as a character: '\x' -> 'x'
    // You get the character instead of an error (good enough)
    AppendChar();
}

void Lexer::LexString()
{
    Advance();

    while (At() != '"' && At() != '\0') {
        if (At() == '\\')
            AppendEscape();
        else
            AppendChar();
    }
    
    if (At() == '\0') {
        buffer = "(eof)";
        return PushToken(TokenType::INVALID);
    }

    Advance();
    PushToken(TokenType::STRING);
}

void Lexer::LexChar()
{
    Advance();
    if (At() == '\\')
        AppendEscape();
    else
        AppendChar();
    
    if (At() != '\'') 
        return PushToken(TokenType::INVALID);

    Advance();
    PushToken(TokenType::CHAR_LITERAL);
}

void Lexer::LexIdent()
{
    AppendChar();

    while (isalnum(At()) || At() == '_')
        AppendChar();

    TokenType type = GetTokenType(buffer);
    PushToken(type);
}

void Lexer::LexNumber()
{
    TokenType numType = TokenType::INT_LITERAL;

    while (isdigit(At()))
        AppendChar();

    if (At() == '.') {
        AppendChar();
        numType = TokenType::DOUBLE_LITERAL;
    }

    while (isdigit(At())) 
        AppendChar();
    
    if (buffer[0] == '.' && buffer.size() == 1) {
        buffer.clear();
        pos -= 1;
        return;
    }

    if (At() == 'f' || At() == 'F') {
        AppendChar();
        numType = TokenType::FLOAT_LITERAL;
    }

    PushToken(numType);
}

// Fastest and most predictable way to lex symbols (same approach as Lua's lexer)
TokenType Lexer::LexSymbol()
{
    AppendChar();

    switch (Next(-1)) {
        case '.': {
            if (At() == '.' && Next(1) == '.') {
                AppendChar();
                AppendChar();
                return TokenType::ELLIPSIS;
            }
            return TokenType::DOT;
        }
        case ',': {
            return TokenType::COMMA;
        }
        case '+': {
            if (Appendable('+')) {
                return TokenType::INCREMENT;
            } else if (Appendable('=')) {
                return TokenType::A_INCREMENT;
            }
            return TokenType::PLUS;
        }
        case '-': {
            if (Appendable('-')) {
                return TokenType::DECREMENT;
            } else if (Appendable('=')) {
                return TokenType::A_DECREMENT;
            } else if (Appendable('>')) {
                return TokenType::ARROW;
            }
            return TokenType::MINUS;
        } 
        case '/': return TokenType::SLASH;
        case '*': return TokenType::STAR;
        case '%': return TokenType::PERCENT;
        case '(': return TokenType::OPENPAREN;
        case ')': return TokenType::CLOSEPAREN;
        case '{': return TokenType::OPENBRACE;
        case '}': return TokenType::CLOSEBRACE;
        case '[': return TokenType::OPENBRACKET;
        case ']': return TokenType::CLOSEBRACKET;
    }

    return TokenType::INVALID;
}

std::vector<Token> Lexer::Tokenize(const char* source)
{
    src = source;
    pos = 0;
    line = 1;
    col = 1;
    lastCol = 1;

    while (At() != '\0') {

        // Capture the token's column.
        lastCol = col; 

        switch (At()) {
            case ' ':
            case '\r':
            case '\t':
            case '\n': {
                Advance();
            } break;

            case '\"': {
                LexString();
            } break;

            case '\'': {
                LexChar();
            } break;

            default: {
                if (isalpha(At()) || At() == '_') {
                    LexIdent();
                    break;
                } 
                if (isdigit(At()) || At() == '.') {
                    LexNumber();
                    break;
                }
                PushToken(LexSymbol());
            }
        }
    }

    buffer = "(eof)";
    PushToken(TokenType::END_OF_FILE);

    return std::move(tokens);
}