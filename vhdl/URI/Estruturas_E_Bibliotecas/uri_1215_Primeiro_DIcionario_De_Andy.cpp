// https://www.urionlinejudge.com.br/judge/pt/problems/view/1215

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>

int main(int argc, char const *argv[])
{
	std::set< std::string > s;
	char word[1500];
	while (scanf("%s", word) == 1)  // A entrada só termina quando recebe EOF (CTRL+D no linux)
	{
		getchar();                             // Recebe o '\n'
		int size = strlen(word);               // Continua caso não tenha sido lido nenhuma palavra
		if (not size) { continue; }
		
		for (int i = 0; i < size; i++)
		{
			word[i] = tolower(word[i]);                                // Converte cada letra para minúscula.
			if ('a' > word[i] or word[i] > 'z') { word[i] = ' '; }     // Caso a letra não tenha sido convertida, 
		}                                                              // receberá um valor de ' '.

		char * temp = strtok(word, " ");                               // Aqui a função strtok procura pelo token " "
		while (temp not_eq NULL)                                       // e transforma ele em '\0', depois retorna um ponteiro
		{		                                                       // para o inicio de temp (&temp[0]).
			if (strlen(temp)) { s.insert(temp); }                      // Dessa forma, eu verifico se exista algum caracter
			temp = strtok(NULL, " ");                                  // nesta palavra e adiciono ao conjunto caso sim.
		}
	}

	for_each(s.begin(), s.end(), [](std::string const& str)           // Para std::set a função for_each parra T const& t.
	{
		printf("%s\n", str.c_str());
	});

	return 0;
}