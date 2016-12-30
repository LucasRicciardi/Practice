#include <stdio.h>
#include <stdlib.h>

#define nil NULL

typedef struct node
{
	int data;

	struct node * next;
	struct node * prev;

} *NODE;

NODE new_node(int data, NODE next, NODE prev)
{
	NODE n = (NODE) malloc(1 * sizeof(struct node));

	n->data = data;
	n->next = nil;
	n->prev = nil;
}

typedef struct queue
{
	int size;

	NODE front;
	NODE back;

} *QUEUE;

QUEUE new_queue()
{
	QUEUE q = (QUEUE) malloc(1 * sizeof(struct queue));

	q->size  = 0;
	q->front = nil;
	q->back  = nil;
}

int size(QUEUE q) { return q->size; }  // Retorna o tamanho da fila.

int isEmpty(QUEUE q) { return size(q) == 0; } // Retorna 1 se a fila estive vazia.

int peek(QUEUE q)					// Retorna o elemento do topo
{                                   // da fila sem retirar ele.
	if (isEmpty(q))                   
		return -1;

	return q->front->data;
}

void push(QUEUE q, int data)
{
	if (isEmpty(q))                                          // Se a fila estiver vazia, adiciona
	{                                                        // um novo elemento que será tanto o
		q->front = new_node(data, nil, nil);                 // primeiro quanto o último.
		q->back  = q->front;
	}
	else                                                     // Se não, adiciona um novo elemento
	{                                                        // no final da fila.
		NODE n = q->back;
		n->prev = new_node(data, n, nil);
		q->back = n->prev;
	}

	q->size++;    // Aumenta o tamanho da fila.
}

void pop(QUEUE q)
{
	if (isEmpty(q))              // Se estive vazia, volta.
		return;

	NODE n = q->front;
	
	if (size(q) == 1)             // Se tiver somente um elemento na fila remove ele
	{
		free(n);
	}
	else                          // Se não, aponta o último para o anterior ao primeiro
	{                             // e remove o primeiro.
		q->front = n->prev;
		free(n);
	}

	q->size--;                   // Diminuí o tamanho da fila.
	return;
}

void kill(QUEUE q)
{
	while (!isEmpty(q))          // Enquanto a fila não estiver vazia,
		pop(q);                 // remova um por um os elemento dela.
	
	free(q);	
}

int main(int argc, char const *argv[])
{	
	while (1)
	{	
		int n; scanf("%d", &n);
		
		if (!n) { return 0; }

		QUEUE q = new_queue();

		int i;
		for (i = 1; i <= n; i++)
			push(q, i);

		while (!isEmpty(q))
		{
			printf("%d\n", peek(q));
			pop(q);	
		}
	}

	return 0;
}