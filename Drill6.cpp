#include <iostream>
#include "std_lib_facilities.h"
class Token {
public:
	char kind;
	double value;
};
class Token_stream {
public:
	Token get();
	void putback(Token t);
private:
	bool full{ false };
	Token buffer;
};
void Token_stream::putback(Token t)
{
	if (full)error("putback() into a full buffer");
	buffer = t;
	full = true;
}
Token Token_stream::get()
{
	if (full) {
		full = false;
		return buffer;
	}
	char ch;
	cin >> ch; 
	switch (ch)		
	{
		case '=':
		case 'x':
		case '(': case ')': case '+': case '-': case '*': case '/':
			return Token{ ch }; 
		case '.':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			{ cin.putback(ch); 
				double val;
				cin >> val; 
				return Token{ '8',val }; 
			}
		default :
			error("Bad token");
	}
}
Token_stream ts;
double expression();

double primary()
{
	Token t = ts.get();
	switch (t.kind)
	{
	case'(':
	{
		double d = expression();
		t = ts.get();
		if (t.kind != ')')error("')'Expected");
		return d;
	}
	case'8':
		return t.value;
	default:
		error("Primary expected");
	}
}
double term()
{
	double left = primary();
	Token t = ts.get();
	while (true)
	{
		switch (t.kind)
		{
		case'*':
			left *= primary();
			t = ts.get();
			break;
		case'/':
		{
			double d = primary();
			if (d == 0)error("Devided by zero!");
			left /= d;
			t = ts.get();
			break;
		}
		/*case'%':
			left %= primary();
			t = get_token();
			break;*/
		default:
			ts.putback(t);
			return left;
		}
	}
}
double expression()
{
	double left = term();
	Token t = ts.get();
	while (true)
	{
		switch (t.kind)
		{
		case'+':
			left += term();
			t = ts.get();
			break;
		case'-':
			left -= term();
			t = ts.get();
			break;
		default:
			ts.putback(t);
			return left;
		}
	}
	return left;
}

vector<Token>tok;

int main()
try
{
	cout << "Welcom to our simple calculator.\nPlease enter expressions using floating-point numbers.";
	cout << "\nHow to use:\n \t1.The whole calculation need to be written in like this: (1+1)/2";
	cout << "\t2.You can use the folloving operators: '(', ')', '+', '-', '*', '/'.\n";
	cout << "\t3.To see the result you need to type in the following operator: '='\n";
	cout << "\t4.To exit from the calculator you need to type in the following operator: 'x'\n";

	double val = 0 ;
	while (cin)
	{
		Token t = ts.get();
		if (t.kind == 'x')break;
		if (t.kind == '=')
			cout << "=" << val << '\n';
		else
			ts.putback(t);
		val = expression();
	}
	keep_window_open();
	return 0;
}
catch(exception& e)
{
	cerr << e.what() << '\n';
	keep_window_open();
	return 1;
}
catch (...)
{
	cerr << "excetion \n";
	keep_window_open();
	return 2;
}
