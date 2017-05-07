#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <cassert>
#include <cstdint>

template <class State, class Output, class Input>
class StateMachine
{
protected:
    State state_;
    const State first_state_;

private:
    void start() { this->state_ = this->first_state_; }

public:    
    StateMachine(State state):
    state_(state),
    first_state_(state)
    {

    }

    virtual ~StateMachine()
    {

    }

    std::vector<Output> transduce(std::vector<Input> const& input, 
        std::function<std::pair<State, Output>(State const&, Input const&)> foo = nullptr)
    {
        this->start();
        std::vector<Output> output(input.size());
        uint64_t i = 0;
        std::for_each(input.begin(), input.end(), [&] (Input const& input_)
        {
            std::pair<State, Output> const& r =
                foo ? foo(this->state(), input_) : this->nextValue(this->state(), input_);
            this->state_ = r.first;
            output.at(i++) = r.second;
        });
        return output;
    }

    virtual std::pair<State, Output> nextValue(State const& /* state */, Input const& /* input */)
    {
        return  { this->state() , Output() };
    }

    State state() const { return this->state_; }
};

// ###################################################################
// # Acumulador
// ###################################################################

class Accumulator:
    public StateMachine<double, double, double>
{
    using State = double;
    using Output = double;
    using Input = double;

public:
    Accumulator(State state):
    StateMachine<State, Output, Input>(state)
    {

    }

    std::pair<State, Output> nextValue(State const& state, Input const& input)
    {
        return { state + input, state + input };
    }
};

// ###################################################################
// # Incrementador
// ###################################################################

class Incrementer:
    public StateMachine<double, double, double>
{
    using State = double;
    using Output = double;
    using Input = double;

public:
    Incrementer(State state):
    StateMachine<State, Output, Input>(state)
    {

    }

    std::pair<State, Output> nextValue(State const& state, Input const& /* input */ )
    {
        return { state + 1, state + 1 };
    }      
};

// ###################################################################
// # UpDown
// ###################################################################

class UpDown: public StateMachine<double, double, double>
{
    using State = double;
    using Output = double;
    using Input = double;

public:
    UpDown(State state = 0):
    StateMachine<State, Output, Input>(state)
    {

    }

    std::pair<State, Output> nextValue(State const& state, Input const& input)
    {   
        if (0 <= input)
        {
            return { state + 1, state + 1 };
        }
        else
        {
            return { state - 1, state - 1 };
        }
    }
};

// ###################################################################
// # Média2
// ###################################################################

class Average2:
    public StateMachine<double, double, double>
{
    using State = double;
    using Output = double;
    using Input = double;

public:
    Average2(State state = 0):
    StateMachine<State, Output, Input>(state)
    {

    }

    std::pair<State, Output> nextValue(State const& state, Input const& input)
    {   
        return { input, state + (input - state) / 2 };
    }
};

// ###################################################################
// # Delay
// ###################################################################

class Delay:
    public StateMachine<double, double, double>
{
    using State = double;
    using Output = double;
    using Input = double;

public:
    Delay(State state = 0):
    StateMachine<State, Output, Input>(state)
    {

    }

    std::pair<State, Output> nextValue(State const& state, Input const& input)
    {   
        return { input, state };
    }
};

// ###################################################################
// # Cascade
// ###################################################################

#define CompositeState std::pair<double, double>    
class Cascade: 
    public StateMachine< CompositeState, double, double>
{
    using State1 = double;
    using State2 = double;
    using Output = double;
    using Input = double;
    using Bridge = double;

private:
    #define Machine1 StateMachine<State1, Bridge, Input>
    #define Machine2 StateMachine<State2, Output, Bridge>

    std::unique_ptr<Machine1> m1;
    std::unique_ptr<Machine2> m2;

public:
    Cascade(Machine1 * sm1, Machine2 * sm2):
    StateMachine<CompositeState, Output, Input>({ sm1->state(), sm2->state() }),
    m1(std::move(sm1)),
    m2(std::move(sm2))
    {   
        assert(sm1 != nullptr);
        assert(sm2 != nullptr);
    }

    std::pair<CompositeState, Output> nextValue(CompositeState const& state, Input const& input)
    {
        std::pair<State1, Bridge> const& r1 = this->m1->nextValue(state.first, input);
        std::pair<State2, Output> const& r2 = this->m2->nextValue(state.second, r1.second);
        return
        {
            { r1.first, r2.first },
            { r2.second }
        };
    }
};
#undef Machine2
#undef Machine1
#undef CompositeState

