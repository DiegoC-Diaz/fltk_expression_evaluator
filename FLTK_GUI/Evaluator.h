#pragma once
#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <iostream>
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
#include <stdexcept>
//for error messages. 
#include <windows.h>
#include <codecvt> // for std::codecvt_utf8
#include <locale>  // for std::wstring_convert


class Evaluator {
public:
    Evaluator();
    bool inputExpression(const std::string& exp, bool isAssignment);
    std::string displayHistory();
    void loadUserConstants(const std::string& filename);

    int precedence(char op);
    float applyOp(float a, float b, char op);
    bool isOperator(const std::string& token);
    bool isNumber(const std::string& s);
    bool isValidToken(const std::string& token, const std::map<std::string, double>& constants, const std::map<std::string, double>& variables);
    std::vector<std::string> tokenize(const std::string& s, const std::map<std::string, double>& constants, std::map<std::string, double>& variables);
    std::string infixToPostfix(const std::vector<std::string>& tokens);
    float evaluateTokens(const std::vector<std::string>& tokens);
    float evaluatePostfix(const std::string& postfix);
    bool isValidExpression(const std::vector<std::string>& tokens);
    bool hasConsecutiveOperators(const std::vector<std::string>& tokens);

    std::map<std::string, double> constants;
    std::map<std::string, double> variables;
    std::list<std::string> history;
    //Added new history for postfix expression
    std::list<std::string> postfixHistory;
};

#endif // EVALUATOR_H
