// https://www.urionlinejudge.com.br/judge/pt/problems/view/1236

#include <iostream>
#include <algorithm>
#include <cmath>

struct Buffer
{
	int  size;
	char word;

	Buffer(): size(0) {}

	bool empty() { return this->size == 0; }

	void verify(char c, std::string& str)
	{
		if (this->empty())
		{
			if (c == '0' or c == ' ') { this->add(c); }
			else                      { str += c; }
		}
		else
		{
			if (this->word == c) { this->add(c); }
			else                 { this->erase(str); this->verify(c, str); }
		}
	}

	void add(char c)
	{
		this->word = c;
		this->size++;
	}

	void erase(std::string& str)
	{
		if (this->size <= 2)
		{
			while (this->size--) { str += this->word; }	
		}
		else 
		{
			char temp = this->word == '0' ? '#' : '$';
			while (this->size > 0)
			{
				str += temp;
				str += char(std::min(255, this->size));
				this->size -= 255;
			}
		}	

		this->size = 0;
	}
};

int main(int argc, char const *argv[])
{
	int n; std::cin >> n;
	getchar();
	while (n--)
	{
		std::string str, res;
		getline(std::cin, str);

		Buffer buffer;
		for_each(str.begin(), str.end(), [&](char& c)
		{
			buffer.verify(c, res);
		});

		//  1  Recebe o input.
		//  2  Verifica se o buffer está vazio:
		//          Se está vazio:
		//                 Verifica se o input é '0' ou ' ':
		//                        Se não:
		//                                Adiciona o input à fila (res).
		//                        Se sim:
		//                                Adiciona o input ao buffer.
        //          Se não está vazio:
        //                 Verifica se o input é igual ao valor do buffer:
        //                        Se não:
        //                                Esvazia o buffer e retorna ao passo 2.
        //                        Se sim:
        //                                Incrementa o tamanho do buffer.  

		if (not buffer.empty()) { buffer.erase(res); }   // Ao final, esvazia o buffer caso ainda não esteja vazio.

		std::cout << res << '\n';
	}

	return 0;
}