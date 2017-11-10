#include <iostream>
#include <functional>
#include <vector>

// ####################################################
// # Cor
// ####################################################

enum Color : unsigned char { Red, Black };

// ####################################################
// # Nó
// ####################################################

template <class Key, class Value>
struct Node
{
    // Ligações
    Node * left;
    Node * right;
    Node * parent;

    // Cor
    Color color;

    // Chave e valor
    Key key;
    Value value;

    // Construtor
    Node(Key k, Value v, Node * p = nullptr):
        left(nullptr),
        right(nullptr),
        parent(p),
        color(Color::Red),
        key(k),
        value(v)
    {}

    static Node ** find(Node ** root, Node ** nil, Key const& key)
    {
        Node ** x = root;
        while ((*x) != (*nil))
        {
            // Referência para facilitar a sintaxe
            auto const& x_key = (*x)->key;

            // Se for igual sai do loop
            if (key == x_key)
            {
                break;
            }

            // Se for menor vai pra esquerda
            else if (key < x_key)
            {
                x = &((*x)->left);
            }

            // Se for maior vai para direita
            else if (key > x_key)
            {
                x = &((*x)->right);
            }
        }

        // Retorna o nó
        return x;
    }
};

// ####################################################
// # Árvore Vermelho-Preta
// ####################################################

enum TraversalOrder : unsigned char
{
    InOrder,
    PreOrder,
    PostOrder
};

template <class Key, class Value>
class RedBlackTree
{
    // ########################################################################
    // # Propriedades da Arvore Vermelho e Preta:
    // ########################################################################
    // #
    // #    1 - Cada nó é vermelho OU preto
    // #    2 - A raiz é preta
    // #    3 - Todas as folhas ("NULL" ou NIL) são pretas
    // #    4 - Se um nó for vermelho, então ambos os seus
    // #        filho são pretos
    // #    5 - Para cada nó, todos os caminhos do nó para
    // #        as folhas descendentes contém o mesmo número
    // #        de nós pretos
    // #
    // ########################################################################

private: // Atributos privados

    // Facilita a vida
    using Leaf = Node<Key, Value>;

    // Raíz da árvore
    Leaf * root;

    // Folha nil
    Leaf * nil;

private: // Métodos privados

    // ########################################################################
    // # Rotações
    // ########################################################################

    void left_rotate(Leaf * x)
    {
        // y é o nó à direita de x
        Leaf * y = x->right;

        // Insere o filho à esquerda de y como filho filho à direita de x
        x->right = y->left;
        if (y->left != this->nil)
        {
            y->left->parent = x;
        }

        // Insere y como filho do pai de x
        y->parent = x->parent;

        // Se x for a raíz
        if (x->parent == this->nil)
        {
            this->root = y;
        }

        // Se x for filho à esquerda
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }

