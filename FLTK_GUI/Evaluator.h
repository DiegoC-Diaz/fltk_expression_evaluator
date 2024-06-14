#pragma once
#ifndef EVALUATOR_H
#define EVALUATOR_H


//#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <cctype>
#include <map>
#include <cmath>
#include <limits>
#include <vector>
#include <list>
#include <iostream>
#include "List.h"


class Evaluator {
public:
   
    Evaluator();
    std::map<std::string, double> variables;
    std::list<std::string> history;
    std::string expression;
    std::map<std::string, double>  constants;
    bool inputExpression(const char* ex, bool mode);
    double convertDouble(const std::string str);
    std::string displayHistory();

    void loadUserConstants(Node<Fl_Input*>* head);

private:

    int precedence(char op);



    float applyOp(float a, float b, char op);

    bool isOperator(const std::string& token);

    std::vector<std::string> tokenize(const std::string& s, const std::map<std::string, double>& constants, std::map<std::string, double>& variables);

    std::string infixToPostfix(const std::vector<std::string>& tokens);

    std::map<std::string, double> loadConstants(const std::string& filename);

    float evaluatePostfix(const std::string& postfix);

    bool isValidExpression(const std::vector<std::string>& tokens);

    bool hasConsecutiveOperators(const std::vector<std::string>& tokens);

    int run_val();
    ~Evaluator();

};

#endif // EVALUATOR_H

double Evaluator::convertDouble(const std::string str){
    //his Fucntion support sicentifc notation to double.
    if (str == "") {
        return 0;
    }
    double d = 0;
 
  
    try {
        std::istringstream os(str);
        os >> d;
        std::cout << "se puede realizar la conversion\n";
    }
    catch (int n) {
        std::cout << "NO se puede realizar la conversion\n";
    }
  

    return d;
}
Evaluator::Evaluator() {
    constants = loadConstants("constants.txt");
}
int Evaluator::precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;
}

float Evaluator::applyOp(float a, float b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': if (b == 0) throw std::runtime_error("Division by zero"); else return a / b;
    case '%': return int(a) % int(b);
    case '^': return std::pow(a, b);
    default: return 0;
    }
}

bool Evaluator::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";
}

std::vector<std::string>  Evaluator::tokenize(const std::string& s, const std::map<std::string, double>& constants, std::map<std::string, double>& variables) {
    std::vector<std::string> tokens;
    std::string current;
    for (char ch : s) {
        if (std::isspace(ch) || isOperator(std::string(1, ch)) || ch == '(' || ch == ')') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            if (!std::isspace(ch)) {
                tokens.push_back(std::string(1, ch));
            }
        }
        else {
            current += ch;
        }
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }

    // Convert constants and variables to their numeric values
    for (auto& token : tokens) {
        if (constants.find(token) != constants.end()) {
            token = std::to_string(constants.at(token));
        }
        else if (variables.find(token) != variables.end()) {
            token = std::to_string(variables.at(token));
        }
    }
    return tokens;
}

std::string  Evaluator::infixToPostfix(const std::vector<std::string>& tokens) {
    std::stack<std::string> stack;
    std::string postfix;
    std::cout << "\nConverting to Postfix:\n";
    for (const std::string& token : tokens) {
        if (std::isdigit(token[0]) || token.find('.') != std::string::npos || (token.size() > 1 && token[0] == '-')) {  // NÃºmero
            postfix += token + " ";
            std::cout << "Added to output: " << token << "\n";
        }
        else if (token == "(") {
            stack.push(token);
            std::cout << "Pushed to stack: " << token << "\n";
        }
        else if (token == ")") {
            while (!stack.empty() && stack.top() != "(") {
                postfix += stack.top() + " ";
                std::cout << "Popped from stack to output: " << stack.top() << "\n";
                stack.pop();
            }
            stack.pop();  // Remueve '('
            std::cout << "Popped from stack: " << "(\n";
        }
        else if (isOperator(token)) {
            while (!stack.empty() && stack.top() != "(" && precedence(stack.top()[0]) >= precedence(token[0])) {
                postfix += stack.top() + " ";
                std::cout << "Popped from stack to output: " << stack.top() << " (Precedence)\n";
                stack.pop();

            }
            stack.push(token);
            std::cout << "Pushed to stack: " << token << "\n";
        }
    }
    while (!stack.empty()) {
        postfix += stack.top() + " ";
        std::cout << "Popped from stack to output at end: " << stack.top() << "\n";
        stack.pop();
    }
    return postfix;
}

void Evaluator::loadUserConstants(Node<Fl_Input*>* head)
{
    //variables.clear();
    //Load cusotm user variables.
    Node<Fl_Input*>* curr = head;
    while (curr != nullptr) {
        constants[curr->token->value()] = convertDouble(curr->value->value());
        curr = curr->next;
    }


}
std::map<std::string, double>   Evaluator::loadConstants(const std::string& filename) {
    std::map<std::string, double> constants;
    std::ifstream file(filename);
    std::string line;

    if (!file) {
        std::cerr << "No se pudo abrir el archivo de constantes: " << filename << std::endl;
        return constants;
    }

    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        double value;
        if (getline(iss, key, '=') && iss >> value) {
            constants[key] = value;
        }
    }
    return constants;
}

