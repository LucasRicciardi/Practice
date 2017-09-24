#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <time.h>

#define DO_NOTHING 0

typedef std::vector<int> Vec;
typedef std::function<bool(int,int)> Fn;

//  ########################################################################
//
//  Algoritmos genéricos que serão utilizados pelo programa
//
//  ########################################################################
namespace Algorithms
{
    /*
    *   Algoritmo de particionamento. Recebe um vetor v, dois inteiros a e b
    *   representando um intervalo número [a, b) e uma função de comparação
    *   f e retorna um inteiro n. Particiona o vetor de forma que todo valor
    *   a esquerda de n (i < n) é menor ou igual à que v[n] e todo valor
    *   a direita de n (i > n) é maior do que v[n]. O pivot escolhido é
    *   v[b-1] por default
    */
    int partition(Vec& v, int begin, int end, Fn fn)
    {
        int left_pointer = begin;
        int right_pointer = end-2;
        int& pivot = v.at(end-1);
        while (true)
        {
            #define value_at_left_is_smaller_than_pivot fn(v.at(left_pointer), pivot)
            while (value_at_left_is_smaller_than_pivot)
                left_pointer++;
            #undef value_at_left_is_smaller_than_pivot

            #define value_at_right_is_greater_than_pivot fn(pivot, v.at(right_pointer))
            while (value_at_right_is_greater_than_pivot and right_pointer > left_pointer)
                right_pointer--;
            #undef value_at_right_is_greater_than_pivot

            if (left_pointer < right_pointer)
                std::swap(v.at(left_pointer), v.at(right_pointer));
            else break;
        }
        std::swap(v.at(left_pointer), pivot);
        return left_pointer;
    }

    /*
    *   Recebe um vetor de inteiros v, dois inteiros a e b e uma
    *   função de comparação f. Transforma o vetor v no intervalo
    *   [a, b) em uma heap, ou seja, tal que se n pertence à
    *   [a+1, b+1), então v[(n/2)-1] > v[n-1]
    */
    void heapify(Vec& v, int begin, int end, Fn fn)
    {
        // Inicia a raíz como sendo o valor em begin
        int tree_size = (end-begin);
        int root = 1;
        while (root < tree_size)
        {
            // Compara os três nós da sub-árvore atual
            int old_root = root;
            int left_child = (root * 2) + 0;
            int right_child = (root * 2) + 1;

            #define child_is_greater_than_parent(child) fn(v.at(root+begin-1), v.at(child+begin-1))
            // Verifica se o nó esquerdo é o maior
            if (left_child <= tree_size and child_is_greater_than_parent(left_child))
                root = left_child;

            // Verifica se o nó direito é o maior
            if (right_child <= tree_size and child_is_greater_than_parent(right_child))
                root = right_child;
            #undef child_is_greater_than_parent

            // Verifica se é necessário mudar a estrutura da árvore
            if (root != old_root)
                std::swap(v.at(root+begin-1), v.at(old_root+begin-1));
            else break;
        }
    }

    /*
    *   Algoritmo de inserçao. Recebe um vetor v, dois inteiros a e b e uma
    *   função de comparação f. Assume que v esteja ordenado no intervalo
    *   [a, b-2] e insere o elemento na posição b-1 na posição correta de
    *   forma que v continue ordenado após a inserção
    */
    void insert(Vec& v, int begin, int end, Fn fn)
    {
        // Insere o elemento na posição correta no mantendo o vetor ordenado
        #define smaller_element_not_in_place fn(v.at(j), v.at(j-1))
        for (int j = end; j > begin and smaller_element_not_in_place; j--)
            std::swap(v.at(j), v.at(j-1));
        #undef smaller_element_not_in_place
    }

