#include <stdio.h>
#include <stdlib.h>

// Typedef para não ficar escrevendo void *
typedef void * Data;

// ###############################################################################
// # Estrutura nó básica usada da aplicação
// ###############################################################################

typedef struct node_t
{
    Data data;              // Dado armazenado no nó
    struct node_t * next;   // Ponteiro para o nó seguinte
    struct node_t * prev;   // Ponteiro para o nó anteror

} Node;

Node * node_new(Data data)
{
    // Aloca para memória para o novo nó
    Node * new_node = (Node *) malloc(1 * sizeof(Node));

    // Inicia o nó
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    // Retorna o nó
    return new_node;
}

Data node_delete(Node * node)
{
    // Retira o dado do nó que será deletado
    Data data = node->data;

    // Libera a memória ocupada pelo nó
    free(node);

    // Retorna o dado
    return data;
}

void node_set_next(Node * A, Node * B)
{
    // A aponta para B
    A->next = B;

    // B volta para A se B não for NULL
    if (B != NULL)
    {
        B->prev = A;
    }
}

void node_set_prev(Node * A, Node * B)
{
    // A volta para B
    A->prev = B;

    // B aponta para A se B não for NULL
    if (B != NULL)
    {
        B->next = A;
    }
}

// ###############################################################################
// # Estrura lista encadeada e funções
// ###############################################################################

typedef struct list_t
{
    Node * head;    // Início da lista
    Node * tail;    // Final da lista
    int size;       // Tamanho da lista

} List;

List * list_new()
{
    // Aloca memória para a lista
    List * new_list = (List *) malloc(1 * sizeof(List));

    // Inicia a lista
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0x00;

    // Retorna a lista
    return new_list;
}

void list_insert(List * list, Data data)
{
    // Cria um novo nó para este dado
    Node * new_node = node_new(data);

    // Se a lista está vazia
    if (list->head == NULL)
    {
        list->head = new_node;
    }

    // Se a lista contém apenas um elemento
    else if (list->tail == NULL)
    {
        list->tail = new_node;
        node_set_next(list->head, list->tail);
        node_set_prev(list->tail, list->head);
    }

    // Caso contrário
    else
    {
        node_set_next(list->tail, new_node);
        node_set_prev(new_node, list->tail);
        list->tail = new_node;
    }

    // Incremeta o tamanho da lista
    list->size++;
}

Data list_pop_front(List * list)
{
    // Salva a nó seguinte ao início da lista
    Node * new_head = list->head->next;

    // Retira o valor no início da lista
    Data data = node_delete(list->head);

    // Marca o novo início da lista
    list->head = new_head;

    // Decrementa o tamanho da lista
    list->size--;

    // Retorna o dado
    return data;
}

Data list_pop_back(List * list)
{
    // Salva a nó anterior ao final da lista
    Node * new_tail = list->tail->next;

    // Retira o valor no final da lista
    Data data = node_delete(list->tail);

    // Marca o novo início da lista
    list->tail = new_tail;

    // Decrementa o tamanho da lista
    list->size--;

    // Retorna o dado
    return data;
}

void list_delete(List * list)
{
    // Enquanto a lista tiver nós
    while (list->size > 0)
    {
        // Deleta os nós do começo da lista
        list_pop_front(list);
    }

    // Deleta a lista
    free(list);
}

// ###############################################################################
// # Estrura árvore e funções
// ###############################################################################

// Lei de formação usada para criação da aŕvore
typedef int (*FormationRule) (Data, Data);

// Ordem de travessia da árvore
typedef enum
{
    IN_ORDER = 0,    // Em ordem
    PRE_ORDER = 1,   // Em pré ordem
    POST_ORDER = 2   // Em pós ordem

} TraversalOrder;

typedef struct tree_t
{
    Node * root;                    // Raíz da árvore
    int leafs;                      // Número de folhas da árvore
    FormationRule formation_rule;   // Lei de formação da árvore

} Tree;

Tree * tree_new(FormationRule formation_rule)
{
    // Aloca memória para a árvore
    Tree * new_tree = (Tree *) malloc(1 * sizeof(Tree));

    // Inicia a árvore
    new_tree->root = NULL;
    new_tree->leafs = 0x00;
    new_tree->formation_rule = formation_rule;

    // Retorna a árvore
    return new_tree;
}

