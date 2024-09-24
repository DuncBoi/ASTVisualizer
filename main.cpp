// main.cpp
#include "tokenizer.h"
#include "parser.h"
#include "visualize.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // Check for expression argument
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " \"expression\"" << std::endl;
        return 1;
    }

    // Input expression
    std::string expression = argv[1];

    try {
        // Tokenize
        auto tokens = tokenize(expression);

        // Parse
        auto astRoot = parse(tokens);

        // Visualize
        visualizeAST(astRoot);

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
}
