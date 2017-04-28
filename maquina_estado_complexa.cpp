//       Nome: Lucas Ricciardi de Salles
//    Assunto: Máquinas de Estado
// Disciplina: Programação Orientada à Objetos
//       Prof: Maurício Marengoni

#include <iostream>
#include <cstdlib>

class MaquinaEstado
{
protected:
    int estado;
    std::string name;

public:
    MaquinaEstado():
    estado(0)
    {
    }

    // Params:
    //    int entrada -> Inteiro representando um valor de entrada.
    //    int posicao -> Posição que o valor de saída deve estar no vetor de saída.
    // float ** saida -> Vetor de 'floats' para registrar a saída na posição 'posicao'. 
    // Descrição:
    //  Esta função realiza um processo na máquina e atualiza o estado atuaç dela 
    //  ao final do processo.
    void etapa(int entrada, int posicao, float ** saida)
    {
        estado = proximoValor(entrada, posicao, saida);
    }

    // Params:
    //   int * entrada -> Vetor de inteiros representado 
    //                    os elementos à serem processados
    //                    pela máquina.
    //   int   tamanho -> Inteiro contendo o número de elementos
    //                    presentes no vetor entrada.
    //  float ** saida -> Vetor de 'floats' para armazenar às saídas
    //                    computadas a cada iteração da máquina.
    // Descrição:
    //   Esta função itera sobre um conjunto de entrada e processa
    //   as entradas uma a uma, atualizando no processo o estado
    //   da máquina e também registra a saída para cada entrada.
    void transdutor(int * entrada, int tamanho, float ** saida)
    {
        std::cout << "\nMaquina: " << this->name << std::endl;
        std::cout << "Estado Inicial: " << this->estado << std::endl;
        for (int i = 0; i < tamanho; i++)
        {
            etapa(entrada[i], i, saida);
            std::cout << "Chamada " << (i+1) << " Entrada "; 
            std::cout << entrada[i] << " Saida " << saida[i][0];
            std::cout << " Estado atual " << this->estado << std::endl;
        }
    }

    void imprime()
    {
        std::cout << "Estado final: " << estado << std::endl;
    }

    // Funções à serem sobreescritas em classes que herdam de 'MaquinaEstado'.
    
    // Params:
    //    int entrada -> Inteiro representando um valor de entrada.
    //    int posicao -> Posição que o valor de saída deve estar no vetor de saída.
    // float ** saida -> Vetor de 'floats' para registrar a saída na posição 'posicao'. 
    // Descrição:
    //  Esta função representa a 'função' da máquina e, portanto, varia para cada 
    //  diferente implementação da mesma.
    virtual int proximoValor(int entrada, int posicao, float ** saida) { return 0; }
    
    // Descrição:
    //  Retorna a máquina ao seu estado inicial.
    virtual int estadoInicial() const { return 0; }
};

class Acumulador: public MaquinaEstado
{
private:
    int estadoInicial_;

public:
    Acumulador(int es = 0):estadoInicial_(es) { estado = estadoInicial_; name = "Acumulador"; }

    // Descrição:
    //  A cada chamada, a função acumula a nova entrada junto à entradas anteriores
    //  e atualiza o estado atual da máquina para o valor acumulado. A saída também
    //  é igual ao valor acumulado, esta função pode ser descrita como:
    //  d(qi, a) = d((qi+a), (qi+a))
    int proximoValor(int entrada, int posicao, float ** saida)
    {
        return saida[posicao][0] = estado += entrada;
    }

    inline int estadoInicial() const { return estadoInicial_; }

    inline void reset() { estado = estadoInicial_; }
};

class Incrementador: public MaquinaEstado
{
private:
    int estadoInicial_;

public:
    Incrementador(int es = 0): estadoInicial_(es) { estado = estadoInicial_;  name = "Incrementador"; }

    // Descrição:
    //  A cada chamada, incrementa o estado atual de máquina em uma unidade, de forma 
    //  que se forem feitas n chamadas e o estado inicial for zero, o valor do estado
    //  será de n*i, onde i é a unidade de incremento. Pode ser descrita como:
    //  d(qi, a) = d((qi + i), e), onde e é um valor opcional de saída. 
    int proximoValor(int entrada, int posicao, float ** saida) { return ++estado; }

    inline int estadoInicial() const { return estadoInicial_; }

    inline void reset() { estado = estadoInicial_; }
};

