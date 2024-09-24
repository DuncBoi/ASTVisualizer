// ast.h
#ifndef AST_H
#define AST_H

#include <string>
#include <memory>

enum NodeType { NUMBER, OPERATOR, FUNCTION };

class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual NodeType getType() const = 0;
};

class NumberNode : public ASTNode {
public:
    double value;
    NumberNode(double val) : value(val) {}
    NodeType getType() const override { return NUMBER; }
};

class OperatorNode : public ASTNode {
public:
    char op;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;

    OperatorNode(char oper) : op(oper) {}
    NodeType getType() const override { return OPERATOR; }
};

class FunctionNode : public ASTNode {
public:
    std::string funcName;
    std::shared_ptr<ASTNode> argument;

    FunctionNode(const std::string& name) : funcName(name) {}
    NodeType getType() const override { return FUNCTION; }
};

#endif // AST_H
