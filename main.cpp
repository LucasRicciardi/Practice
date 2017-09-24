#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
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

            // Verifica se o nó esquerdo é o maior
            #define child_is_greater_than_parent(child) fn(v.at(root+begin-1), v.at(child+begin-1))
            if (left_child <= tree_size and child_is_greater_than_parent(left_child))
                root = left_child;

            // Verifica se o nó esquerdo é o maior
            if (right_child <= tree_size and child_is_greater_than_parent(right_child))
                root = right_child;

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

protected:
    virtual void _sort(Vec&, int, int, Fn) = 0;

public:
    SortingAlgorithm():
        _calls_list(),
        _last_call_time(0)
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

    double last_call_time() const { return this->_last_call_time; }

    int average_time() const
    {
        int average = 0;
        for (uint32_t i = 0; i < this->_calls_list.size(); i++)
            average += (this->_calls_list.at(i) - average) / this->_calls_list.size();
        return average;
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
    MergeSort(MergeSort::Type merge_type = Algorithms::merge):
        SortingAlgorithm(),
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
        MergeSort(Algorithms::merge_in_loco)
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
        SortingAlgorithm()
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
        SortingAlgorithm()
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
        SortingAlgorithm()
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
        SortingAlgorithm()
    {}

protected:
    void _sort(Vec& v, int begin, int end, Fn fn)
    {
        // A cada iteração do loop, o início do vetor está ordenado
        for (int i = begin; i < end; i++)
        {
            // Insere o maior elemento na posição correta
            Algorithms::insert(v, begin, i, fn);
        }
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

public:
    void show_menu()
    {
        std::string author = "Lucas Ricciardi de Salles";
        std::vector< std::string > message(5);
        message.at(0) = "Selecione uma das opções: \n";

        // Letra a)
        message.at(1) = "1 - 'a-) Fazer programa que grava N números aleatórios um por linha. \
        O programa deve receber um parâmetro N via linha de comando ou utilizar 10000 como padrão.'";

        // Letra b)
        message.at(2) = "2 - 'b-) Fazer programa que lê os números de um arquivo e grava \
        em um vetor na memória.'";

        // Letra c)
        message.at(3) = "3 - 'c-) Implemente a classificação crescente por seleção \
        no vetor carregado e salve a saída em um arquivo.'";

        // Letra d)
        message.at(4) = "4 - 'd-) Realiza a medida de tempo computacional para sua rotina \
        de ordenação e imprima na tela o tempo para a ordenação médio.'";

        // Exibe o menu
        printf("%s\n", author.c_str());
        for (uint32_t i = 0; i < message.size(); i++)
            printf("%s\n", message.at(i).c_str());
    }
};

void print_vector(std::vector<int> const& v)
{
    for (uint32_t i = 0; i < v.size(); i++)
        printf("%d ", v.at(i));
    printf("\n");
}

int main()
{
    SortingAlgorithm * algorithm = new HeapSort;

    std::vector<int> v;
    for (int i = 9; i > -1; i--)
        v.push_back(i);
    print_vector(v);
    algorithm->sort(v, 0, v.size(), [] (int const& a, int const& b)
    {
        return a < b;
    });
    print_vector(v);
    return 0;
}
