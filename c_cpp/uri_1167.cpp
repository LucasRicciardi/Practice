// https://www.urionlinejudge.com.br/judge/pt/problems/view/1167

#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
	while (true)
	{
		int n; scanf("%d%*c", &n);
		if (not n) { return 0; }

		vector<string> names(n);
		vector<pair<int, int> > v(n);

		for (int i = 0; i < n; i++)
		{
			cin >> names[i];
			cin >> v[i].first;
			v[i].second = i;
		}

		int kid, jump, ref;
		kid = 0, jump = ref = v.at(0).first;

		while (v.size() > 1)
		{
			// Na primeira iteração, eu dou o salto completo visto que
			// o primeiro elemento é o de índice zero. Quando um for elimi-
			// nado, tenho de voltar ou avançar uma posição dependendo se estou
			// indo no sentido horário ou anti-horário. Para evitar isso reduzo
			// o tamanho do salto em um para as próximas iterações. 
			if ( v.size() not_eq n )  { jump = (ref - 1) % v.size(); }
			else					  { jump = ref % v.size(); }	
			
			// Caso a volta seja no sentido horário, basta somar o salto naturalmente.
			// Caso contrário, voltar para trás equivale a dar uma volta no círculo e
			// depois voltar as k posições. Como nas linhas acima eu reduzi o salto para 
			// um valor menor do que o tamanho do círculo, é garantido que
			// (kid + v.size() - jump) < v.size(), logo a operação é realizada com inteiros
			// positivos. 
			if (ref & 1)  { kid = (kid + jump) % v.size(); }
			else          { kid = (kid + v.size() - jump) % v.size(); }

			// Salvo a referência usada para a próxima volta e retiro a criança que
			// foi eliminada nesta rodada.
			ref = v.at(kid).first;
			v.erase(v.begin() + kid);

			// A cada criança eliminada, a contagem se reinicia com a criança ANTERIOR
			// áquela que foi eliminada. Como estou AVANÇANDO uma criança nas primeiras linhas
			// deste laço, quando a próxima volta for para trás, eu compenso esse avanço voltando
			// a posição aqui.
			if (not (ref & 1)) { kid = (kid + v.size() - 1) % v.size(); }
		}
	
		cout << "Vencedor(a): " << names[v[0].second] << endl;
	}

	return 0;
}