        // Se x for filho à direita
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }

        // Insere x como filho à esquerda de y
        y->left = x;
        x->parent = y;
    }

    void right_rotate(Leaf * x)
    {
        // y é o nó à esquerda de x
        Leaf * y = x->left;

        // Insere o filho à direita de y como filho à esquerda de x
        x->left = y->right;
        if (y->right != this->nil)
        {
            y->right->parent = x;
        }

        // Insere y como filho do pai de x
        y->parent = x->parent;

        // Se x for a raíz
        if (x->parent == this->nil)
        {
            this->root = y;
        }

        // Se x for filho à esquerda
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }

        // Se x for filho à direita
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }

        // Insere x como filho à direita de y
        y->right = x;
        x->parent = y;
    }

    // #######################################
    // # Fix Up
    // #######################################
    void fix_up(Leaf * z)
    {
        while (z->parent->color == Color::Red)
        {
            // Se o pai de z for filho à esquerda do avô de z
            if (z->parent == z->parent->parent->left)
            {
                // y é o filho à direita do avô de z
                Leaf * y = z->parent->parent->right;

                // Se o tio de z for vermelho
                if (y->color == Color::Red)
                {
                    // O pai e o tio de z serão pretos
                    z->parent->color = y->color = Color::Black;

                    // O avô de z será vermelho
                    z->parent->parent->color = Color::Red;

                    // z sobre dois níveis na hierarquia
                    z = z->parent->parent;
                }

                // Se o tio de z for preto
                else if (y->color == Color::Black)
                {
                    // Se z for filho à direita gira a árvore
                    if (z == z->parent->right)
                    {
                        // z sobe um nível na hierarquia
                        z = z->parent;

                        // Gira à esquerda
                        this->left_rotate(z);
                    }

                    // Pai de z será preto
                    z->parent->color = Color::Black;

                    // Avô de z será vermelho
                    z->parent->parent->color = Color::Red;

                    // Gira à direita o avô de z
                    this->right_rotate(z->parent->parent);
                }
            }

            // Se o pai de z for filho à direita do avô de z
            else if (z->parent == z->parent->parent->right)
            {
                // y é o filho à esquerda do avô de z
                Leaf * y = z->parent->parent->left;

                // Se o tio de z for vermelho
                if (y->color == Color::Red)
                {
                    // O pai e o tio de z serão pretos
                    z->parent->color = y->color = Color::Black;

                    // O avô de z será vermelho
                    z->parent->parent->color = Color::Red;

                    // z sobre dois níveis na hierarquia
                    z = z->parent->parent;
                }

                // Se o tio de z for preto
                else if (y->color == Color::Black)
                {
                    // Se z for filho à esquerda
                    if (z == z->parent->left)
                    {
                        // z sobe um nível na hierarquia
                        z = z->parent;

                        // Gira à direita
                        this->right_rotate(z);
                    }

                    // Pai de z será preto
                    z->parent->color = Color::Black;

                    // Avô de z será vermelho
                    z->parent->parent->color = Color::Red;

                    // Gira à direita o avô de z
                    this->left_rotate(z->parent->parent);
                }
            }
        }

        // A raíz da árvore é sempre preta
        this->root->color = Color::Black;
    }

public: // Interface pública

    // #######################################
    // # Construtor
    // #######################################
    RedBlackTree():
        root(nullptr),
        nil(nullptr)
    {
        // Cria o nó nil
        this->nil = new Leaf(
            Key(), Value()
        );
        this->nil->color = Color::Black;

        // Seta a raíz como nil
        this->root = this->nil;
    }

    // #######################################
    // # Destrutor
    // #######################################
    ~RedBlackTree()
    {
        // Função para deletar recursivamente as folhas da árvore
        std::function<void(Leaf*)> kill = [&] (Leaf * leaf) -> void
        {
            if (leaf != this->nil)
            {
                kill(leaf->left);
                kill(leaf->right);
                delete leaf;
                leaf = nullptr;
            }
        };

        // Delete recursivamente a árvore
        kill(this->root);

        // Deleta a folha nil
        delete this->nil;
        this->nil = nullptr;
    }

    // #######################################
    // # Retorna a folha nil da árvore
    // #######################################
    Leaf const * const nil_leaf() const { return this->nil; }

    // #######################################
    // # Inserção
    // #######################################
    bool insert(Key key, Value value)
    {
        // Busca o pai do par <key, value> na árvore
        Leaf * y = this->nil;
        for (Leaf * x = this->root; x != this->nil; )
        {
            // Salva o último valor de x em y
            y = x;

            // Verifica qual será o próximo nó
            if (key < x->key)
            {
                x = x->left;
            }
            else if (key > x->key)
            {
                x = x->right;
            }

            // Se encontrou uma chave igual a que está
            // sendo inserida, retorna false porque não
            // pode haver duplicatas na árvore
            else return false;
        }

        // Se chegou até aqui, não será a toa alocar
        // memória para um novo nó
        Leaf * z = new Leaf(key, value);
        z->parent = y;
        z->left = this->nil;
        z->right = this->nil;

        // Se y for null, a árvore está vazia e z
        // é a raíz da árvore
        if (y == this->nil)
        {
            this->root = z;
        }

        // Se não, insere na posição correta em relação a y
        else if (z->key < y->key)
        {
            y->left = z;
        }

        else if (z->key > y->key)
        {
            y->right = z;
        }

        // Conserta a árvore e retorna true
        this->fix_up(z);
        return true;
    }

    // #######################################
    // # Busca na árvore
    // #######################################
    Leaf ** find(Key const& key)
    {
        // Começa a busca pela raíz
        return Leaf::find(
            &(this->root), &(this->nil), key
        );
    }

    // #######################################
    // # Travessia em ordem
    // #######################################
    template <typename Fn>
    void traversal(TraversalOrder traversal_order, Fn fn)
    {
        std::function<void(Leaf*, int)> traversal;
        switch (traversal_order)
        {
            case TraversalOrder::InOrder :
            {
                traversal = [&] (Leaf * leaf, int depth) -> void
                {
                    if (leaf != this->nil)
                    {
                        traversal(leaf->left, depth+1);
                        fn(leaf, depth);
                        traversal(leaf->right, depth+1);
                    }
                };

            } break;

            case TraversalOrder::PreOrder :
            {
                traversal = [&] (Leaf * leaf, int depth) -> void
                {
                    if (leaf != this->nil)
                    {
                        fn(leaf, depth);
                        traversal(leaf->left, depth+1);
                        traversal(leaf->right, depth+1);
                    }
                };

            } break;

            case TraversalOrder::PostOrder :
            {
                traversal = [&] (Leaf * leaf, int depth) -> void
                {
                    if (leaf != this->nil)
                    {
                        traversal(leaf->left, depth+1);
                        traversal(leaf->right, depth+1);
                        fn(leaf, depth);
                    }
                };

            } break;
        }
        traversal(this->root, 0);
    }
};