    /*
    *   Algoritmo de intercalação. Recbe um vetor v, dois inteiros a e b e uma
    *   função de comparação f. Divide v em dois vetores u = v[a, (a+b)/2) e
    *   w = v[(a+b)/2, b) e assume que u e v estejam ordenados. Intercala os
    *   vetores u e w em v de forma que v esteja ordenado
    */
    void merge(Vec& v, int begin, int mid, int end, Fn fn)
    {
        // Variáveis auxiliares
        int i, j, k;

        // Copia as duas metades nos vetores auxiliares
        std::vector<int> left(mid-begin);
        std::vector<int> right(end-mid);
        for (i = begin, j = mid; i < mid or j < end; i++, j++)
        {
            i < mid ? left.at(i-begin) = v.at(i) : DO_NOTHING;
            j < end ? right.at(j-mid) = v.at(j) : DO_NOTHING;
        }

        // Intercala os dois vetores de volta no original
        for (i = j = 0, k = begin; i < (mid-begin) && j < (end-mid); k++)
        {
            #define left_is_smaller_than_right fn(left.at(i), right.at(j))
            if (left_is_smaller_than_right)
                v.at(k) = left.at(i++);
            else
                v.at(k) = right.at(j++);
            #undef left_is_smaller_than_right
        }

        // Copia de volta o que ainda não foi copiado de um dos auxiliares
        while (i < (mid-begin))
            v.at(k++) = left.at(i++);
        while (j < (end-mid))
            v.at(k++) = right.at(j++);
    }

    /*
    *   Idém ao algoritmo acima mas sem o uso de vetores auxiliares
    */
    void merge_in_loco(Vec& v, int begin, int mid, int end, Fn fn)
    {
        // Intercala os vetores
        for (int i = begin, j = mid, k = begin; i < j and j < end; k++)
        {
            #define left_is_smaller_than_right fn(v.at(i), v.at(j))
            if (left_is_smaller_than_right)
                i++;
            else
                Algorithms::insert(v, i, j++, fn);
            #undef left_is_smaller_than_right
        }
    }
}

// ########################################################################
//
//   -- ALGORITMOS DE ORDENAÇÃO
//
//   Interface de um algoritmo de ordenação, armazena o número de chamadas
//   e o tempo que cada chamada levou para executar. Cada algoritmo de
//   ordenação deve receber um vetor de inteiros v, dois inteiros a
//   e b e uma função p que recebe dois inteiros a e b e retorna bool.
//   O algoritmo ordena o vetor v de acordo com a ordem imposta por p
//   no intervao [a, b), ou seja, fechado em a e aberto em b.
//   Logo, 0 <= a <= b < TAMANHO(v) é a condição para que o algoritmo
//   funcione corretamente.
//
// ########################################################################

class SortingAlgorithm
{
private:
    std::vector<double> _calls_list;
    double _last_call_time;
    std::string _name;

protected:
    virtual void _sort(Vec&, int, int, Fn) = 0;

public:
    SortingAlgorithm(std::string name):
        _calls_list(),
        _last_call_time(0),
        _name(name)
    {}

    virtual ~SortingAlgorithm() {}

    void sort(Vec& v, int begin, int end, Fn fn)
    {
        clock_t t0 = ::clock();
        this->_sort(v, begin, end, fn);
        clock_t tf = ::clock();
        this->_last_call_time = (double) (tf-t0) / CLOCKS_PER_SEC;
        this->_calls_list.push_back(this->last_call_time());
    }

    std::string name() const { return this->_name; }

    double last_call_time() const { return this->_last_call_time; }

    double average_time() const
    {
        double average = 0;
        for (uint32_t i = 0; i < this->_calls_list.size(); i++)
            average += this->_calls_list.at(i) / this->_calls_list.size();
        return average;
    }

    void log() const
    {
        printf("\n");
        printf("Nome: %s\n", this->name().c_str());
        printf("Chamda %d chamada: %.4fs\n", (int)this->_calls_list.size(), this->last_call_time());
        printf("Média atual: %.4fs\n", this->average_time());
    }
};

// ########################################################################
//
//   Implementação do algoritmo Merge Sort
//
// ########################################################################
class MergeSort:
    public SortingAlgorithm
{
private:
    typedef std::function<void(Vec&, int, int, int, Fn)> Type;
    MergeSort::Type _merge_type;

public:
    MergeSort(MergeSort::Type merge_type = Algorithms::merge, std::string name = "Merge Sort"):
        SortingAlgorithm(name),
        _merge_type(merge_type)
    {}

protected:
    void _sort(Vec& v, int begin, int end, Fn fn)
    {
        // Divide o vetor
        int mid = begin + ((end-begin) / 2);
        if ((end-begin) > 1)
        {
            this->_sort(v, begin, mid, fn);
            this->_sort(v, mid, end, fn);
        }
        // Intercala os vetores w = v[begin, mid) e u = v[mid, end)
        this->_merge_type(v, begin, mid, end, fn);
    }
};

