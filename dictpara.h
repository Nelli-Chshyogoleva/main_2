#ifndef __DICTPARA_15_2021__
#define __DICTPARA_15_2021__

#include "stroka.h"

class DictPara {
	stroka word, translate;
public:
	DictPara() : word(""), translate("") {}
	DictPara(const stroka& w, const stroka& t) : word(w), translate(t) {}
	DictPara(stroka&& w, stroka&& t) : word(std::move(w)), translate(std::move(t)) {}

	DictPara(const DictPara& X) : word(X.word), translate(X.translate) {} // не обязательно реализовывать в данном случае
	DictPara(DictPara&& X) : word( std::move(X.word) ), translate( std::move(X.translate) ) {}

	DictPara& operator=(const DictPara& X){
		if (this != &X){
			word = X.word;
			translate = X.translate;
		}
		return *this;
	}

	DictPara& operator=(DictPara&& X){
		if (this != &X) {
			word = std::move(X.word);
			translate = std::move(X.translate);
		}
		return *this;
	}

	const stroka& get_word()const { return word; }
	const stroka& get_translate()const { return translate; }
	bool operator==(const DictPara& X) const { return word == X.word; }
	bool operator!=(const DictPara& X) const { return word != X.word; }
	bool operator<(const DictPara& X) const { return word < X.word; }
};

#include <iostream>
std::ostream& operator<< (std::ostream& out, const DictPara& X) {
	out << X.get_word() << ": " << X.get_translate() << std::endl;
	return out;
}

#include <fstream>
std::ifstream& operator>> (std::ifstream& in, DictPara& X){
	char buf[1024];
	in.getline(buf, 1024);
	char* p = buf;
	while ((*p != '\t') && (*p != '\0')) ++p;
	if (*p) { *p = '\0'; ++p; }
	X = DictPara(buf, p);
	return in;
}

#endif
