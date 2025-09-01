
#pragma once

#include "ast.hpp"

class Parser
{
public:
    std::vector<NodePtr> Parse(const std::vector<Token>& tokens);

private:
    std::vector<Token> tokens;
    std::vector<NodePtr> nodes;
    std::vector<Span> spanStack;
    size_t pos;

    // Parser Methods
    NodePtr ParsePrimary();
    NodePtr ParseMult();
    NodePtr ParseAdd();

    // Parser Root Methods
    NodePtr ParseExpr();
    NodePtr ParseStmt();

    // Token Methods
    void Advance() { pos += 1; }
    Token* At() { return &tokens[pos]; }
    Token* Eat() { return &tokens[pos++]; }
    TokenType Type() const { return tokens[pos].type; }
    bool NotEnd() const { return Type() != TokenType::END_OF_FILE; }
    void PushSpan() { spanStack.push_back(At()->span); }

    // Node Methods
    NodePtr MakeNode(NodeType type, NodeValue&& value)
    {
        Span& span = spanStack.back();
        Token* endToken = At();
        spanStack.pop_back();
        return std::make_unique<Node>(type, std::move(value), span.line, span.col, endToken->colEnd);
    }
};