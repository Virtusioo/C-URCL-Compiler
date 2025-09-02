
#pragma once

#include "ast.hpp"

#include <unordered_set>

class Parser
{
public:
    std::vector<NodePtr> Parse(const std::vector<Token>& tokens);

private:
    std::vector<Token> tokens;
    std::vector<NodePtr> nodes;
    std::vector<Span> spanStack;
    std::unordered_set<std::string> types;
    size_t pos;

    // Parser Expr Methods
    NodePtr ParsePrimary();
    NodePtr ParseMult();
    NodePtr ParseAdd();

    // Parser Stmt Methods
    std::vector<VarDeclInfo> ParseParams();
    NodePtr ParseFuncDecl(const DeclInfo& decl);
    NodePtr ParseDecl();
    std::vector<VarDeclInfo> ParseStructBody();
    NodePtr ParseStruct();
    NodePtr ParseStatic();

    // Parser Root Methods
    NodePtr ParseExpr();
    NodePtr ParseStmt();

    // Token Methods
    void Advance() { pos += 1; }
    Token& At() { return tokens[pos]; }
    Token& Eat() 
    { 
        if (Type() == TokenType::END_OF_FILE)
            return At();
        return tokens[pos++]; 
    }
    Token& Expect(TokenType type, std::string&& message) 
    {
        Token& tk = Eat();
        if (tk.type != type) {
            PushNode(MakeNode(NodeType::INVALID, message + ", got '" + tk.value + '\''));
        }
        return tk;
    }
    std::string& ExpectValue(TokenType type, std::string&& message)
    {
        return Expect(type, std::move(message)).value;
    }
    TokenType Type() const { return tokens[pos].type; }
    bool NotEnd() const { return Type() != TokenType::END_OF_FILE; }
    void PushSpan() { spanStack.push_back(At().span); }
    size_t CountStars() 
    {
        size_t n = 0;
        while (Type() == TokenType::STAR) {
            Advance();
            n += 1;
        }
        return n;
    }

    // Node Methods
    NodePtr MakeNode(NodeType type, NodeValue&& value)
    {
        Span span = spanStack.back();
        Token& endToken = tokens[pos - 1];
        spanStack.pop_back();
        return std::make_unique<Node>(type, std::move(value), span.line, span.col, endToken.colEnd);
    }
    void PushNode(NodePtr&& node) { nodes.push_back(std::move(node)); }
};