class UpDown: public MaquinaEstado
{
private:
    int estadoInicial_;

public:
    UpDown(int es = 0): estadoInicial_(es) { estado = estadoInicial_;  name = "UpDown"; }

    // Descrição:
    // A cada chamada, incrementa ou decrementa o estado atual da máquina em uma unidade,
    // de forma que se a entrada for menor do que 0 (a < 0), a máquina decrementa o estado atual,
    // e caso contrário (a >= 0) incrementa o valor da máquina. Pode ser descrita como:
    // d(qi, a) = {
    //              ((qi - 1), e) se a < 0
    //              ((qi + 1), e) se a >= 0
    //            } onde e é um valor opcional de saída.
    int proximoValor(int entrada, int posicao, float ** saida)
    {
        saida[posicao][0] = estado;

        if (entrada < 0) { return --estado; }
        else             { return ++estado; }
    }

    inline int estadoInicial() const { return estadoInicial_; }

    inline void reset() { estado = estadoInicial_; }
};

class Media2: public MaquinaEstado
{
private:
    int estadoInicial_;

public:
    Media2(int es = 0): estadoInicial_(es) { estado = estadoInicial_; name = "Media2"; }

    // Descrição:
    //  A cada chamada, a saída será a média entre o estado anterior e a entrada e o
    //  estado atual será a entrada. Pode ser descrita como:
    //  d(qi, a) = (a, ((qi + a) / 2))
    int proximoValor(int entrada, int posicao, float ** saida)
    {
        saida[posicao][0] = (estado / 2) + (entrada / 2);
        return entrada;
    }

    int estadoInicial() { return   estadoInicial_; }
    void reset()        { estado = estadoInicial_; }
};

class Delay: public MaquinaEstado
{
private:
    int estadoInicial_;

public:
    Delay(int es = 0): estadoInicial_(es) { estado = estadoInicial_; name = "Delay"; }

    // Descrição:
    //  A cada chamada, a função tem como saída o estado atual da máquina e o 
    //  próximo estado será a entrada. Pode ser descrita como:
    //  d(qi, a) = (a, qi)
    int proximoValor(int entrada, int posicao, float ** saida)
    {
        // A saída atual é o estado anterior.
        saida[posicao][0] = estado;

        // Atualiza o estado atual para a última entrada recebida.
        return entrada;
    }

    inline int estadoInicial() const { return estadoInicial_; }

    inline void reset() { estado = estadoInicial_; }
};

class Porta: public MaquinaEstado
{
private:
    int estadoInicial_;

public:
    Porta(int es = 0): estadoInicial_(es) { estado = estadoInicial_; name = "Porta"; }

    #define pessoa_na_porta  1
    #define ninguem_na_porta 0

    #define fechada  0
    #define aberta   1
    #define fechando 2
    #define abrindo  3

    #define porta estado

    // Implementação da porta automática.
    // Os estados da máquina são:
    //      { ABERTA, FECHADA, ABRINDO, FECHANDO }
    // As entradas da máquina são:
    //      { Pessoa na Porta, Ninguém na Porta }
    // As saídas da máquina são:
    //      d(fechada, pessoa na porta)   = abrindo
    //      d(abrindo, pessoa na porta)   = aberta
    //      d(aberta, niguem na porta)    = fechando
    //      d(fechando, ninguem na porta) = fechada
    //      Qualquer outra entrada        = estado atual 
    int proximoValor(int entrada, int posicao, float ** saida)
    {
        if (porta == fechada and entrada == pessoa_na_porta)
        {
            return saida[posicao][0] = abrindo;            
        }

        else if (porta == abrindo and entrada == pessoa_na_porta)
        {
            return saida[posicao][0] = aberta;
        }

        else if (porta == aberta and entrada == ninguem_na_porta)
        {
            return saida[posicao][0] = fechando;
        }

        else if (porta == fechando and entrada == ninguem_na_porta)
        {
            return saida[posicao][0] = fechada;
        }

        else
        {
            return saida[posicao][0] = porta;
        }
    }

    #undef pessoa_na_porta
    #undef ninguem_na_porta

    #undef fechada
    #undef aberta
    #undef fechando
    #undef abrindo

    #undef porta

    inline int estadoInicial() const { return estadoInicial_; }

    inline void reset() { estado = estadoInicial_; }
};

