// tokenizer.h
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

enum TokenType { TT_NUMBER, TT_OPERATOR, TT_FUNCTION, TT_PAREN_OPEN, TT_PAREN_CLOSE };

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& expression);

#endif // TOKENIZER_H
