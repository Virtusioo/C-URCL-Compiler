
#include "parser.hpp"

// Node Methods



// Parser Expr Methods 

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

// Parser Stmt Methods

std::vector<VarDeclInfo> Parser::ParseParams()
{
    Advance();
    std::string& type = ExpectValue(TokenType::TYPE_NAME, "expected typename");
}

NodePtr Parser::ParseFuncDecl(const DeclInfo& decl)
{

}

NodePtr Parser::ParseDecl()
{
    std::string& type = Eat().value;
    DeclInfo decl;
    decl.ptrs = CountStars();
    decl.value = ExpectValue(TokenType::IDENT, "expected identifier next to typename");

    if (Type() == TokenType::OPENPAREN)
        return ParseFuncDecl(decl);
    

}

std::vector<VarDeclInfo> Parser::ParseStructBody()
{
    Advance();
    std::vector<StructAttr> attrs;
}

NodePtr Parser::ParseStruct()
{
    bool isClass = Eat().type == TokenType::CLASS;
    std::string& type = ExpectValue(TokenType::IDENT, "expected identifier as custom type");
}

NodePtr Parser::ParseStatic()
{
    Advance();
}

// Parser Root Methods

NodePtr Parser::ParseExpr()
{
    return ParseAdd();
}

NodePtr Parser::ParseStmt()
{
    switch (Type()) {
        case TokenType::TYPE_NAME:
            return ParseDecl();
        case TokenType::STATIC:
            return ParseStatic();
        case TokenType::CLASS:
        case TokenType::UNION:
            return ParseStruct();
    }

    return ParseExpr();
}

std::vector<NodePtr> Parser::Parse(const std::vector<Token>& tokenz)
{
    tokens = tokenz;
    pos = 0;

    while (NotEnd()) 
        PushNode(ParseStmt());

    return std::move(nodes);
}