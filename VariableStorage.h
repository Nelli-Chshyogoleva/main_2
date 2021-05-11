#pragma once

#include <string>
#include <vector>

class VariableStorage {
    struct Variable {
        char name; // имя переменной
        bool value; // значение переменной
    };

    std::vector<Variable> variables;

    int IndexOfVariable(char name) const; // поиск переменной по имени
public:
    void AddVariable(char name); // добавление переменной
    void SetVariable(char name, bool value); // установка значения переменной
    void SetTableValue(int value); // задание значения для таблицы истинности
    
    bool GetValue(char name) const; // получение значения переменной по имени
    int GetVariablesCount() const; // получение количества переменных
    
    void Read(); // ввод значений

    void PrintHeader() const; // вывод шапки
    void Print() const; // вывод строки переменных
};

// поиск переменной по имени
int VariableStorage::IndexOfVariable(char name) const {
    for (int i = 0; i < variables.size(); i++)
        if (variables[i].name == name) 
            return i; 

    return -1; 
}

// добавление переменной
void VariableStorage::AddVariable(char name) {
    if (IndexOfVariable(name) != -1) 
        return; 

    
    Variable var;
    var.name = name;
    var.value = 0;

    variables.push_back(var); 
}

// установка значения переменной
void VariableStorage::SetVariable(char name, bool value) {
    int index = IndexOfVariable(name); 

    if (index == -1) { 
        Variable v;
        v.name = name;
        v.value = value;

        variables.push_back(v); 
    }
    else {
        variables[index].value = value; 
    }
}

// получение значения переменной по имени
bool VariableStorage::GetValue(char name) const {
    int index = IndexOfVariable(name); 
    if (index == -1) 
        throw std::string("Variable '") + name + "' is not correct!";

    return variables[index].value; 
}

// получение количества переменных
int VariableStorage::GetVariablesCount() const {
    return variables.size();
}

// задание значения для таблицы истинности
void VariableStorage::SetTableValue(int value) {
    for (int i = 0; i < variables.size(); i++) {
        variables[i].value = (value >> (variables.size() - 1 - i)) & 1;
    }
}

// ввод значений
void VariableStorage::Read() {
    for (int i = 0; i < variables.size(); i++) {
        std::cout << "Enter value of '" << variables[i].name << "': ";
        std::cin >> variables[i].value; 
    }
}

// вывод шапки
void VariableStorage::PrintHeader() const {
    std::cout << "|";

    for (int i = 0; i < variables.size(); i++) {
        std::cout << variables[i].name << "|";
    }

    std::cout << "F|" << std::endl;
}


// вывод строки переменных
void VariableStorage::Print() const {
    std::cout << "|";

    for (int i = 0; i < variables.size(); i++) {
        std::cout << variables[i].value << "|";
    }
}
