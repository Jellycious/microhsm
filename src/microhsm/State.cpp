#include <microhsm/State.hpp>

namespace microhsm
{

    State::State(unsigned int ID, State* parent, State* initial) :
        ID(ID),
        parent(parent),
        initial(initial)
    {
    }

    void State::setHSM(HSM* hsm)
    {
        this->hsm = hsm;
    }

    bool State::no_transition()
    {
        return false;
    }

    bool State::transition_external(State* target, s_transition *t, f_transition_effect effect)
    {
        t->source = this;
        t->target = target;
        t->kind = eKIND_EXTERNAL;
        t->effect = effect;
        return true;
    }

    bool State::transition_internal(s_transition *t, f_transition_effect effect)
    {
    
        t->source = this;
        t->target = this;
        t->kind = eKIND_INTERNAL;
        t->effect = effect;
        return true;
    }

    bool State::transition_local(State* target, s_transition *t, f_transition_effect effect)
    {
        t->source = this;
        t->target = target;
        t->kind = eKIND_LOCAL;
        t->effect = effect;
        return true;
    }

    bool State::match(unsigned int event, s_transition* t, void *ctx)
    {
        // By default: no match
        return false;
    }

}