// ###################################################################
// # Paralelo
// ###################################################################

#define CompositeState std::pair<double, double>
#define CompositeOutput std::pair<double, double>
class Parallel:
    public StateMachine< CompositeState, CompositeOutput, double>
{
    using State = double;    
    using Output = double;
    using Input = double;

private:
    #define Machine StateMachine<State, Output, Input>
    
    std::unique_ptr<Machine> m1;
    std::unique_ptr<Machine> m2;

public:
    Parallel(Machine * sm1, Machine * sm2):
    StateMachine<CompositeState, CompositeOutput, Input>({ sm1->state(), sm2->state() }),
    m1(std::move(sm1)),
    m2(std::move(sm2))
    {
        assert(sm1 != nullptr);
        assert(sm2 != nullptr);
    }

    std::pair<CompositeState, CompositeOutput> nextValue(CompositeState const& state, Input const& input)
    {
        std::pair<State, Output> const& r1 = this->m1->nextValue(state.first, input);
        std::pair<State, Output> const& r2 = this->m2->nextValue(state.second, input);
        return
        {
            { r1.first, r2.first },
            { r1.second, r2.second }
        };
    }
};
#undef Machine
#undef CompositeOutput
#undef CompositeState

// ###################################################################
// # Feedback
// ###################################################################

#define CompositeState std::pair<double, double>
class Feedback:
    public StateMachine<CompositeState, double, double>
{
    using State = CompositeState;
    using Output = double;
    using Input = double;

private:
    #define Machine StateMachine<State, Output, Input>
        std::unique_ptr< Machine > m;
    #undef Machine

public:
    Feedback(StateMachine<State, Output, Input> * sm):
    StateMachine<State, Output, Input>(sm->state()),
    m(std::move(sm))
    {
        assert(sm != nullptr);
    }

    std::pair<State, Output> nextValue(State const& state, Input const& input)
    {
        std::pair<State, Output> const& r1 = this->m->nextValue(state, input);
        std::pair<State, Output> const& r2 = this->m->nextValue(state, r1.second);

        return { r2.first, r1.second };
    }
};
#undef CompositeState

// ###################################################################
// # WallController
// ###################################################################

class WallController:
    public StateMachine<double, double, double>
{
    using State = double;
    using Output = double;
    using Input = double;

    using Constant = const double;

private:
    Constant k;
    Constant d;

public:
    WallController(Constant k_ = -1.5f, Constant d_ = 1.0f, State state = 0.0f):
    StateMachine<double, double, double>(state),
    k(k_),
    d(d_)
    {   
        assert(("k deve ser MENOR do que zero !", k < 0));
    }

    std::pair<State, Output> nextValue(State const&, Input const& dt)
    {
        Output res = k * (d - dt);
        return { res, res };
    }
};

// ###################################################################
// # WallController
// ###################################################################

class WallWorld:
    public StateMachine<double, double, double>
{
    using State = double;
    using Output = double;
    using Input = double;

    using Constant = const double;

private:
    Constant dt;

public:
    WallWorld(Constant dt_ = 0.1f, State state = 5.0f):
    StateMachine<double, double, double>(state),
    dt(dt_)
    {
        assert(dt > 0);
    }

    std::pair<State, Output> nextValue(State const& t, Input const& n)
    {
        return { t - dt * n, t };
    }
};

// ###################################################################
// # Main
// ###################################################################

template<typename T>
void show_output(std::vector<T> const& v)
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
    std::vector<double> v(30);
    for (uint32_t i = 0; i < v.size(); v.at(i) = i++ & 2 ? ::rand() : -1 * ::rand());

    Feedback robot(
        new Cascade(
            new WallController, new WallWorld
            )
        );
    show_output(robot.transduce(v));   
    
    return 0;
}