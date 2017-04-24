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

int main()
{
    int entrada[] = { 5, 3, -2, 4, 1 };
    const int tamanho = 5;

    float ** saida;
    saida = (float **) malloc(5 * sizeof(float *));
    for (int i = 0; i < 6; i++)
    {
        saida[i] = (float *) malloc(1 * sizeof(float));
    }

    // Acumulador
    Acumulador acm1, acm2(10);

    acm1.transdutor(entrada, tamanho, saida);
    acm1.imprime();

    acm2.transdutor(entrada, tamanho, saida);
    acm2.imprime();

    // Incrementador
    Incrementador icm1, icm2(10);

    icm1.transdutor(entrada, tamanho, saida);
    icm1.imprime();

    icm2.transdutor(entrada, tamanho, saida);
    icm2.imprime();

    // UpDown
    UpDown upd1, upd2(10);

    upd1.transdutor(entrada, tamanho, saida);
    upd1.imprime();

    upd2.transdutor(entrada, tamanho, saida);
    upd2.imprime();

    // Media2
    Media2 md1, md2(10);

    md1.transdutor(entrada, tamanho, saida);
    md1.imprime();

    md2.transdutor(entrada, tamanho, saida);
    md2.imprime();

    // Delay
    Delay de1, de2(10);

    de1.transdutor(entrada, tamanho, saida);
    de1.imprime();

    de2.transdutor(entrada, tamanho, saida);
    de2.imprime();

    // Porta
    Porta p;

    // Tamanho 16
    int entrada_porta[] = { 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0 };
    int tamanho_porta = 16;
    float ** saida_porta = (float **) malloc(16 * sizeof(float *));
    for (int i = 0; i < 16; i++)
    {
        saida_porta[i] = (float *) malloc(1 * sizeof(float));
    }

    p.transdutor(entrada_porta, tamanho_porta, saida_porta);
    p.imprime();

    return 0;
}


