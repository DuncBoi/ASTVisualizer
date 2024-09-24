# AST Visualizer for Math Expressions

This is a C++ program that takes in a math expression and outputs an Abstract Syntax Tree. It does so by decomposing the input into binary expressions via the Shunting Yard Algorithm. It then creates the syntax tree with these ordered, binary expressions. Lastly, the tree object is visualized using OpenGL.

## Usage

The program can be compiled like so:

`g++ main.cpp tokenizer.cpp parser.cpp visualize.cpp -std=c++11 -I/opt/homebrew/include -framework OpenGL -L/opt/homebrew/lib -lglfw -o ast_visualizer`

And then run via:

`./ast_visualizer "expression"`

As an example, you could do the following:

- `./ast_visualizer "sin(3 + 4 * 2) / (1 - 5)^2^3"`
- `./ast_visualizer "5 + 6 * 9 / -5 + 10"`
- `./ast_visualizer "0 * 10^18 - (4 * cos(23 / 4))"`

## Scope

The program will currently tolerate basic operations:

- Addition
- Subtraction
- Multiplication
- Division

The following trigonometric operations:

- Sine
- Cosine
- Tangent

As well as the following:

- Parentheses
- Exponentiation
- Unary operators denoting negative numbers

## Attributions

I borrowed `stb_easy_font` from Sean Barrett under the MIT license to illustrate the text.