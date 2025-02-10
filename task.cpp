#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>

// Token Types
enum class TokenType { IDENTIFIER, NUMBER, ASSIGN, OPERATOR, IF, WHILE, LPAR, RPAR, LBRACE, RBRACE, SEMICOLON, END };

struct Token {
    TokenType type;
    std::string value;
};

// Lexer: Converts SimpleLang code into tokens
class Lexer {
public:
    explicit Lexer(const std::string& src) : source(src), pos(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (pos < source.length()) {
            if (std::isspace(source[pos])) {
                pos++;
            } else if (std::isalpha(source[pos])) {
                std::string ident;
                while (std::isalnum(source[pos])) ident += source[pos++];
                if (ident == "if") tokens.push_back({TokenType::IF, ident});
                else if (ident == "while") tokens.push_back({TokenType::WHILE, ident});
                else tokens.push_back({TokenType::IDENTIFIER, ident});
            } else if (std::isdigit(source[pos])) {
                std::string num;
                while (std::isdigit(source[pos])) num += source[pos++];
                tokens.push_back({TokenType::NUMBER, num});
            } else {
                char ch = source[pos++];
                if (ch == '=') tokens.push_back({TokenType::ASSIGN, "="});
                else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') tokens.push_back({TokenType::OPERATOR, std::string(1, ch)});
                else if (ch == '(') tokens.push_back({TokenType::LPAR, "("});
                else if (ch == ')') tokens.push_back({TokenType::RPAR, ")"});
                else if (ch == '{') tokens.push_back({TokenType::LBRACE, "{"});
                else if (ch == '}') tokens.push_back({TokenType::RBRACE, "}"});
                else if (ch == ';') tokens.push_back({TokenType::SEMICOLON, ";"});
            }
        }
        tokens.push_back({TokenType::END, ""});
        return tokens;
    }
private:
    std::string source;
    size_t pos;
};

// Parser and AST Nodes
struct ASTNode {
    virtual ~ASTNode() = default;
};

struct NumberNode : public ASTNode {
    int value;
    explicit NumberNode(int val) : value(val) {}
};

struct VariableNode : public ASTNode {
    std::string name;
    explicit VariableNode(std::string n) : name(std::move(n)) {}
};

struct BinaryOpNode : public ASTNode {
    std::unique_ptr<ASTNode> left, right;
    std::string op;
    BinaryOpNode(std::unique_ptr<ASTNode> l, std::string o, std::unique_ptr<ASTNode> r)
        : left(std::move(l)), op(std::move(o)), right(std::move(r)) {}
};

struct AssignmentNode : public ASTNode {
    std::string name;
    std::unique_ptr<ASTNode> value;
    AssignmentNode(std::string n, std::unique_ptr<ASTNode> v)
        : name(std::move(n)), value(std::move(v)) {}
};

// Code Generator: Prints Assembly Code Instead of Writing to File
class CodeGenerator {
public:
    void generateAssignment(AssignmentNode* node) {
        std::cout << "MOV A, " << dynamic_cast<NumberNode*>(node->value.get())->value << std::endl;
        std::cout << "STORE " << node->name << std::endl;
    }

    void generate(std::vector<std::unique_ptr<ASTNode>>& statements) {
        for (auto& stmt : statements) {
            if (auto assign = dynamic_cast<AssignmentNode*>(stmt.get())) {
                generateAssignment(assign);
            }
        }
    }
};

// Main Compiler Program
int main() {
    std::string sourceCode = "x = 10;";
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();
    
    std::vector<std::unique_ptr<ASTNode>> ast;
    ast.push_back(std::make_unique<AssignmentNode>("x", std::make_unique<NumberNode>(10)));
    
    CodeGenerator generator;
    generator.generate(ast);
    
    return 0;
}