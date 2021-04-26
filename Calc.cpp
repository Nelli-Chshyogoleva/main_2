#include <iostream>
#include "Calculator.hpp"

using namespace std;

int main() {
    try {
        string expression; // выражение
        cout << "Enter expression: ";
        getline(cin, expression); // считывание выражения

        Calculator calculator(expression); // создание калькулятора

        cout << endl << "Read variable values:" << endl;
        calculator.ReadVariables();
        cout << "Value: " << calculator.Evaluate() << endl;

        cout << endl << "Table: " << endl;
        calculator.PrintTable();
    }
    catch (const string& s) {
        cout << s << endl;
    }
}
