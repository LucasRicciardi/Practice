#include <stdio.h>
#include <stdlib.h>

typedef void * Data;

// #######################################################
// # Estrutura para representar um nó (Node)
// #######################################################

typedef struct node_struct
{
    Data data;                      // ponteiro para o valor salvo
    struct node_struct * right;     // ponteiro para o próximo
    struct node_struct * left;      // ponteiro para o anterior

} Node;

Node * new_node(Data data)
{
    // Cria um novo nó
    Node * node = (Node *) malloc(1 * sizeof(Node));

    // Inicia o nó
    node->data = data;
    node->right = NULL;
    node->left = NULL;

    // Retorna o nó
    return node;
}

Data delete_node(Node * node)
{
    // Retira o valor salvo no nó
    Data data = node->data;

    // Libera a memória do nó
    free(node);

    // Retorna o valor
    return data;
}

void node_set_right(Node * A, Node * B)
{
    // A aponta para B
    A->right = B;

    // Se B não for NULL, B volta para A
    if (B != NULL)
        B->left = A;
}

void node_set_left(Node * A, Node * B)
{
    // A volta para B
    A->left = B;

    // Se B não for NULL, B aponta para A
    if (B != NULL)
        B->right = A;
}

// #######################################################
// # Estrutura lista encadeada para usar de fila e pilha
// #######################################################

typedef struct list_struct
{
    int size;       // tamanho da lista
    Node * head;    // começo da lista
    Node * tail;    // final da lista

} LinkedList;

LinkedList * new_list()
{
    // Cria uma nova lista
    LinkedList * list = (LinkedList *) malloc(1 * sizeof(LinkedList));

    // Inicia a lista
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    // Retorna a lista
    return list;
}

void list_push(LinkedList * list, Data data)
{
    // Cria um novo nó
    Node * node = new_node(data);

    // Se a lista estiver sem 'head'
    if (list->head == NULL)
    {
        list->head = node;
    }

    // Se a lista estiver sem 'tail'
    else if (list->tail == NULL)
    {
        list->tail = node;
        node_set_right(list->head, list->tail);
        node_set_left(list->tail, list->head);
    }

    // Se a lista estiver 'completa'
    else
    {
        node_set_right(list->tail, node);
        node_set_left(node, list->tail);
        list->tail = node;
    }

    // Incrementa o tamanho da lista
    list->size++;
}

Data pop_front(LinkedList * list)
{
    // Salva uma referência ao nó posterior ao 'head'
    Node * new_head = list->head->right;

    // Retira o dado do nó deletado
    Data data = delete_node(list->head);

    // Aponta a nova 'head' e decrementa o tamanho da lista
    list->head = new_head;
    list->size--;

    // Retorna o dado
    return data;
}

Data pop_back(LinkedList * list)
{
    // Salva uma referência ao nó anterior ao 'tail'
    Node * new_tail = list->tail->left;

    // Retira o dado do nó deletado
    Data data = delete_node(list->tail);

    // Aponta a nova 'tail' e decrementa o tamanho da lista
    list->tail = new_tail;
    list->size--;

    // Retorna o dado
    return data;
}

typedef void (*CleanOperation) (void*);
void delete_list(LinkedList * list, CleanOperation clean_operation)
{
    // Enquando tiver elementos na list
    while (list->size > 0)
    {
        // Retira o dado do nó deletado
        Data data = pop_back(list);

        // Se o usuário quiser uma última chance de 'limpar a casa'
        if (clean_operation != NULL)
            clean_operation(data);
    }

    // Libera a lista
    free(list);
}

// #######################################################
// # Estrutura árvore
// #######################################################

typedef int (*FormationRule) (Data, Data);
typedef struct tree_struct
{
    Node * root;                        // raíz da árvore
    FormationRule formation_rule;       // lei de formação da árvore

} Tree;

Tree * new_tree(FormationRule formation_rule)
{
    // Cria uma nova árvore
    Tree * tree = (Tree *) malloc(1 * sizeof(Tree));

    // Inicia a árvore
    tree->root = NULL;
    tree->formation_rule = formation_rule;

    // Retorna a árvore
    return tree;
}

void tree_add_leaf(Tree * tree, Data data)
{
    // Recebe o endereço da raíz
    Node ** root = &(tree->root);

    // Busca a posição correta na árvore
    while ((*root) != NULL)
    {
        if (tree->formation_rule((*root)->data, data))
            root = &((*root)->left);
        else
            root = &((*root)->right);
    }

    // Insere o valor na árvore
    (*root) = new_node(data);
}

int binary_tree_formation_rule(void * root, void * leaf)
{
    int * _root = (int *) root;
    int * _leaf = (int *) leaf;
    return (*_root) > (*_leaf);
}

int main()
{
    Tree * binary_tree = new_tree(binary_tree_formation_rule);
    for (int i = 0; i < 100; i++)
    {
        int * data = (int *) malloc(1 * sizeof(int));
        *data = rand() % 100;
        tree_add_leaf(binary_tree, data);
    }
    foo(binary_tree);
    printf("\n");
    return 0;
}
