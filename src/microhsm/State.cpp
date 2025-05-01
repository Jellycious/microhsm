#include <microhsm/State.hpp>

namespace microhsm
{

    State::State(unsigned int ID, State* parent, State* initial) :
        ID(ID),
        parent(parent),
        initial(initial)
    {
    }

    bool State::no_transition()
    {
        return false;
    }

    bool State::transition_external(unsigned int target_ID, s_transition *t, f_transition_effect effect)
    {
        t->source_ID = ID;
        t->target_ID = target_ID;
        t->kind = eKIND_EXTERNAL;
        t->effect = effect;
        return true;
    }

    bool State::transition_internal(s_transition *t, f_transition_effect effect)
    {
    
        t->source_ID = ID;
        t->target_ID = ID;
        t->kind = eKIND_INTERNAL;
        t->effect = effect;
        return true;
    }

    bool State::transition_local(unsigned int target_ID, s_transition *t, f_transition_effect effect)
    {
        t->source_ID = ID;
        t->target_ID = target_ID;
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
