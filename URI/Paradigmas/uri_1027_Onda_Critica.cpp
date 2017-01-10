#include <iostream>
#include <vector>
#include <algorithm>

using value = signed short;

struct Point
{
	Point():x(0), y(0){}
	Point(int x_, int y_): x(x_), y(y_) {}
	~Point() {}

	value x;
	value y; 
};

int n;
value a, b;
Point p[1500];
int w[1500][2];
int main()
{
	while (scanf("%d", &n) == 1)
	{
		for (int i = 0; i < n; i++)
		{
			scanf("%hi %hi", &a, &b);  // O problema pede explicitamente por inteiros de 2 bytes
			p[i] = Point(a, b);        // com sinal, ou seja signed short.
		}

		// Comece ordenando os pontos tal que para dois pontos p e q quaisquer,
		// p será menor do que q se p.x < q.y. Se p.x == q.y, então p < q se
		// p.y < q.y. Está ordenação está baseada no fato de que o problema diz que dois
		// pontos consecutivos não podem estar na mesma coordenada x, portanto eliminamos a possibilidade
		// de encontrar o mesmo ponto em posições diferentes do vetor.
		std::sort(p, p + n, [] (Point const& a, Point const& b)
		{
			return a.x < b.x;
		});

		// Começando a partir da posição de indice um do vetor, voltamos ele
		// buscando por pontos que pertençam a mesma curva que p[i] pertence.
		for (int i = 1; i < n; i++)
		{
			w[i][0] = w[i][1] = 1;             // Como cada ponto é uma curva em potencial,
			Point const& a = p[i];             // cada um começa com valor 1.
			for (int j = (i-1); j >= 0; j--) 
			{
				Point const& b = p[j];

				// Caso o ponto p[j] tenha coordenada igual à de p[x], pulamos para próxima iteração.
				if (a.x > b.x)
				{
					if      ((a.y - b.y) == 2) { w[i][0] = std::max(w[i][0], w[j][1] + 1); }
					else if ((b.y - a.y) == 2) { w[i][1] = std::max(w[i][1], w[j][0] + 1); }}

					// Aqui dividimos o problema em duas vertentes, visto que o enunciado diz sua coordenadas y devem
					// ser na forma a-1, ou a+1. Então, para cada valor p[i].y, buscamos algum valor p[j].y	tal que 
					// a diferença entre eles seja de dois, ou seja, (a.y - b.y) == 2 ou (b.y - a.y) == 2.
					// Suponha que um ponto p[i].y = 5 tenha encontrado um ponto p[j2].y = 3. Logo este ponto pertence,
					// mas o próximo ponto não poderá ser p[j2].y = 1 e sim p[j2].y = 5. Como a linha de cima busca os pontos
					// tal ACIMA da linha (a+1) e a outra abaixo da linha (a-1). Então assim como o ponto p[i].y = 5 busca por pontos
					// p[j].= 3, os pontos p[j].3 na linha de baixa ja terão buscado e marcado pontos p[i].5 anteriores.
					// Ex.:
					//       -----------------------------------------------------------------------
					// vetor:|(0,-3)|(0, 3)|(2,-4)|(2,-1)|(2, 0)|(2, 3)|(4, 1)|(5,-3)|(5,-2)|(5,-1)|
					// (a-1):|   1  |   1  |   1  |   2  |   1  |   1  |   2  |   1  |   2  |   2  |
					// (a+1):|   1  |   1  |   1  |   1  |   1  |   1  |   2  |   3  |   2  |   3  |
					//       -----------------------------------------------------------------------
			}
		}

		// Por fim, basta percorrer os dois vetores e armazenar qual o maior caminho possível.
		int max = 1;
		for  (int i = 0; i < n; i++) { max = std::max(max, std::max(w[i][0], w[i][1])); }

		printf("%d\n", max);	
	}

	return 0;
}