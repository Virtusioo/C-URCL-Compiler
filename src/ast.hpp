
#pragma once

#include "lexer.hpp"

#include <variant>
#include <memory>
#include <optional>

struct DeclInfo
{
    size_t ptrs;
    std::string value;
};

struct VarDeclInfo
{
    std::string type;
    std::optional<DeclInfo> name;
};

enum class StructAttrType
{
    CLASS,
    UNION,
    DECL
};

using StructAttrValue = std::variant<VarDeclInfo, std::vector<VarDeclInfo>>;

struct StructAttr
{
    StructAttrType type;
    StructAttrValue value;
};

enum class NodeType
{
    FLOAT,
    INTEGER,
    STRING,
    INVALID,

    BINOP,
};

struct Node;
using NodePtr = std::unique_ptr<Node>;

struct NodeBinop
{
    NodePtr left;
    NodePtr right;
    TokenType op;
};

using NodeValue = std::variant<
    std::string,
    NodePtr,
    NodeBinop
>;

struct Node
{
    NodeType type;
    NodeValue value;
    size_t line;
    size_t col;
    size_t colEnd;

    Node(NodeType t, NodeValue&& v, size_t l, size_t c, size_t ce)
        : type(t), 
          value(std::move(v)), 
          line(l), 
          col(c),
          colEnd(ce) {}
};