Node ** tree_search(Tree * tree, Data data, FormationRule equals)
{
    // Retira o endereço da raíz da árvore
    Node ** node = &(tree->root);

    // Busca o nó correto de acordo com a lei de formação da árvore
    while ((*node) != NULL)
    {
        // Verifica se o nó atual é o nó desejado e retorna ele caso sim
        if (equals != NULL && equals((*node)->data, data))
            return node;

        // Compara o dado buscado com o dado do nó atual
        // segundo a lei de formação da árvore
        if (tree->formation_rule(data, (*node)->data))
        {
            node = &((*node)->prev);
        }
        else
        {
            node = &((*node)->next);
        }
    }

    // Retorna o nó NULL em alguma ponta da árvore
    return node;
}

void tree_insert(Tree * tree, Data data)
{
    // Busca o local correto para inserção na árvore
    Node ** node = tree_search(tree, data, NULL);

    // Insere o novo nó na árvore
    (*node) = node_new(data);

    // Incrementa o número de folhas
    tree->leafs++;
}

Data tree_remove(Node ** node)
{
    // Se o nó não tem nenhum descendente
    if ((*node)->next == NULL && (*node)->prev == NULL)
    {

    }

    // Se o nó tem dois descendentes
    else if ((*node)->next != NULL && (*node)->prev != NULL)
    {

    }

    // Se o nó tem apenas um descendente
    else
    {

    }
}

// Operação para ser realizada durante a travessia
typedef void (*Fn) (Node *);
void tree_traversal(Tree * tree, TraversalOrder order, Fn fn)
{
    // Cria uma nova pilha
    List * stack = list_new();

    // Insere a raíz da árvore na pilha
    list_insert(stack, tree->root);

    // Itera pelos nós da árvore
    while (stack->size > 0)
    {
        // Ordem em que a árvore é percorrida
        switch (order)
        {
            case IN_ORDER:
            {

            }
            break;

            case PRE_ORDER:
            {
                // Retira o nó atual da pilha
                Node * current_node = list_pop_back(stack);

                // Realiza uma operação no nó
                fn(current_node);

                // Insere o da direta
                if (current_node->next != NULL)
                    list_insert(stack, current_node->next);

                // Depois o da esquerda
                if (current_node->prev != NULL)
                    list_insert(stack, current_node->prev);
            }
            break;

            case POST_ORDER:
            {

            }
            break;
        }

    }
}

void tree_delete(Tree * tree)
{
    // Cria uma fila para armazenar os nós
    List * queue = list_new();

    // Insere o endereço da raíz na fila
    list_insert(queue, tree->root);

    // Deleta os nós um a um até que a árvore fique vazia
    while (tree->leafs > 0)
    {
        // Retira o nó atual da árvore
        Node * current_node = list_pop_front(queue);

        // Verifica se existem descendentes
        if (current_node->next != NULL)
            list_insert(queue, current_node->next);

        if (current_node->prev != NULL)
            list_insert(queue, current_node->prev);

        // Deleta o nó atual e diminui o tamanho da árvore
        node_delete(current_node);
        tree->leafs--;
    }

    // Deleta a fila
    list_delete(queue);

    // Deleta a árvore em si
    free(tree);
}

// Lei de formação para a árvore binária canônica
int binary_tree_formation_rule(Data a, Data b)
{
    int * left = (int *) a;
    int * right = (int *) b;
    return (*left) < (*right);
}

void bar(Node * node, int depth)
{
    for (int i = 0; i < depth; i++)
        printf("    ");

    printf("----> Level: %d = ", depth);
    if (node == NULL)
    {
        printf(" ** NULL **\n\n");
        return;
    }
    else
    {
        int * data = (int *) node->data;
        printf(" %d\n\n", *data);
    }
    bar(node->prev, depth+1);
    bar(node->next, depth+1);
}

void foo(Tree * tree)
{
    bar(tree->root, 0);
}

void debug(Node * node)
{
    int * data = (int *) node->data;
    printf("%d\n", (*data));
}

int main()
{
    Tree * tree = tree_new(binary_tree_formation_rule);

    printf("%d\n", 2131);
    
    for (int i = 0; i < 20; i++)
    {
        int * data = (int *) malloc(1 * sizeof(int));
        (*data) = rand() % 100;
        tree_insert(tree, data);
    }


    foo(tree);

    tree_delete(tree);

    return 0;
}