class MergeSortInLoco:
    public MergeSort
{
public:
    MergeSortInLoco():
        MergeSort(Algorithms::merge_in_loco, "Merge Sort In Loco")
    {}
};

// ########################################################################
//
//   Implementação do algoritmo Quick Sort
//
// ########################################################################
class QuickSort:
    public SortingAlgorithm
{
public:
    QuickSort():
        SortingAlgorithm("Quick Sort")
    {}

protected:
    void _sort(Vec& v, int begin, int end, Fn fn)
    {
        // Verifica se o vetor contém ao menos 2 elementos
        if ((end-begin) > 1)
        {
            // Particiona o vetor e descobre o elemento do 'meio' da partição
            int new_pivot = Algorithms::partition(v, begin, end, fn);

            // Particiona as outras duas metades recursivamente
            this->_sort(v, begin, new_pivot, fn);
            this->_sort(v, new_pivot+1, end, fn);
        }
    }
};
// ########################################################################
//
//  Implementação do algoritmo Heap Sort
//
// ########################################################################
class HeapSort:
    public SortingAlgorithm
{
public:
    HeapSort():
        SortingAlgorithm("Heap Sort")
    {}

protected:
    void _sort(Vec& v, int begin, int end, Fn fn)
    {
        // Transforma o vetor em uma maxheap
        for (int i = (end/2); i > 0; i--)
            Algorithms::heapify(v, i-1, end, fn);

        for (int i = end; i > begin; i--)
        {
            // Troca o valor no topo da heap com o último elemento
            std::swap(v.at(begin), v.at(i-1));

            // Insere o valor do topo na posição correta da heap
            Algorithms::heapify(v, begin, i-2, fn);
        }
    }
};

// ########################################################################
//
//   Implementação do algoritmo Bubble Sort
//
// ########################################################################
class BubbleSort:
    public SortingAlgorithm
{
public:
    BubbleSort():
        SortingAlgorithm("Bubble Sort")
    {}

protected:
    void _sort(Vec& v, int begin, int end, Fn fn)
    {
        // A cada iteração deste loop, o final do vetor está ordenado
        for (int i = begin; i < (end-1); i++)
        {
            // Calcula quantos elementos, do fim ao começo, já estão ordenados
            int last = begin > 0 ? i % begin : 0;

            // Percorre novamente o vetor até o último elemento ordenado
            // enquanto 'borbulha' o maior elemento do vetor desordenado até lá
            for (int j = begin; j < (end-last-1); j++)
            {
                #define swap_required fn(v.at(j+1), v.at(j))
                if (swap_required)
                    std::swap(v.at(j+1), v.at(j));
                #undef swap_required
            }
        }
    }
};

// ########################################################################
//
//   Implementação do algoritmo Insertion Sort
//
// ########################################################################
class InsertionSort:
    public SortingAlgorithm
{
public:
    InsertionSort():
        SortingAlgorithm("Insertion Sort")
    {}

protected:
    void _sort(Vec& v, int begin, int end, Fn fn)
    {
        // A cada iteração do loop, o início do vetor está ordenado,
        // insere então o novo elemento na posição correta
        for (int i = begin; i < end; i++)
            Algorithms::insert(v, begin, i, fn);
    }
};

// ##################################################################
//
//   Menu de interface do usuário
//
// ##################################################################
class UserMenu
{
private:
    Vec v;

public:
    UserMenu():
        v()
    {}

    void show_menu()
    {
        std::string author = "Lucas Ricciardi de Salles";
        std::vector< std::string > message(5);
        message.at(0) = "Selecione uma das opções: \n";

        // Letra a)
        message.at(1) = "1 - 'a-) Fazer programa que grava N números aleatórios um por linha. \
        \n             O programa deve receber um parâmetro N via linha de comando ou utilizar 10000 como padrão.'\n";

        // Letra b)
        message.at(2) = "2 - 'b-) Fazer programa que lê os números de um arquivo e grava \
        \n             em um vetor na memória.'\n";

        // Letra c)
        message.at(3) = "3 - 'c-) Implemente a classificação crescente por seleção \
        \n             no vetor carregado e salve a saída em um arquivo.'\n";

        // Letra d)
        message.at(4) = "4 - 'd-) Realiza a medida de tempo computacional para sua rotina \
        \n             de ordenação e imprima na tela o tempo para a ordenação médio.'\n";

        // Exibe o menu
        printf("\nNome do autor: %s\n", author.c_str());
        for (uint32_t i = 0; i < message.size(); i++)
            printf("%s\n", message.at(i).c_str());
    }

