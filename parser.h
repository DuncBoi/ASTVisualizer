// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "tokenizer.h"

std::shared_ptr<ASTNode> parse(const std::vector<Token>& tokens);

#endif // PARSER_H
