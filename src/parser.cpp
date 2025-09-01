
#include "parser.hpp"

// Node Methods



// Parser Methods 

NodePtr Parser::ParsePrimary()
{
    PushSpan();
    switch (Type()) {
        case TokenType::STRING:
            return MakeNode(NodeType::STRING, Eat().value);
        
    }
    return MakeNode(NodeType::INVALID, "unexpected primary");
}

NodePtr Parser::ParseMult()
{
    PushSpan();
    NodePtr left = ParsePrimary();

    while (
        Type() == TokenType::STAR || 
        Type() == TokenType::SLASH ||
        Type() == TokenType::PERCENT
    ) {
        TokenType op = Eat().type;
        NodePtr right = ParsePrimary();

        left = MakeNode(NodeType::BINOP, NodeBinop{
            std::move(left),
            std::move(right),
            op
        });
    }

    return left;
}

NodePtr Parser::ParseAdd()
{
    PushSpan();
    NodePtr left = ParseMult();

    while (Type() == TokenType::PLUS || Type() == TokenType::MINUS) {
        TokenType op = Eat().type;
        NodePtr right = ParseMult();

        left = MakeNode(NodeType::BINOP, NodeBinop{
            std::move(left),
            std::move(right),
            op
        });
    }
    return left;
}

// Parser Root Methods

NodePtr Parser::ParseExpr()
{
    return ParseAdd();
}

NodePtr Parser::ParseStmt()
{
    return ParseExpr();
}

std::vector<NodePtr> Parser::Parse(const std::vector<Token>& tokenz)
{
    tokens = tokenz;
    pos = 0;

    while (NotEnd()) 
        ParseStmt();

    return std::move(nodes);
}