    void option_one()
    {
        printf("\nCaso queira fazer como no enunciado, feche o programa e passe como argumento, \
        \ncaso contrário, digite um novo valor que o vetor atual será descartado e um novo será criado !\n\n");
        int n = get_user_input();
        this->create_vector(n);
        printf("\nVetor com %d elementos criados !\n\n", n);
    }

    void option_two()
    {
        printf("\nO vetor atual será descartado e um vetor será lido do arquivo 'file.txt' incluido nesta pasta\n\n");
        this->v.clear();
        FILE * file = fopen("file.txt", "a+");
        int n; char c;
        while (fscanf(file, "%d%c", &n, &c) == 2)
            this->v.push_back(n);
        fflush(file);
        fclose(file);
    }

    void option_three()
    {
        printf("\nO Vetor atual será ordenado e salvo no arquivo 'file.txt'\n\n");
        SortingAlgorithm * s = this->get_sorting_algorithm();
        s->sort(this->v, 0, this->v.size(), [] (int const& a, int const& b)
        {
            return a < b;
        });
        s->log();
        FILE * file = fopen("file.txt", "w+");
        for (uint32_t i = 0; i < this->v.size(); i++)
            fprintf(file, "%d\n", this->v.at(i));
        fflush(file);
        fclose(file);
        delete s;
    }

    void option_four()
    {
        SortingAlgorithm * s = this->get_sorting_algorithm();
        printf("\n Quantas chamadas deseja realizar ?\n\n");
        int n = this->get_user_input();
        printf("\nA cada chamada o vetor será embaralhado usando o algoritmo de \
        particionamento e a função ::rand() para que a ordenação tenha efeito\n\n");
        while (n --> 0)
        {
            // Ordena
            s->sort(this->v, 0, this->v.size(), [] (int const& a, int const& b)
            {
                return a < b;
            });

            // Embaralha
            Algorithms::partition(this->v, 0, this->v.size(), [] (int const&, int const&)
            {
                ::srand(::clock());
                return ::rand() % 2;
            });

            // Mostra o log
            s->log();
        }
        delete s;
    }

    SortingAlgorithm * get_sorting_algorithm()
    {
        printf("\nEscolha um dos algoritmos de ordenação: \n\
        ---> 1) Merge Sort \n\
        ---> 2) Merge Sort In Loco\n\
        ---> 3) Quick Sort\n\
        ---> 4) Heap Sort\n\
        ---> 5) Insertion Sort\n\
        ---> 6) Bubble Sort\n\n");
        int n = this->get_user_input();
        switch (n)
        {
            case 1: return new MergeSort;
            case 2: return new MergeSortInLoco;
            case 3: return new QuickSort;
            case 4: return new HeapSort;
            case 5: return new InsertionSort;
            case 6: return new BubbleSort;
            default: return new MergeSort;
        }
    }

    int get_user_input()
    {
        int n; scanf("%d", &n); getchar();
        return n;
    }

    void create_vector(int n)
    {
        v.clear();
        for (int i = 0; i < n; i++)
        {
            this->v.push_back(::rand());
            ::srand(::clock());
        }
    }
};

void print_vector(std::vector<int> const& v)
{
    for (uint32_t i = 0; i < v.size(); i++)
        printf("%d ", v.at(i));
    printf("\n");
}

int main(int argc, char * argv[])
{
    UserMenu menu;
    if (argc > 1)
    {
        int n = ::atoi(argv[1]);
        menu.create_vector(n);
        printf("\nVetor com %d elementos criados !\n\n", n);
    }

    while (true)
    {
        menu.show_menu();
        int choice = menu.get_user_input();
        bool done = false;
        switch (choice)
        {
            case 1: menu.option_one(); break;
            case 2: menu.option_two(); break;
            case 3: menu.option_three(); break;
            case 4: menu.option_four(); break;
            default: done = true;
        }
        if (done) break;
    }
    return 0;
}
