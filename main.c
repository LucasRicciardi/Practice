#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define upper_limit 2000

typedef struct matrix { unsigned long long int _[upper_limit][upper_limit]; int size; } Matrix;

#define num_threads 4

pthread_t thread[num_threads];
pthread_t sub_thread[num_threads * num_threads];

struct data_
{
	int tid;
	int slice;
	int size;
};

Matrix a;
Matrix b;
Matrix c;

void random_matrix(int size)
{	
	if (size > upper_limit)	{ size = upper_limit; }

	a.size = b.size = size;
	
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			a._[i][j] = rand() % size;
			b._[i][j] = rand() % (a._[i][j] + 1);
		}
	}
}

void product_matrix()
{
	int size = a.size < b.size ? a.size : b.size;
	
	clock_t start = clock();

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			c._[i][j] = 0;
			for (int k = 0; k < size; k++)
			{
				c._[i][j] += a._[i][k] * b._[k][j];
			}
		}		
	}

	clock_t diff = clock() - start;
	int time = diff * 1000 / CLOCKS_PER_SEC;
	
	printf("\n############ VERSAO SERIAL #############\n");
	printf("Matrizes %d por %d ", size, size);
	printf("multiplicadas em %d segundos e %d milisegundos\n\n", time / 1000, time % 1000);
}

void * runner(void * args)
{
	struct data_ *data = args;

	int tid   = data->tid;
	int slice = data->slice; 
	int size  = data->size;
	
	int from  = tid * slice; 
	int to    = (tid + 1) * slice;

	for (int i = from; i < to; i++)
	{
		for (int j = 0; j < size; j++)
		{
			c._[i][j] = 0;
			for (int k = 0; k < size; k++)
			{
				c._[i][j] += a._[i][k] * b._[k][j];
			}
		}
	}
	free(data);
}

void threaded_product_matrix()
{
	int size = a.size < b.size ? a.size : b.size;
	
	for (int i = 0; i < num_threads; i++)
	{
		struct data_ *data = (struct data_ *) malloc(sizeof(struct data_));		
		data->tid   = i;
		data->slice = size / num_threads; 
		data->size  = size;

		pthread_create(&thread[i], NULL, runner, (void*)data);
	}

	for (int i = 0; i < num_threads; i++)
	{
		pthread_join(thread[i], NULL);
	}

	clock_t diff = clock() - start;
	int time = diff * 1000 / CLOCKS_PER_SEC;
	
	printf("\n############ VERSAO PARALELA #############\n");
	printf("Matrizes %d por %d ", size, size);
	printf("multiplicadas em %d segundos e %d milisegundos, \n", time / 1000, time % 1000);
	printf("usando %d threads\n", num_threads);
}

void print_matrix(Matrix matrix)
{
	int size = matrix.size;
	printf("\n");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%.3d ", matrix._[i][j]);
		}
		printf("\n");
	}
	printf("\n");	
}

int main(int argc, char * argv[])
{
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			random_matrix(atoi(argv[i]));
			threaded_product_matrix();
			product_matrix();
		}
	}
	else
	{
		random_matrix(upper_limit);
		threaded_product_matrix();
		product_matrix();
	}

	return 0;
}