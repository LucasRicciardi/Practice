#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

#define max 1000

int a[max][max], b[max][max], c[max][max];

pthread_t t[4];
pthread_mutex_t mutex;

struct data
{
	int tid;
	int slice;
};

void * work(void * args)
{
	std::clock_t start = std::clock();
	
	struct data * d = (struct data * ) args;

	int from = d->tid * d->slice;
	int to   = (d->tid + 1) * d->slice;

	for (int i = from; i < to; i++)
	{
		for (int j = 0; j < max; j++)
		{
			c[i][j] = 0;
			for (int k = 0; k < max; k++)
			{
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	
	std::clock_t diff = (std::clock() - start) * 1000 / CLOCKS_PER_SEC;
	pthread_mutex_lock(&mutex);
	printf("thread %d %d segundos %d milisegundos\n", d->tid, diff / 1000, diff % 1000);
	pthread_mutex_unlock(&mutex);

	free(d);
}
	
void multiply()
{
	std::clock_t start = std::clock();
	
	int slice = max / 4;
	for (int i = 0; i < 4; i++)
	{
		struct data * d = (struct data *) malloc(sizeof(int));
		d->tid 	= i;
		d->slice = slice;
		pthread_create(&t[i], NULL, work, d);
	}
	
	for (int i = 0; i < 4; i++) { pthread_join(t[i], NULL); }

	std::clock_t diff = (std::clock() - start) * 1000 / CLOCKS_PER_SEC;
	printf("%d segundos %d milisegundos\n", diff / 1000, diff % 1000);
	
}

void generate_random()
{
	for (int i = 0; i < max; i++)
	{
		for (int j = 0; j < max; j++)
		{
			a[i][j] = rand() % 10 + 1;
			b[i][j] = rand() % a[i][j];
		}
	}
}

int main()
{
	generate_random();
	multiply();
	
	struct data * x = (struct data *) malloc(sizeof(struct data));
	x->tid   = 0;
	x->slice = max;
	work(x);
	
	return 0;
}