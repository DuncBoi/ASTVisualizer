// visualize.cpp
#include "visualize.h"
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>
#include <map>

// Include stb_easy_font
#include "stb_easy_font.h"

// Struct for node positions
struct NodePosition {
    double x;
    double y;
};

// Function to calculate positions of nodes
void calculatePositions(const std::shared_ptr<ASTNode>& node, double x, double y, double offset, std::map<const ASTNode*, NodePosition>& positions) {
    if (!node) return;
    NodePosition initPos;
    initPos.x = x; initPos.y = y;
    positions[node.get()] = initPos;

    if (node->getType() == OPERATOR) {
        auto opNode = std::static_pointer_cast<OperatorNode>(node);
        calculatePositions(opNode->left, x - offset, y - 150.0, offset / 1.5, positions);
        calculatePositions(opNode->right, x + offset, y - 150.0, offset / 1.5, positions);
    } else if (node->getType() == FUNCTION) {
        auto funcNode = std::static_pointer_cast<FunctionNode>(node);
        calculatePositions(funcNode->argument, x, y - 150.0, offset / 1.5, positions);
    }
}

// Function to get label for a node
std::string labelForNode(const std::shared_ptr<ASTNode>& node) {
    if (node->getType() == NUMBER) {
        auto numNode = std::static_pointer_cast<NumberNode>(node);
        
        // Use ostringstream to control the number format
        std::ostringstream out;
        if (std::floor(numNode->value) == numNode->value) {
            // If the number is an integer, print without decimal places
            out << static_cast<int>(numNode->value);
        } else {
            // Otherwise, print with limited precision (if necessary)
            out << numNode->value;
        }
        return out.str();
    } else if (node->getType() == OPERATOR) {
        auto opNode = std::static_pointer_cast<OperatorNode>(node);
        return std::string(1, opNode->op);
    } else if (node->getType() == FUNCTION) {
        auto funcNode = std::static_pointer_cast<FunctionNode>(node);
        return funcNode->funcName;
    }
    return "";
}

// Function to draw text at a position
void drawNodeText(float x, float y, const char* text) {
    char buffer[99999]; // ~500 chars
    int num_quads;

    // Save the current transformation matrix
    glPushMatrix();

    // Translate to the text position
    glTranslatef(x, y, 0.0f);

    // Flip the y-axis for text only
    glScalef(1.0f, -1.0f, 1.0f);
    
    glScalef(2.0f, 2.0f, 1.0f);

    // Move the text back to the correct y position
    glTranslatef(0, -7.0f, 0.0f);  // Adjust to account for text height

    // Render the text (as before)
    num_quads = stb_easy_font_print(-(float)(strlen(text) * 3.0f), 0, (char*)text, NULL, buffer, sizeof(buffer));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glColor3f(0.0f, 0.0f, 0.5f); // Dark blue
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Restore the transformation matrix
    glPopMatrix();
}


// Function to draw a filled circle
void drawFilledCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Center of circle
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments); // Get the current angle
        float x = r * cosf(theta); // Calculate the x component
        float y = r * sinf(theta); // Calculate the y component
        glVertex2f(x + cx, y + cy); // Output vertex
    }
    glEnd();
}

// Function to draw a line between two points
void drawLine(double x1, double y1, double x2, double y2) {
    glColor3f(1.0f, 1.0f, 1.0f);  // Set the line color to white
    glLineWidth(3.0f);             // Set the line width to 3.0 for thicker edges
    glBegin(GL_LINES);
        glVertex2f((float)x1, (float)y1);
        glVertex2f((float)x2, (float)y2);
    glEnd();
    glLineWidth(1.0f);             // Reset line width to default
}


// Function to draw the AST recursively
void drawAST(const std::shared_ptr<ASTNode>& node, const std::map<const ASTNode*, NodePosition>& positions) {
    if (!node) return;

    auto pos = positions.at(node.get());

    // Draw lines to children first
    glColor3f(1.0f, 1.0f, 1.0f); // White color for lines
    if (node->getType() == OPERATOR) {
        auto opNode = std::static_pointer_cast<OperatorNode>(node);

        if (opNode->left) {
            auto leftPos = positions.at(opNode->left.get());
            drawLine(pos.x, pos.y, leftPos.x, leftPos.y);
            drawAST(opNode->left, positions);
        }
        if (opNode->right) {
            auto rightPos = positions.at(opNode->right.get());
            drawLine(pos.x, pos.y, rightPos.x, rightPos.y);
            drawAST(opNode->right, positions);
        }
    } else if (node->getType() == FUNCTION) {
        auto funcNode = std::static_pointer_cast<FunctionNode>(node);

        if (funcNode->argument) {
            auto argPos = positions.at(funcNode->argument.get());
            drawLine(pos.x, pos.y, argPos.x, argPos.y);
            drawAST(funcNode->argument, positions);
        }
    }

    // Draw filled circle for the node
    glColor3f(1.0f, 1.0f, 1.0f); // White color for circle fill
    drawFilledCircle((float)pos.x, (float)pos.y, 30.0f, 32);

    // Draw circle outline
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for circle outline
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 32; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(32);
        float x = 30.0f * cosf(theta);
        float y = 30.0f * sinf(theta);
        glVertex2f(x + pos.x, y + pos.y);
    }
    glEnd();
    glLineWidth(1.0f); // Reset line width

    // Draw the label inside the circle
    drawNodeText((float)pos.x, (float)pos.y, labelForNode(node).c_str());
}

// Function to visualize the AST
void visualizeAST(const std::shared_ptr<ASTNode>& node) {
    if (!node) return;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return;
    }

    // Request an OpenGL 2.1 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Calculate node positions
    std::map<const ASTNode*, NodePosition> positions;
    calculatePositions(node, 400.0, 550.0, 200.0, positions); // Start from the top center

    // Find the bounding box of the tree
    double minX = 400.0, maxX = 400.0, minY = 550.0, maxY = 550.0;
    for (const auto& entry : positions) {
        minX = std::min(minX, entry.second.x);
        maxX = std::max(maxX, entry.second.x);
        minY = std::min(minY, entry.second.y);
        maxY = std::max(maxY, entry.second.y);
    }

    // Add some padding to the bounding box
    double padding = 50.0;
    minX -= padding;
    maxX += padding;
    minY -= padding;
    maxY += padding;

    // Calculate window size
    int windowWidth = static_cast<int>(maxX - minX);
    int windowHeight = static_cast<int>(maxY - minY);

    // Create window with calculated size
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "AST Visualization", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window." << std::endl;
        return;
    }

    glfwMakeContextCurrent(window);

    // Set up coordinate system
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set up orthographic projection matching the calculated bounding box
    glOrtho(minX, maxX, minY, maxY, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();

        // Draw AST
        drawAST(node, positions);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

