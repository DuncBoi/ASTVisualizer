// tokenizer.cpp
#include "tokenizer.h"
#include <cctype>
#include <sstream>
#include <stdexcept>

std::vector<Token> tokenize(const std::string& expression) {
    std::vector<Token> tokens;
    size_t i = 0;
    while (i < expression.size()) {
        if (std::isspace(expression[i])) {
            i++;
            continue;
        }
        if (std::isdigit(expression[i]) || expression[i] == '.') {
            std::ostringstream number;
            while (i < expression.size() && (std::isdigit(expression[i]) || expression[i] == '.')) {
                number << expression[i++];
            }
            Token token;
            token.type = TT_NUMBER; token.value = number.str();
            tokens.push_back(token);
        } else if (std::isalpha(expression[i])) {
            std::ostringstream func;
            while (i < expression.size() && std::isalpha(expression[i])) {
                func << expression[i++];
            }
            Token token;
            token.type = TT_FUNCTION; token.value = func.str();
            tokens.push_back(token);
        } else if (expression[i] == '+' || expression[i] == '-' ||
                   expression[i] == '*' || expression[i] == '/' || expression[i] == '^') {
            Token token;
            token.type = TT_OPERATOR; token.value = expression[i++];
            tokens.push_back(token);
        } else if (expression[i] == '(') {
            Token token;
            token.type = TT_PAREN_OPEN; token.value = "(";
            tokens.push_back(token);
            i++;
        } else if (expression[i] == ')') {
            Token token;
            token.type = TT_PAREN_CLOSE; token.value = ")";
            tokens.push_back(token);
            i++;
        } else {
            // Handle invalid characters
            throw std::runtime_error("Invalid character in expression.");
        }
    }
    return tokens;
}
