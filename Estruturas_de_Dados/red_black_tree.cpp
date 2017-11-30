// default
#include <iostream>
#include <cstdio>

// std lib
#include <functional>

// Mais legível
using None = void;

// ################################################################################
// # Estrutura nó
// ################################################################################

enum Color : unsigned char { Red = 0x00, Black = 0x01 };

template <typename Key, typename Value>
struct Node
{
    // Pai eFilhos
    Node * parent;
    Node * child[2];

    // Cor
    Color color;

    // Chave e valor
    Key key;
    Value value;

    // Construtor
    Node(Key k, Value v, Color c, Node * nil):
        parent(nil),
        child{ nil, nil },
        color(c),
        key(k),
        value(v)
    {}
};

// ################################################################################
// # Árvore Vermelho e preta
// ################################################################################

template <typename Key, typename Value>
class RedBlackTree
{
public: // Typedefs públicos

    // Facilita a vida
    using Leaf = Node<Key, Value>;
    using Depth = int;

private: // Atributos privados

    // Filhos
    static constexpr bool left = false;
    static constexpr bool right = true;

    // nil e root
    Leaf * nil;
    Leaf * root;

private: // Métodos privados

    /*
    *   Rotações
    */
    void rotate(bool d, Leaf * x)
    {
        // Vetor com sentido da rotação e o sentido contrário
        constexpr bool rotation = false;
        constexpr bool other = true;
        bool direction[2] = { d, not d };

        // y é o filho de x no sentido contrário ao da rotação
        Leaf * y = x->child[ direction[other] ];

        // O filho de x no sentido contrário da rotação é o filho de y no sentido da rotação
        x->child[ direction[other] ] = y->child[ direction[rotation] ];
        if (y->child[ direction[rotation] ] != this->nil)
        {
            y->child[ direction[rotation] ]->parent = x;
        }

        // y é filho do pai de x
        y->parent = x->parent;
        if (x->parent == this->nil)
        {
            this->root = y;
        }
        else x->parent->child[ x->key > x->parent->key ] = y;

        // Nova ligação de x e y
        y->child[ direction[rotation] ] = x;
        x->parent = y;
    }

    /*
    *   Fix up
    */
    void fix_up(Leaf * z)
    {
        while (z->parent->color == Color::Red)
        {
            #define d (z->parent == z->parent->parent->child[right])
            constexpr bool father = false;
            constexpr bool uncle = true;
            bool side[2] = { d, not d };
            #undef d

            // y é o tio de z
            Leaf * y = z->parent->parent->child[ side[uncle] ];

            // Caso 1: tio de z é vermelho
            if (y->color == Color::Red)
            {
                z->parent->color = y->color = Color::Black;
                z->parent->parent->color = Color::Red;
                z = z->parent->parent;
            }

            // Casos 2 e 3: tio de z é preto
            else if (y->color == Color::Black)
            {
                // Caso 2: z é filho do lado contrário ao do pai
                if (z == z->parent->child[ side[uncle] ])
                {
                    z = z->parent;
                    this->rotate(
                        side[father], z
                    );
                }
                z->parent->color = Color::Black;
                z->parent->parent->color = Color::Red;
                this->rotate(
                    side[uncle], z->parent->parent
                );
            }
        }
        // Caso 7: a raíz é vermelha
        this->root->color = Color::Black;
    }

public: // Métodos públicos

    /*
    *   Construtor
    */
    RedBlackTree<Key, Value>():
        nil(nullptr),
        root(nullptr)
    {
        this->root = this->nil = new Leaf(
            Key(), Value(), Color::Black, nullptr
        );
    }

    /*
    *   Travessia
    */
    None traverse(std::function<None(Leaf&, Depth)> fn)
    {
        std::function<None(Leaf*, Depth)> t = [&] (Leaf * leaf, Depth const& depth)
        {
            if (leaf != this->nil)
            {
                fn(*leaf, depth);
                t(leaf->child[left], depth+1);
                t(leaf->child[right], depth+1);
            }
        };
        t(this->root, 0);
    }

    /*
    *   Inserção
    */
    bool insert(Key key, Value value)
    {
        // Busca pelo nó que será pai do novo nó z
        Leaf * y = this->nil;
        Leaf * x = this->root;
        while (x != this->nil)
        {
            y = x;
            if (x->key != key)
            {
                x = x->child[ key > x->key ];
            }
            else return false;
        }

        // Cria o novo nó z e insere ele na árvore
        Leaf * z = new Leaf(key, value, Color::Red, this->nil);
        z->parent = y;
        if (y == this->nil)
        {
            this->root = z;
        }
        else y->child[ z->key > y->key ] = z;

        // Conserta a árvore e retorna true
        this->fix_up(z);
        return true;
    }
};

int main()
{
    // Tamanho do input
    printf("Digite o tamanho do input !\n");
    int n; scanf("%d", &n);
    
    // Cria a árvore
    RedBlackTree<int, int> tree;

    // Insere n elementos na árvore
    for (int i = 0; i < n; i++)
    {
        tree.insert(i, i);
    }

    // Travessia pela árvore em pré ordem
    tree.traverse([&] (RedBlackTree<int,int>::Leaf& leaf, int depth)
    {
        for (int i = 0; i < depth; i++)
            printf("----");
        printf("---> Key: %d\n", leaf.key);
    });

    // Retorna 0
    return 0;
}
