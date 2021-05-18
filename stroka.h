#ifndef __STROKA_15_2021__
#define __STROKA_15_2021__
#include <string.h>

class stroka {
	char* data;
	int Len;
public:
	stroka(const char* str = "");
	stroka(int N);
	stroka(const stroka&);
	stroka(stroka&&);
	~stroka() {
		if (data)
			delete[] data;
		data = nullptr;
		Len = 0;
	}
	int length()const { return strlen(data); }
	int size()const { return Len; }
	const char* str()const { return data; }
	stroka operator+(const stroka&)const;
	stroka& operator=(const stroka&);
	stroka& operator=(stroka&&);
	bool operator==(const stroka& X)const { return strcmp(data, X.data) == 0; }
	bool operator!=(const stroka& X)const { return strcmp(data, X.data) != 0; }
	//bool operator!=(const stroka& X)const { return !(*this==X); } // this->operator==(X)
	bool operator<(const stroka& X)const { return strcmp(data, X.data) < 0; }
	char operator[](int i)const {
		if (i < 0 || i>Len) throw "Out of string limits";
		return data[i];
	}
};

stroka::stroka(const char* str) {
	Len = strlen(str) + 1;
	data = new char[Len];
	memcpy(data, str, Len);
}

stroka::stroka(int N) {
	Len = N > 0 ? N : 0;
	++Len;
	data = new char[Len];
	data[0] = '\0';
}

stroka::stroka(const stroka& X) {
	Len = X.Len;
	data = new char[Len];
	memcpy(data, X.data, Len);
}

stroka::stroka(stroka&& X) {
	Len = X.Len;
	data = X.data;
	X.data = nullptr;
	X.Len = 0;
}

stroka& stroka::operator=(const stroka& X) {
	if (this != &X) {
		if (data) delete[] data;
		Len = X.Len;
		data = new char[Len];
		memcpy(data, X.data, Len);
	}
	return *this;
}

stroka& stroka::operator=(stroka&& X) {
	if (this != &X) {
		if (data) delete[] data;
		Len = X.Len;
		data = X.data;
		X.data = nullptr;
		X.Len = 0;
	}
	return *this;
}

stroka stroka::operator+(const stroka& X)const {
	int Len1 = length();
	int Len2 = X.length();
	stroka result(Len1 + Len2 + 1);
	memcpy(result.data, data, Len1);
	memcpy(result.data + Len1, X.data, Len2 + 1);
	return result;
}

#include <iostream>
std::ostream& operator<< (std::ostream& out, const stroka& s) {
	out << s.str();
	return out;
}

#endif
