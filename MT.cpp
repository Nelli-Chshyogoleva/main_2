#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_TAPE_SIZE = 2000000; // максимальный размер ленты
const int INIT_HEAD_POSITION = 1000000; // начальное положение головки
const int MAX_ITERATIONS = 10000000; // максимальное число итераций

enum class Action {
    LEFT, // смещение влево
    NONE, // остаться на месте
    RIGHT // смещение вправо
};

// правило перехода
struct Rule {
    int currState; // текущее состояние
    char currChar; // текущий символ
    int nextState; // следующее состояние
    char nextChar; // следующий символ
    Action action; // действие
};

// машина Тьюринга
class TuringMachine {
    char *tape; // лента МТ
    int tapePosition; // положение на ленте

    Rule* rules; // массив переходов
    int rulesSize; // количество правил
    int rulesCapacity; // ёмкость массива правил

    bool IsState(const string& state); 
    bool ParseLine(const string& line); 
    void AddRule(const Rule& rule); 

    int GetRuleIndex(int state, char c) const; // поиск индекса состояния

    bool IsZerosLeft() const; 
    void SaveTape(ofstream& f); 
public:
    TuringMachine();

    bool ParseRules(const string& path); 
    bool ParseTape(const string& path); 

    void Run(const string& path); // запуск машины

    ~TuringMachine(); 
};

TuringMachine::TuringMachine() {
    tape = new char[MAX_TAPE_SIZE];
    rulesSize = 0; 
    rulesCapacity = 1; 
    rules = new Rule[rulesCapacity]; 
}

// проверка, что строка является состоянием
bool TuringMachine::IsState(const string& state) {
    if (state.length() < 2)
        return false; 
        
    if (state[0] != 'q')
        return false; 

    for (size_t i = 1; i < state.length(); i++)
        if (state[i] < '0' || state[i] > '9') 
            return false; 

    return true; 
}

// добавление правила
void TuringMachine::AddRule(const Rule& rule) {
    rules[rulesSize++] = rule; 

    if (rulesSize < rulesCapacity) 
        return; 

    rulesCapacity *= 2; 
    Rule* tmp = new Rule[rulesCapacity]; 

    for (int i = 0; i < rulesSize; i++)
        tmp[i] = rules[i];

    delete[] rules; 
    rules = tmp; 

// обработка одной строки
bool TuringMachine::ParseLine(const string& line) {
    stringstream ss(line); // создаём поток
    string currState;
    string currChar;
    string arrow;
    string nextChar;
    string action;
    string nextState;

    
    if (!(ss >> currState >> currChar >> arrow))
        return false; // то строка некорректна
    if (!(ss >> nextChar >> action >> nextState))
        return false; // то строка некорректна
    if (!IsState(currState)) 
        return false; 
    if (currChar != "0" && currChar != "1") // если символ не 0 и не 1
        return false; 
    if (arrow != "->") 
        return false; 
    if (nextChar != "0" && nextChar != "1") 
        return false; 
    if (action != "L" && action != "N" && action != "R") 
        return false;       
    if (!IsState(currState)) 
        return false; 

    Rule rule;
    rule.currState = stoi(currState.substr(1));
    rule.currChar = currChar[0];
    rule.nextState = stoi(nextState.substr(1));
    rule.nextChar = nextChar[0];
    rule.action = Action::NONE;

    if (action == "L") {
        rule.action = Action::LEFT;
    }
    else if (action == "R") {
        rule.action = Action::RIGHT;
    }

    AddRule(rule); 
    return true; 
}

bool TuringMachine::ParseRules(const string& path) {
    ifstream f(path.c_str()); 

    if (!f) { 
        cout << "Unable to open file with rules '" << path << "'" << endl;
        return false; 
    }

    string line; 
    int lineNumber = 0; 
    bool isCorrect = true; 

    rulesSize = 0; 

    while (getline(f, line)) {
        lineNumber++; 

        if (!ParseLine(line)) {
            cout << lineNumber << ". " << line << endl; 
            isCorrect = false; 
            break; 
        }
    }

    f.close(); 
    return isCorrect; 
}

bool TuringMachine::ParseTape(const string& path) {
    ifstream f(path.c_str()); 

    for (int i = 0; i < MAX_TAPE_SIZE; i++)
        tape[i] = '0'; 

    if (!f) { 
        cout << "Unable to open file with tape '" << path << "'" << endl;
        return false; 
    }

    bool isCorrect = true; 
    string line;
    getline(f, line);

    for (int i = 0; i < line.length(); i++) {
        char c = line[i]; 

        if (c != '0' && c != '1') { 
            cout << "Invalid character in tape (" << c << ")" << endl;
            isCorrect = false;
            break;
        }

        tape[INIT_HEAD_POSITION + i] = c; 
    }

    f.close(); 
    return isCorrect; 
}


bool TuringMachine::IsZerosLeft() const {
    for (int i = tapePosition - 1; i >= 0; i--)
        if (tape[i] != '0') 
            return false; 
return true; 

// сохранение ленты в файл
void TuringMachine::SaveTape(ofstream& f) {
    int i = tapePosition; 
    int left = tapePosition; 
    int right = MAX_TAPE_SIZE - 1; 

    if (!IsZerosLeft()) { /
        left = 0;
        while (tape[left] != '1')
            left++;
    }

    while (tape[right] != '1') {
        right--;
    }

    for (int i = left; i <= right; i++)
        f << tape[i];

    f << endl;

    for (int i = left; i <= right; i++)
        f << (i == tapePosition ? '^' : ' ');

    f << endl;

    for (int i = left; i <= right; i++)
        f << (i == tapePosition ? 'q' : ' ');
}

// поиск индекса состояния
int TuringMachine::GetRuleIndex(int state, char c) const {
    for (int i = 0; i < rulesSize; i++)
        if (rules[i].currState == state && rules[i].currChar == c) 
            return i; 
    return -1; 

// запуск машины
void TuringMachine::Run(const string& path) {
    tapePosition = INIT_HEAD_POSITION; 
    int q = rules[0].currState; 
    int tacts = 0; 

    while (tacts < MAX_ITERATIONS && q != 0 && tapePosition >= 0 && tapePosition < MAX_TAPE_SIZE) {
        int index = GetRuleIndex(q, tape[tapePosition]); 
        if (index == -1) {
            cout << "No rule for state q" << q << " and character '" << tape[tapePosition] << "'" << endl;
            break;
        }

        q = rules[index].nextState; 
        tape[tapePosition] = rules[index].nextChar; 
        tacts++;
        
        if (rules[index].action == Action::LEFT) {
            tapePosition--;
        }
        else if (rules[index].action == Action::RIGHT) {
            tapePosition++;
        }
    }

    ofstream f(path.c_str()); 

    if (tacts == MAX_ITERATIONS) { 
        f << "not applicable"; 
    }
    else if (tapePosition < 0 || tapePosition >= MAX_TAPE_SIZE) { 
        f << "out of memmory"; 
    }
    else if (q != 0) { 
        f << "rule error" << endl; 
    }
    else {
        SaveTape(f); 
    }

    f.close(); 
}

TuringMachine::~TuringMachine() {
    delete[] tape;
    delete[] rules; 
}

int main() {
    TuringMachine turingMachine;

    if (!turingMachine.ParseRules("MT.txt")) /
        return -1;

    if (!turingMachine.ParseTape("input.txt")) 
        return -1;

    turingMachine.Run("output.txt");
}
