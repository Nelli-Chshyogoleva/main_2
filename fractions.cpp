#include <iostream>
#include <string>

using namespace std;
class Fraction
{
private:
	int numerator, denominator;
	int NOD(int a, int b){
		while (a > 0 && b > 0) {
			if (a > b) {
				a %= b;
			}
			else {
				b %= a;
			}
		}
		return a + b;
	}
public:
	Fraction(int num = 0, int denum = 1) : numerator(num), denominator(denum){
		//cout << "I was born!";
	}
	Fraction(const Fraction& X) : numerator(X.numerator), denominator(X.denominator){
		// Копирующий конструктор
		//cout << "I was born like a copy!\n";
	}
	~Fraction() {
		//cout << "Noooo, I'm dying";
	}
	int num()const { return numerator; }
	int denum()const { return denominator; }
	Fraction operator+(const Fraction& B){
		int krat1 = 1, krat2 = 1;
		for (int i = 1;  ; ++i){
			if (denominator * i % B.denominator == 0){
				krat1 = i;
				break;
			}
		}
		krat2 = denominator * krat1 / B.denominator;
		return Fraction(numerator*krat1 + B.numerator * krat2, denominator * krat1);
	}
	Fraction operator-(const Fraction& B){
		int krat1 = 1, krat2 = 1;
		for (int i = 1;; ++i){
			if (denominator * i % B.denominator == 0){
				krat1 = i;
				break;
			}
		}
		krat2 = denominator * krat1 / B.denominator;
		return Fraction(numerator * krat1 - B.numerator * krat2, denominator * krat1);
	}
	Fraction operator/(const Fraction& B){
		return Fraction(numerator * B.denominator, denominator * B.numerator);
	}
	Fraction operator*(const Fraction& B){
		return Fraction(numerator * B.numerator, denominator * B.denominator);
	}
	Fraction reduction(){
		int nod = NOD(numerator, denominator);
		return Fraction(numerator / nod, denominator / nod);
	}
};


ostream& operator<<(ostream& out, const Fraction& X){
	out << X.num() << '/' << X.denum();
	return(out);
}

int main(){
	string str;
	int num1 = 0, num2 = 0, denum1 = 0, denum2 = 0;
	char sign;

	cout << "Enter an expression with fractions: ";
	cin >> str;
	char buff[128];
	int i = 0;
	while (str[i] != '\0'){
    for (int k = 0; str[i] != '/'; ++i, ++k){
			buff[k] = str[i];
		}
		num1 = atoi(buff);
		memset(buff, 0, 128);
		++i;

		for (int k = 0; ; ++i, ++k){
			if (str[i] == '/' || str[i] == '*' || str[i] == '-' || str[i] == '+')
				break;
			buff[k] = str[i];
		}
		denum1 = atoi(buff);
		sign = str[i];
		memset(buff, 0, 128);
		++i;

		for (int k = 0; str[i] != '/'; ++i, ++k){
			buff[k] = str[i];
		}
		num2 = atoi(buff);
		memset(buff, 0, 128);
		++i;

		for (int k = 0; str[i] != '\0'; ++i, ++k){
			buff[k] = str[i];
		}
		denum2 = atoi(buff);
		memset(buff, 0, 128);
	}
	
	Fraction X(num1, denum1);
	Fraction Y(num2, denum2);
	Fraction C;
	
	if (sign == '+')
		C = X + Y;
	else if (sign == '-')
		C = X - Y;
	else if (sign == '*')
		C = X * Y;
	else if (sign == '/')
		C = X / Y;
	
	//Fraction A(2, 3), B(3,4),C(7,6),D(5,9),E(8,5),F(3,6);
	//Fraction otv = (A + B)*(C - D) / (E - F);
	//cout << otv.reduction();
	cout << str << " = " << C.reduction();
	return 0;
}