int main()
{
    // Salva os valores inseridos
    std::vector<int> v;

    // ######################################################
    // # Inserção da árvore
    // ######################################################

        // Cria a árvore vermelho e preta e insere elementos
        RedBlackTree<int, int> tree;
        std::cout << "Inserindo chaves na árvore ... \n\n";
        for (int i = 0; i < 10; i++)
        {
            int value = ::rand() % 100;
            std::cout << "      inserindo " << value << std::endl;
            tree.insert( value, i );
            v.push_back(value);
        }

    // ######################################################
    // # Travessia da árvore
    // ######################################################

        // Percorre em ordem
        std::cout << "\nPercorrendo em ordem ...\n\n";
        tree.traversal(TraversalOrder::InOrder, [] (Node<int, int> * node, int depth) -> void
        {
            for (int i = 0; i < depth; i++)
                std::cout << "     ";
            std::cout << "Nível " << depth << " = " << node->key << "\n\n";
        });

        // Percorre em pré ordem
        std::cout << "\nPercorrendo em pré-ordem ...\n\n";
        tree.traversal(TraversalOrder::PreOrder, [] (Node<int, int> * node, int depth) -> void
        {
            for (int i = 0; i < depth; i++)
                std::cout << "     ";
            std::cout << "Nível " << depth << " = " << node->key << "\n\n";
        });

        // Percorre em pós ordem
        std::cout << "\nPercorrendo em pós-ordem ...\n\n";
        tree.traversal(TraversalOrder::PostOrder, [] (Node<int, int> * node, int depth) -> void
        {
            for (int i = 0; i < depth; i++)
                std::cout << "     ";
            std::cout << "Nível " << depth << " = " << node->key << "\n\n";
        });

    // ######################################################
    // # Busca na árvore
    // ######################################################

        // Busca elementos na árvore
        std::cout << "\nBuscando elementos ....\n\n";
        v.push_back(999999);
        for (int i = 0; i < 11; i++)
        {
            auto node = tree.find( v.at(i) );
            if ((*node) != tree.nil_leaf())
            {
                std::cout << "      Encontrada a chave: " << (*node)->key << std::endl;
            }
            else
            {
                std::cout << "      Não encontrada a chave: " << v.at(i) << std::endl;
            }
        }

    return 0;
}
