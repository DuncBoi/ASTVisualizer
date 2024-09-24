// parser.cpp
#include "parser.h"
#include <stack>
#include <map>
#include <stdexcept>

int getPrecedence(const std::string& op) {
    if (op == "sin" || op == "cos" || op == "tan")
        return 4;
    if (op == "^")
        return 3;
    if (op == "*" || op == "/")
        return 2;
    if (op == "+" || op == "-")
        return 1;
    return 0;
}

bool isRightAssociative(const std::string& op) {
    return op == "^";
}

std::shared_ptr<ASTNode> parse(const std::vector<Token>& tokens) {
    std::stack<std::shared_ptr<ASTNode> > output;
    std::stack<Token> operators;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        if (token.type == TT_NUMBER) {
            output.push(std::make_shared<NumberNode>(std::stod(token.value)));
        } else if (token.type == TT_FUNCTION) {
            operators.push(token);
        } else if (token.type == TT_OPERATOR) {
            while (!operators.empty() && operators.top().type != TT_PAREN_OPEN &&
                   ((getPrecedence(operators.top().value) > getPrecedence(token.value)) ||
                    (getPrecedence(operators.top().value) == getPrecedence(token.value) &&
                     !isRightAssociative(token.value)))) {
                Token opToken = operators.top();
                operators.pop();

                if (opToken.type == TT_OPERATOR) {
                    auto right = output.top(); output.pop();
                    auto left = output.top(); output.pop();
                    auto opNode = std::make_shared<OperatorNode>(opToken.value[0]);
                    opNode->left = left;
                    opNode->right = right;
                    output.push(opNode);
                } else if (opToken.type == TT_FUNCTION) {
                    auto arg = output.top(); output.pop();
                    auto funcNode = std::make_shared<FunctionNode>(opToken.value);
                    funcNode->argument = arg;
                    output.push(funcNode);
                }
            }
            operators.push(token);
        } else if (token.type == TT_PAREN_OPEN) {
            operators.push(token);
        } else if (token.type == TT_PAREN_CLOSE) {
            while (!operators.empty() && operators.top().type != TT_PAREN_OPEN) {
                Token opToken = operators.top();
                operators.pop();

                if (opToken.type == TT_OPERATOR) {
                    auto right = output.top(); output.pop();
                    auto left = output.top(); output.pop();
                    auto opNode = std::make_shared<OperatorNode>(opToken.value[0]);
                    opNode->left = left;
                    opNode->right = right;
                    output.push(opNode);
                } else if (opToken.type == TT_FUNCTION) {
                    auto arg = output.top(); output.pop();
                    auto funcNode = std::make_shared<FunctionNode>(opToken.value);
                    funcNode->argument = arg;
                    output.push(funcNode);
                }
            }
            if (!operators.empty() && operators.top().type == TT_PAREN_OPEN) {
                operators.pop();
            } else {
                throw std::runtime_error("Mismatched parentheses.");
            }
        }
    }

    while (!operators.empty()) {
        Token opToken = operators.top();
        operators.pop();

        if (opToken.type == TT_OPERATOR) {
            auto right = output.top(); output.pop();
            auto left = output.top(); output.pop();
            auto opNode = std::make_shared<OperatorNode>(opToken.value[0]);
            opNode->left = left;
            opNode->right = right;
            output.push(opNode);
        } else if (opToken.type == TT_FUNCTION) {
            auto arg = output.top(); output.pop();
            auto funcNode = std::make_shared<FunctionNode>(opToken.value);
            funcNode->argument = arg;
            output.push(funcNode);
        }
    }

    if (output.size() != 1) {
        throw std::runtime_error("Invalid expression.");
    }

    return output.top();
}
