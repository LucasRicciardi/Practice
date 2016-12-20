// https://www.urionlinejudge.com.br/judge/pt/problems/view/1083

#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

int procedencia(char c)
{
	if (c == '^')
		return 6;

	if ( c == '*' or c == '/')
		return 5;

	if ( c == '+' or c == '-')
		return 4;

	if ( c == '>' or c == '<' or c == '=' or c == '#')
		return 3;

	if ( c == '.')		// AND
		return 2;

	if ( c == '|')
		return 1;

	return 0;
}

string check_stack(stack<char>& v, char c)
{
	string r = "";

	if (v.empty())			// Se a stack estiver vazia, adiciona o operador
	{
		v.push(c);
	}
	else
	{
		if (c == '(')		// Se estiver abrindo um parênteses, adiciona ele e retorna
		{
			v.push(c);
			return r;
		}

		if (c == ')')		// Se estiver fechando um parênteses,
		{					// Elimna os elementos até o '(', e retira ele também
			while (v.top() not_eq '(')
			{
				r += v.top(); v.pop(); 
			}

			v.pop();
			return r;
		}

		while (true)
		{
			if (v.empty())
				break;

			if (v.top() == '(')
				break;

			if (procedencia(c) > procedencia(v.top()))
				break;

			r += v.top(); v.pop();
		}

		v.push(c);
	}

	return r;
}

bool is_operator(char c)
{
	string op = "^*/+-<>=#.|()";

	for (int i = 0; i < op.size(); i++)
		if (op[i] == c)
			return true;

	return false;	
}

bool is_operando(char c)
{
	if ( 'a' <= c  and c <= 'z')
		return true;

	if ( 'A' <= c and c <= 'Z')
		return true;

	if ( '0' <= c and c <= '9')
		return true;

	return false;
}

string to_rpn(string s)
{
	string rpn = "";

	stack<char> v;

	for_each(s.begin(), s.end(), [&] (char& c)
	{
		if (is_operator(c))
			rpn += check_stack(v,c);
		else
			rpn += c;
	});

	while (not v.empty())
	{
		if (v.top() == '(')
			continue;
		else
			rpn += v.top(); v.pop();
	}

	return rpn;
}

bool validate(string& s)
{
	int parentesis = 0;
	int num_operador = 0;
	int num_operando = 0;

	bool already = false;

	for (int i = 0; i < s.size(); i++)
	{
		char c = s[i];

		if (not is_operator(c) and not is_operando(c))
		{
			s = "Lexical Error!";
			return false;
		}

		if (c == '(')
		{
			parentesis++;
			continue;
		}	

		if (c == ')')
		{
			parentesis--;
			continue;
		}

		if (is_operator(c) and (c not_eq '(' and c not_eq ')'))
		{
			already = false;
			num_operador++;
		}
		else
		{
			if (already == false)
			{
				already = true;
				num_operando++;
			}
			else
			{
				s = "Syntax Error!";
				return false;
			}
		}
	}

	if ((num_operando - num_operador) not_eq 1 or (parentesis not_eq 0) ) 
	{
		s = "Syntax Error!";
		return false;
	} 

	return true;
}

int main(int argc, char const *argv[])
{
	string s;

	while (cin >> s)
	{
		if (not validate(s))
		{
			cout << s << endl;
		}
		else 
		{
			cout << to_rpn(s) << endl;
		}
			
	}

	return 0;
}

