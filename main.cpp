// Nomes:
//      Felipe Oliveira Bueno
//      Lucas Ricciardi de Salles
//      Matheus de Almeida Souza 

// Requer: C++ 11
// g++ maquina_estado_planta_controlador.cpp -o maquina_estado_planta_controlador -std=c++11

#include <iostream>
#include <vector>
#include <algorithm>

#define Input double

template <class State, class Output>
class StateMachine
{
protected:
    State state_;
    const State start_state;

private:
    void start()
    {
        this->state_ = this->start_state;
    }

    Output step(Input const& input)
    {
        std::pair<State, Output> const& res = this->getNextValues(this->state_, input);        
        return (this->state_ = res.first, res.second);
    }

public:
    StateMachine(State s):
    state_(s),
    start_state(s)
    {
        // Máquina de Estado
    }

    virtual ~StateMachine()
    {
        // Destrutor
    }

    State getState() const { return this->state_; }

    std::vector<Output> transduce(std::vector<Input> const& input)
    {
        this->start();
        std::vector<Output> output;
        std::for_each(input.begin(), input.end(), [&] (Input const& x)
        {
            Output res = this->step(x);
            output.push_back(res);
        });
        return output;
    }

    virtual State getNextState(State const& state, Input const& /* input */ )
    {
        return state;
    }

    virtual std::pair<State, Output> getNextValues(State const& state , Input const& input ) 
    { 
        return { this->getNextState(state, input), Output() }; 
    }
};

#define State std::pair<double, double>
#define SubState double 
#define Output double
class Cascade: public StateMachine<State, Output>
{
private:
    StateMachine<SubState, Output> * m1;
    StateMachine<SubState, Output> * m2;

public:
    Cascade(StateMachine<SubState, Output> * sm1, StateMachine<SubState, Output> * sm2):
    StateMachine( { sm1->getState(), sm2->getState() }),
    m1(sm1),
    m2(sm2)
    {

    }

    Cascade(Cascade const&) = delete;
    Cascade(Cascade&&) = delete;
    Cascade& operator=(Cascade) = delete;

    std::pair<State, Output> getNextValues(State const& state, Input const& input)
    {
        std::pair<SubState, Output> const& r1 = m1->getNextValues(state.first, input);
        std::pair<SubState, Output> const& r2 = m2->getNextValues(state.second, r1.second);
        return 
        {
            { r1.first, r2.first },
            r2.second
        };
    }
};
#undef State
#undef SubState
#undef Output

#define State std::pair<double, double>
#define Output double
class Feedback: public StateMachine<State, Output>
{
private:
    StateMachine<State, Output> * m;

public:
    Feedback(StateMachine<State, Output> * sm):
    StateMachine(sm->getState()),
    m(sm)
    {
        // Feedback
    }

    Feedback(Feedback const&) = delete;
    Feedback(Feedback&&) = delete;
    Feedback& operator=(Feedback) = delete;

    std::pair<State, Output> getNextValues(State const& state, Input const& input)
    {   
        std::pair<State, Output> const& r1 = m->getNextValues(state, input);
        std::pair<State, Output> const& r2 = m->getNextValues(state, r1.second);
        
        return { r2.first, r1.second }; 
    }
};
#undef State
#undef Output

#define A double
#define B double
class WallController:  public StateMachine<A, B>
{
private:
    const A k;
    const A d;

public:
    WallController(A k_ = -1.5f, A d_ = 1.0f, A state = 0.0f):
    StateMachine(state),
    k(k_),
    d(d_)
    {
        // WallController
    }

    std::pair<A, B> getNextValues(A const&, Input const& dt)
    {
        A res = k * (d - dt);
        return { res, res };
    }
};
#undef A
#undef B

#define A double
#define B double
class WallWorld: public StateMachine<A, B>
{
private:
    const A dt;

public:
    WallWorld(A dt_ = 0.1f, A state = 5.0f):
    StateMachine(state),
    dt(dt_)
    {
        // WallWorld
    }

    std::pair<A, B> getNextValues(A const& t, Input const& n)
    {
        return { t - dt * n, t };
    }
};
#undef A
#undef B

template <typename T>
void show_output(std::vector<T> v)
{
    std::cout << std::endl;
    std::for_each(v.begin(), v.end(), [] (T const& t)
    {
        std::cout << "Output " << t << std::endl;
    });
    std::cout << std::endl;
}

int main()
{
    std::vector<Input> v;
    for (int i = 0; i++ < 30; v.push_back((double) i));

    Feedback robot(
        new Cascade(
            new WallController, new WallWorld
            )
        );
    show_output(robot.transduce(v));

    return 0;
}