// Combina duas máquinas de estado em série, a entrada
// de uma máquina Cascade é a entrada de uma máquina de estado
// m1, a saída da máquina de estado m1 será a entrada de uma
// máaquina de estado m2 e a saída da máquina de estado m2 será a saida
// da Cascade
class Cascade: public MaquinaEstado
{
private:
    MaquinaEstado * m1;
    MaquinaEstado * m2;

    int estadoInicial_;

public:
    Cascade(MaquinaEstado * m1_, MaquinaEstado * m2_, int es = 0):
    m1(m1_),
    m2(m2_),
    estadoInicial_(es)
    {
        this->estado = es;
        this->name = "Cascade";
    } 

    ~Cascade()
    {
        delete m1;
        m1 = NULL;
        
        delete m2;
        m2 = NULL;
    }

    // Descrição:
    //  A máquina 'Cascade' combina duas máquinas de estado
    //  de maneira serial, ou seja, a saída da primeira é a 
    //  entrada da segunda, e a saída da segunda é a saída da 
    //  'Cascade'. O estado da 'Cascade' pode ser uma combinação
    //  dos estados das máquinas internas ou um estado único separado,
    //  depende da implementação, neste caso o estado da 'Cascade' é
    //  o estado da segunda máquina.  
    int proximoValor(int entrada, int posicao, float ** saida)
    {
        return 
        (
            m1->proximoValor(entrada, posicao, saida),
            m2->proximoValor(saida[posicao][0], posicao, saida)
        );
    }

    int estadoInicial() const { return this->estadoInicial_; }
    void reset() { estado = estadoInicial_; }
};

class Paralelo: public MaquinaEstado
{
private:
    MaquinaEstado * m1;
    MaquinaEstado * m2;

    int estadoInicial_;

public:
    Paralelo(MaquinaEstado * m1_, MaquinaEstado * m2_, int es = 0):
    m1(m1_),
    m2(m2_),
    estadoInicial_(es)
    {
        this->estado = es;
        this->name = "Paralelo";
    } 

    ~Paralelo()
    {
        delete m1;
        m1 = NULL;
        delete m2;
        m2 = NULL;
    }

    // Descrição:
    //  A máquina 'Paralelo' tem funcionamento muito parecido com a 'Cascade'.
    //  A diferença é que, enquanto na 'Cascade' uma máquina alimenta a outra,
    //  na 'Paralelo' a mesma entrada é usada nas duas máquinas, que operam de forma
    //  independente.
    int proximoValor(int entrada, int posicao, float ** saida)
    {
        return 
        (
            m1->proximoValor(entrada, posicao, saida),
            m2->proximoValor(entrada, posicao, saida)
        );
    }

    int estadoInicial() const { return this->estadoInicial_; }
    void reset() { estado = estadoInicial_; }
};

// Main com exemplos de uso e algumas combinações
int main()
{
    float ** saida = new float * [10];
    int * entrada = new int[10]; 
    int tamanho_vetor = 10;

    // Porta automática que conta quantas pessoas passaram
    Cascade porta_automatica(new Porta, new Incrementador);
    for (int i = 0; i < tamanho_vetor; i++)
    { 
        entrada[i] = ::rand() % 2; 
        saida[i] = new float[10];
    }
    porta_automatica.transdutor(entrada, tamanho_vetor, saida);
    porta_automatica.imprime();

    // Objeto que acumula valores e conta quantos objetos inteiros foram acumulados
    Paralelo acumulador_incrementador(new Acumulador, new Incrementador);
    for (int i = 0; i < tamanho_vetor; i++)
    {
        entrada[i] = ::rand() % 100;
    }
    acumulador_incrementador.transdutor(entrada, tamanho_vetor, saida);
    acumulador_incrementador.imprime();

    // Trilho que move-se para esquerda ou para direita de acordo com o input e armazena a posição atual
    Cascade trilhos(new UpDown, new Delay);
    for (int i = 0; i < tamanho_vetor; i++)
    {
        entrada[i] *= i & 2 ? -1: 1;
    }
    trilhos.transdutor(entrada, tamanho_vetor, saida);
    trilhos.imprime();

    // Objeto formado por duas máquinas complexas
    Paralelo objeto_complexo(
        new Cascade(new Incrementador, new Media2), 
        new Paralelo(new Delay, new UpDown)
        );
    objeto_complexo.transdutor(entrada, tamanho_vetor, saida);
    objeto_complexo.imprime();

    for (int i = 0; i < tamanho_vetor; i++) { delete saida[i]; }
    delete saida;
    delete entrada;
}