#include "evaluator.h"

Evaluator::Evaluator() {
    loadUserConstants("constants.txt");
}

void Evaluator::loadUserConstants(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file) {
        std::cerr << "No se pudo abrir el archivo de constantes: " << filename << std::endl;
        return;
    }

    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        double value;
        if (getline(iss, key, '=') && iss >> value) {
            constants[key] = value;
        }
    }
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
    case '%': if (b == 0) throw std::runtime_error("Modulo by zero"); else return int(a) % int(b);
    case '^': return std::pow(a, b);
    default: return 0;
    }
}

bool Evaluator::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";
}

bool Evaluator::isNumber(const std::string& s) {
    char* end = nullptr;
    std::strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0';
}

bool Evaluator::isValidToken(const std::string& token, const std::map<std::string, double>& constants, const std::map<std::string, double>& variables) {
    return isNumber(token) || isOperator(token) || token == "(" || token == ")" ||
        constants.find(token) != constants.end() || variables.find(token) != variables.end();
}

std::vector<std::string> Evaluator::tokenize(const std::string& s, const std::map<std::string, double>& constants, std::map<std::string, double>& variables) {
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

    // Validate tokens
    for (const auto& token : tokens) {
        if (!isValidToken(token, constants, variables)) {
            throw std::runtime_error("Invalid token: " + token);
        }
    }

    return tokens;
}

std::string Evaluator::infixToPostfix(const std::vector<std::string>& tokens) {
    std::stack<std::string> stack;
    std::string postfix;

    std::cout << "\nConverting to Postfix:\n";
    for (const std::string& token : tokens) {
        if (std::isdigit(token[0]) || token.find('.') != std::string::npos || (token.size() > 1 && token[0] == '-')) {  // Número
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
    std::cout << "Postfix Expression: " << postfix << "\n";
    return postfix;
}


float Evaluator::evaluatePostfix(const std::string& postfix) {
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


bool Evaluator::isValidExpression(const std::vector<std::string>& tokens) {
    int openParentheses = 0;
    bool lastWasOperator = true; // Asume que la expresión empieza incorrectamente con un operador

    for (const auto& token : tokens) {
        if (token == "(") {
            openParentheses++;
            lastWasOperator = true; // Un paréntesis abierto después puede ser seguido por un operador
        }
        else if (token == ")") {
            if (openParentheses == 0) {
                throw std::runtime_error("Mismatched parentheses: Too many closing parentheses");
            }
            if (lastWasOperator) {
                throw std::runtime_error("Invalid sequence: operator before closing parenthesis");
            }
            openParentheses--;
        }
        else if (isOperator(token)) {
            if (lastWasOperator) {
                throw std::runtime_error("Invalid sequence of operators");
            }
            lastWasOperator = true;
        }
        else {
            lastWasOperator = false;
        }
    }

    if (openParentheses != 0) {
        throw std::runtime_error("Mismatched parentheses: Not all parentheses are closed");
    }

    // Verifica si la expresión empieza o termina con un operador
    if (!tokens.empty() && (isOperator(tokens.front()) || isOperator(tokens.back()))) {
        throw std::runtime_error("Expression cannot start or end with an operator");
    }

    return true;
}

bool Evaluator::hasConsecutiveOperators(const std::vector<std::string>& tokens) {
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

float Evaluator::evaluateTokens(const std::vector<std::string>& tokens) {
    std::stack<float> values;
    std::stack<char> ops;

    for (const auto& token : tokens) {
        if (isNumber(token)) {
            values.push(std::stof(token));
        }
        else if (token == "(") {
            ops.push('(');
        }
        else if (token == ")") {
            while (!ops.empty() && ops.top() != '(') {
                float val2 = values.top();
                values.pop();
                float val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.pop(); // Remove '(' from ops
        }
        else if (isOperator(token)) {
            while (!ops.empty() && precedence(ops.top()) >= precedence(token[0])) {
                float val2 = values.top();
                values.pop();
                float val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(token[0]);
        }
    }

    while (!ops.empty()) {
        float val2 = values.top();
        values.pop();
        float val1 = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.top();
}


bool Evaluator::inputExpression(const std::string& expression, bool isAssignment) {
    if (isAssignment) {
        size_t equals = expression.find('=');
        if (equals != std::string::npos) {
            std::string varName = expression.substr(0, equals);
            std::string valueExpr = expression.substr(equals + 1);

            // Trim spaces
            varName.erase(0, varName.find_first_not_of(' '));
            varName.erase(varName.find_last_not_of(' ') + 1);
            valueExpr.erase(0, valueExpr.find_first_not_of(' '));
            valueExpr.erase(valueExpr.find_last_not_of(' ') + 1);

            // Verificar que el nombre de la variable empiece con una letra
            if (!std::isalpha(varName[0])) {
                std::cerr << "Invalid variable name. Variable names must start with a letter." << std::endl;
                return false;
            }

            try {
                auto tokens = tokenize(valueExpr, constants, variables);
                if (hasConsecutiveOperators(tokens)) {
                    throw std::runtime_error("Invalid sequence of operators");
                }
                if (!isValidExpression(tokens)) {
                    return false; // If expression is not valid, skip further processing
                }
                // Verificar errores durante la evaluación antes de convertir a postfijo
                float value = evaluateTokens(tokens);
                std::string postfix = infixToPostfix(tokens);
                variables[varName] = value;
                std::cout << "Variable " << varName << " set to " << value << std::endl;
                return true;
            }
            catch (const std::exception& e) {
                std::cerr << "Error setting variable: " << e.what() << std::endl;
                return false;
            }
        }
        else {
            std::cerr << "Invalid assignment format. Please use the format 'variable = value'." << std::endl;
            return false;
        }
    }
    else {
        if (expression.find('=') != std::string::npos) {
            std::cerr << "Assignment not allowed in expression evaluation mode." << std::endl;
            return false;
        }

        try {
            auto tokens = tokenize(expression, constants, variables);
            if (hasConsecutiveOperators(tokens)) {
                throw std::runtime_error("Invalid sequence of operators");
            }
            if (!isValidExpression(tokens)) {
                return false; // If expression is not valid, skip further processing
            }
            // Verificar errores durante la evaluación antes de convertir a postfijo
            float result = evaluateTokens(tokens);
            std::string postfix = infixToPostfix(tokens);
            std::cout << "Result: " << result << std::endl;
            history.push_back(expression + " = " + std::to_string(result));
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }
}


std::string Evaluator::displayHistory() {
    std::cout << "History:\n";
    std::string output = "";
    for (const auto& h : history) {
        std::cout << h << std::endl;
        output += h + "\n";
    }
    return output;
}