float  Evaluator::evaluatePostfix(const std::string& postfix) {
    std::stack<float> stack;
    std::istringstream iss(postfix);
    std::string token;
    while (iss >> token) {
        if (isdigit(token[0]) || (token.size() > 1 && token[0] == '-') || token.find('.') != std::string::npos) {  // If the token is a number
            stack.push(std::stof(token));
        }
        else if (isOperator(token)) {
            float b = stack.top(); stack.pop();
            float a = stack.top(); stack.pop();
            stack.push(applyOp(a, b, token[0]));
        }
    }
    return stack.top();
}


bool  Evaluator::isValidExpression(const std::vector<std::string>& tokens) {
    int openParentheses = 0;

    for (const auto& token : tokens) {
        if (token == "(") {
            openParentheses++;
        }
        else if (token == ")") {
            if (openParentheses == 0) {
                throw std::runtime_error("Mismatched parentheses: Too many closing parentheses");
            }
            openParentheses--;
        }
    }

    if (openParentheses != 0) {
        throw std::runtime_error("Mismatched parentheses: Not all parentheses are closed");
    }

    return true;
}

bool  Evaluator::hasConsecutiveOperators(const std::vector<std::string>& tokens) {
    bool lastWasOperator = false;
    for (const auto& token : tokens) {
        if (isOperator(token)) {
            if (lastWasOperator) {
                return true;  // Encontró dos operadores consecutivos
            }
            lastWasOperator = true;
        }
        else {
            lastWasOperator = false;
        }
    }
    return false;
}

inline bool Evaluator::inputExpression(const char* exp,bool mode)

{   

    //voy a dividir el Input en dos partes una que evalue la funcion
    std::string expression= exp;
    if (mode) {
        size_t equals = expression.find('=');
        if (equals != std::string::npos) {
            std::string varName = expression.substr(0, equals);
            std::string valueExpr = expression.substr(equals + 1);

            try {
                auto tokens = tokenize(valueExpr, constants, variables);
                if (hasConsecutiveOperators(tokens)) {
                    throw std::runtime_error("Invalid sequence of operators");
                }
                if (!isValidExpression(tokens)) {
                    return false; // If expression is not valid, skip further processing
                }
                std::string postfix = infixToPostfix(tokens);
                float value = evaluatePostfix(postfix);
                variables[varName] = value;
                std::cout << "Variable " << varName << " set to " << value << std::endl;
                return false;
            }
            catch (const std::exception& e) {
                std::cerr << "Error setting variable: " << e.what() << std::endl;
                return false;
            }
        }
    }
   

    try {
        auto tokens = tokenize(expression, constants, variables);

   
        if (hasConsecutiveOperators(tokens)) {
            throw std::runtime_error("Invalid sequence of operators");
        }
        if (!isValidExpression(tokens)) {
            return false; // If expression is not valid, skip further processing
        }
        std::string postfix = infixToPostfix(tokens);
        float result = evaluatePostfix(postfix);
        std::cout << "Result: " << result << std::endl;
        std::cout << "Postfix Expression: " << postfix << std::endl;
        history.push_back(expression + " = " + std::to_string(result));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }


    return true;
}

inline std::string Evaluator::displayHistory()
{
    std::cout << "History:\n";
    std::string output="";
    for (const auto& h : history) {
        std::cout << h << std::endl;
        output += h+"\n";
    }
    return  output;

}





int   Evaluator::run_val() {
    auto constants = loadConstants("constants.txt");
    std::map<std::string, double> variables;
    std::list<std::string> history;
    std::string expression;

    while (true) {
        std::cout << "Enter an expression (or type 'exit' to quit): ";
        std::getline(std::cin, expression);

        if (expression == "exit" || expression == "Exit") {
            break;
        }

        size_t equals = expression.find('=');
        if (equals != std::string::npos) {
            std::string varName = expression.substr(0, equals);
            std::string valueExpr = expression.substr(equals + 1);

            try {
                auto tokens = tokenize(valueExpr, constants, variables);
                if (hasConsecutiveOperators(tokens)) {
                    throw std::runtime_error("Invalid sequence of operators");
                }
                if (!isValidExpression(tokens)) {
                    continue; // If expression is not valid, skip further processing
                }
                std::string postfix = infixToPostfix(tokens);
                float value = evaluatePostfix(postfix);
                variables[varName] = value;
                std::cout << "Variable " << varName << " set to " << value << std::endl;
                continue;
            }
            catch (const std::exception& e) {
                std::cerr << "Error setting variable: " << e.what() << std::endl;
                continue;
            }
        }

        try {
            auto tokens = tokenize(expression, constants, variables);
            if (hasConsecutiveOperators(tokens)) {
                throw std::runtime_error("Invalid sequence of operators");
            }
            if (!isValidExpression(tokens)) {
                continue; // If expression is not valid, skip further processing
            }
            std::string postfix = infixToPostfix(tokens);
            float result = evaluatePostfix(postfix);
            std::cout << "Result: " << result << std::endl;
            std::cout << "Postfix Expression: " << postfix << std::endl;
            history.push_back(expression + " = " + std::to_string(result));
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            continue;
        }

        std::cout << "History:\n";
        for (const auto& h : history) {
            std::cout << h << std::endl;
        }
    }

    return 0;
}

inline Evaluator::~Evaluator()
{
}

