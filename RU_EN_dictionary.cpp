#include "dictpara.h"
#include "array.h"
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

typedef Array<DictPara> Container;
class B_W{
	ofstream BW;
public:
	B_W(const char* FName){
		BW.open(FName);
		if (!BW){
			cout << "This file doesn't open!" << endl;
		}
	}

	ofstream& getFile(){ 
		return BW; 
	}
	void inFile(const DictPara& X){ 
		BW << X; 
	}
	~B_W(){
		BW.close();
	}
};

bool scan(B_W& Y, DictPara& X){
	const char* S = X.get_word().str();
	while (*S != '\0') {
		if (*S == ' ') {
			Y.inFile(X);
			return 1;
		}
		  ++S;
	}
	if (X.get_word() == '\0'){
		Y.inFile(X);
		return 1;
	} else{
		const char* s = X.get_word().str();
		for (  ; *s != '\0'; s++){
			int i = 0;
			if (*s == ' '){
				i++;
			    if (i == 2){
					  Y.inFile(X);
					  return 1;
				}
			}
		}
	}
	return 0;
}
void un(B_W& bad, const DictPara X, Container& A){
	char* s = X.get_word().str_2();
	vector<char*> cont = {s};
	int i = 1;
	while (*s != '\0'){
		if (*s == ','){
			*s = '\0';
			if (*(s + 1) == ' '){
				cont.push_back(s + 2);
			} else{
				cont.push_back(s++);
			}
			if (*(s - 1) == ' '){
				*(s - 1) = '\0';
			} else{}
				i++;
		}
		s++;
	}
	for (int i = 0; i < cont.size(); i++){
		DictPara Y = DictPara(cont[i], X.get_translate().str());
		if (!scan(bad, Y))A.push_back(Y);
	}
}
int main(){
	char fname[] = "en_ru.txt";
	B_W File("BadWords.txt");
	try{
		ifstream inFile(fname);
		if (!inFile){
			throw stroka("This dictionary doesn't open!");
		}	
    
		DictPara X;
		Container Dictionary;
		int count = 0;
		int tt = time(NULL);
		while (inFile.peek() != EOF){
			inFile >> X;
			un(File, X, Dictionary);
			   ++count;
		}

		inFile.close();
		for (int i = 0; i < Dictionary.size(); i++){
			std::cout << Dictionary[i];
		}

		Dictionary.sort();
		std::ofstream outFile("ru_en.txt");
		for (auto pos = Dictionary.begin(); pos != Dictionary.end(); ++pos)
			outFile << *pos;
	}
	catch (...){
		cout << "ERROR! Check again for correct input or other problem." << endl;
	}
		return 0;
}
