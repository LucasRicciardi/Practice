#include <iostream>

template <typename Key, typename Value>
struct Node
{
    // Ligações
    Node * left;
    Node * right;
    Node * parent;

    // Cor
    enum Color { Red, Black } color;

    // Dados salvos
    Key key;
    Value value;

    Node(Key k, Value v, Node * p = nullptr):
        left(nullptr),
        right(nullptr),
        parent(p),
        color(Node::Color::Red),
        key(k),
        value(v)
    {}
};

// ########################################################################
// # RedBlackTree
// ########################################################################

template <typename Key, typename Value>
class RedBlackTree
{
    /*
    *   Propriedades de uma árvore Rubro-Negra (RedBlackTree):
    *       1. Todo nó é vermelho ou preto
    *       2. A raíz da árvore é preta
    *       3. Toda folha (Nil) é preta
    *       4. Se um nó é vermelho, então seus filhos são todos pretos
    *       5. Para cada nó, todos os caminhos simples de um nó até suas
    *           folhas descendentes contém o mesmo número de nós pretos
    */

    // Facilita a vida
    using Leaf = Node<Key, Value>;

// Atributos
private:
    Leaf * root;

// Métodos
private:
    // ########################################################
    // # Rotações
    // ########################################################

    void left_rotate(Leaf * x)
    {
        // y é o filho à direita de x
        Leaf * y = x->right;

        // Novas configurações de y:
        //  -- Pai de y será o pai de x
        //  -- Filho à esquerda de y será x
        //  -- Filho à direita de y não muda

        // Novas configurações de x:
        //  -- Pai de x será y
        //  -- Filho à esquerda de x não muda
        //  -- Filho à direita de x será o filho à esquerda de y

        // Reposicionamento externo
        y->parent = x->parent;
        x->right = y->left;

        // Reposicionamento interno
        y->left = x;
        x->parent = y;

        // Valores que não mudam
        y->right = y->right;
        x->left = x->left;
    }

    void right_rotate(Leaf * x)
    {
        // y é o filho á esquerda de x
        Leaf * y = x->left;

        // Novas configurações de y:
        //  -- Pai de y será o pai de x
        //  -- Filho à esquerda de y não muda
        //  -- FIlho a direita de y será x

        // Novas configurações de x:
        //  -- Pai de x será y
        //  -- Filho à esquerda de x será o filho a direita de y
        //  -- Filho à direita de x será o mesmo

        // Reposicionamento externo
        y->parent = x->parent;
        x->left = y->right;

        // Reposicionamento interno
        y->right = x;
        x->parent = y;

        // Valores que não mudam
        y->left = y->left;
        x->right = x->right;
    }

    // ########################################################
    // # Fix Up
    // ########################################################
    void fix_up(Leaf * z)
    {
        // Primeira condição violada:
        //  -- Se z é vermelho e o pai de z também,
        //  então a propriedade 4 da árvore foi violada
        while (z->parent->color == Leaf::Color::Red)
        {
            // Se o pai de z for filho à esquerda do avô de z
            if (z->parent == z->parent->parent->left)
            {
                // y é o filho a direita do avô de z
                Leaf * y = z->parent->parent->right;

                if (y->color == Leaf::Color::Red)
                {
                    z->parent->color = Leaf::Color::Black;
                    y->color = Leaf::Color::Black;
                    z->parent->parent->color = Leaf::Color::Red;
                    z = z->parent->parent;
                }

                // Se z for filho a direita
                else if (z == z->parent->right)
                {
                    z = z->parent;
                    this->left_rotate(z);
                }
            }

            // Se o pai de z for filho à direita do avô de z
            else
            {

            }
        }

        // Aplica preto à cor da raíz
        this->root->color = Leaf::Color::Black;
    }

public:
    RedBlackTree():
        root(nullptr)
    {}

    // ########################################################
    // # Inserção
    // ########################################################
    bool insert(Key key, Value value)
    {
        // Busca pelo elemento que será pai do novo elemento
        Leaf * y = nullptr;
        for (Leaf * x = this->root; x != nullptr; )
        {
            // Salva em y o último nó visitado
            y = x;

            // Vai para a esquerda
            if (x->key < key)
            {
                x = x->left;
            }

            // Vai para a direita
            else if (x->key > key)
            {
                x = x->right;
            }

            // Achou um elemento, retorna false
            else return false;
        }

        // Não existe um elemento de chave 'key', então cria um novo nó
        Leaf * z = new Leaf(key, value, y);

        // Se o y for null, então z é a raíz da árvore
        if (y == nullptr)
        {
            this->root = z;
        }

        // Se a chave de z for menor do que a chave de y, então z é fiho a esquerda
        else if (y->key < z->key)
        {
            y->left = z;
        }

        // Se a chave de z for maior do que a chave de y, então z é filho a direita
        else if (y->key > z->key)
        {
            y->right = z;
        }

        // Conserta a árvore para manter as propriedades
        this->fix_up(z);

        // Retorna true
        return true;
    }
};

int main()
{
    RedBlackTree<int, int> rb;
    return 0;
}
