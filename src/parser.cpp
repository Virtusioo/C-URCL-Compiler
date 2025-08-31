
#include "parser.hpp"

// Node Methods



// Parser Methods 

NodePtr Parser::ParsePrimary()
{
    return MakeNode(NodeType::INVALID, "unexpected primary");
}

NodePtr Parser::ParseAdd()
{
    NodePtr left = ParsePrimary();

    while (Type() == TokenType::PLUS || Type() == TokenType::MINUS) {
        TokenType op = Eat()->type;
        NodePtr right = ParsePrimary();

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
    return ParsePrimary();
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