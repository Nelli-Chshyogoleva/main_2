#pragma once

#include <iostream>
#include <string>
#include <stack>
#include "VariableStorage.hpp"

const char AND = '&';
const char OR = 'v';
const char NOT = '~';
const char IMPL = '>';
const char INV_IMPL = '<';
const char XOR = '+';
const char EQUAL = '=';
const char SHEFFER = '|';
const char PIRS = '@';

class Calculator {
    std::string expression; // выражение
    std::string rpn; // польская запись
    VariableStorage variables; // переменные

    bool IsNumber(char c) const; // проверка на цифру
    bool IsVariable(char c) const; // проверка на букву
    bool IsOperator(char c) const; // проверка на операцию
    bool IsBracket(char c) const; // проверка на скобку
    bool IsSpace(char c) const; // проверка на пробельный символ
    bool IsLexeme(char c) const; // проверка на лексемный символ
    std::string SplitToLexemes(std::string expression) const; // разбиение выражения на лексемы (удаление пробельных символов)

    int GetPriority(char c); // получение приоритета
    bool IsMorePriority(char curr, char top); // проверка, что текущая лексема менее приоритетна лексемы на вершине стека
    std::string ConvertToRPN(); // получение польской записи

    bool EvaluateOperator(char op, bool arg1, bool arg2) const; // вычисление оператора
public:
    Calculator(const std::string& expression);
    
    bool Evaluate(); // вычисление выражения
    void ReadVariables(); // ввод значений переменных
    void PrintTable(); // вывод таблицы истинности
};

// проверка на цифру
bool Calculator::IsNumber(char c) const {
    return c == '0' || c == '1';
}

// проверка на букву
bool Calculator::IsVariable(char c) const {
    return c != 'v' && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// проверка на операцию
bool Calculator::IsOperator(char c) const {
    return c == AND || c == OR || c == NOT || c == IMPL || c == INV_IMPL || c == XOR || c == EQUAL || c == SHEFFER || c == PIRS;
}

// проверка на скобку
bool Calculator::IsBracket(char c) const {
    return c == '(' || c == ')';
}

// проверка на пробельный символ
bool Calculator::IsSpace(char c) const {
    return c == ' ' || c == '\t';
}

// проверка на лексемный символ
bool Calculator::IsLexeme(char c) const {
    return IsNumber(c) || IsVariable(c) || IsOperator(c) || IsBracket(c);
}

// разбиение выражения на лексемы
std::string Calculator::SplitToLexemes(std::string expression) const {
    int j = 0;

    for (int i = 0; i < expression.length(); i++) {
        if (IsLexeme(expression[i])) { 
            expression[j++] = expression[i]; 
        }
        else if (!IsSpace(expression[i])) { 
            throw std::string("Invalid character in expression '") + expression[i] + "'"; 
        }
    }

    return expression.substr(0, j); 

Calculator::Calculator(const std::string& expression) {
    this->expression = SplitToLexemes(expression); 
    this->rpn = ConvertToRPN();
}

// получение приоритета
int Calculator::GetPriority(char lexeme) {
    if (lexeme == NOT)
        return 7;

    if (lexeme == AND)
        return 6;

    if (lexeme == OR || lexeme == XOR)
        return 5;

    if (lexeme == SHEFFER)
        return 4;

    if (lexeme == PIRS)
        return 3;

    if (lexeme == IMPL)
        return 2;

    if (lexeme == EQUAL)
        return 1;

    return 0;
}

// проверка, что текущая лексема менее приоритетна лексемы на вершине стека
bool Calculator::IsMorePriority(char curr, char top) {
    if (curr == NOT) 
        return GetPriority(top) > GetPriority(curr); 

    return GetPriority(top) >= GetPriority(curr); 
}

// получение польской записи
std::string Calculator::ConvertToRPN() {
    std::string rpn = "";
    std::stack<char> stack;

    for (int i = 0; i < expression.length(); i++) {
        char lexeme = expression[i]; 

        if (IsNumber(lexeme)) { 
            rpn += lexeme; 
        }
        else if (IsOperator(lexeme)) { 
            while (stack.size() > 0 && IsMorePriority(lexeme, stack.top())) { 
                rpn += stack.top(); 
                stack.pop();
            }

            stack.push(lexeme); 
        }
        else if (IsVariable(lexeme)) { 
            rpn += lexeme; 
            variables.AddVariable(lexeme); 
        }
        else if (lexeme == '(') { 
            stack.push(lexeme); 
        }
        else if (lexeme == ')') { 
            while (stack.size() > 0 && stack.top() != '(') { 
                rpn += stack.top();
                stack.pop();
            }

            if (stack.size() == 0) 
                throw std::string("Incorrect expression: brackets are disbalanced");

            stack.pop(); 
        }
        else
            throw std::string("Incorrect expression: unknown lexeme '") + lexeme + "'";
    }

    while (stack.size() > 0) { 
        if (stack.top() == '(') 
            throw std::string("Incorrect expression: brackets are disbalanced");

        rpn += stack.top(); 
        stack.pop();
    }

    return rpn;
}

// вычисление оператора
bool Calculator::EvaluateOperator(char op, bool arg1, bool arg2) const {
    if (op == AND)
        return arg1 && arg2;

    if (op == OR)
        return arg1 || arg2;

    if (op == XOR)
        return arg1 ^ arg2;

    if (op == EQUAL)
        return arg1 == arg2;

    if (op == IMPL)
        return !arg1 || arg2;

    if (op == INV_IMPL)
        return arg1 || !arg2;

    if (op == PIRS)
        return !arg1 && !arg2;

    if (op == SHEFFER)
        return !arg1 || !arg2;

    throw std::string("Unhandled operator '") + op + "'";
}

// вычисление выражения
bool Calculator::Evaluate() {
    std::stack<bool> stack;

    for (int i = 0; i < rpn.size(); i++) {
        char lexeme = rpn[i];

        if (lexeme == NOT) { 
            if (stack.size() < 1) 
                throw std::string("Unable to evaluate NOT"); 

            bool arg = stack.top(); 
            stack.pop();
            stack.push(!arg); 
        }
        else if (IsOperator(lexeme)) { 
            if (stack.size() < 2) 
                throw std::string("Unable to evaluate operator '") + lexeme + "'"; 

            bool arg2 = stack.top(); 
            bool arg1 = stack.top();
            stack.pop();

            stack.push(EvaluateOperator(lexeme, arg1, arg2)); 
        }
        else if (IsVariable(lexeme)) { 
            stack.push(variables.GetValue(lexeme)); 
        }
        else if (IsNumber(lexeme)) { 
            stack.push(lexeme == '1'); 
        }
        else
            throw std::string("Unknown rpn lexeme '") + lexeme + "'"; 
    }

    if (stack.size() != 1) 
        throw std::string("Incorrect expression"); 

    return stack.top(); 
}

// ввод значений переменных
void Calculator::ReadVariables() {
    variables.Read(); 
}

// вывод таблицы истинности
void Calculator::PrintTable() {
    int total = 1 << variables.GetVariablesCount();
    
    variables.PrintHeader();

    for (int i = 0; i < total; i++) {
        variables.SetTableValue(i);
        variables.Print();
        std::cout << Evaluate() << "|" << std::endl;
    }
}
