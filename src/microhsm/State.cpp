#include <microhsm/microhsm.hpp>

#define UNUSED_ARG_(x) (void)x;

namespace microhsm
{
    /**
     * @brief Compute depth of state
     * @param s Pointer to state to compute depth of
     * @return depth of state
     */
    unsigned int State::computeDepth_(State* s)
    {
        State* ancestor = s;
        unsigned int depth = 0;
        while (ancestor->parent != nullptr) {
            depth++;
            ancestor = ancestor->parent;
        }
        return depth;
    }

    State::State(unsigned int id, State* parentState, State* initialState) :
        parent(parentState),
        initial(initialState),
        ID(id),
        depth(State::computeDepth_(this))
    {
        // Make sure parent is registered as a composite state.
        if(parent != nullptr) {
            parent->isComposite_ = true;
        }
    }

    bool State::noTransition()
    {
        return false;
    }

    bool State::isComposite()
    {
        // Any composite state should have its initial state set
        return isComposite_;
    }

    bool State::isDescendentOf(unsigned int id)
    {
        bool descendent = false;
        State* s = this;
        while (!descendent && s != nullptr) {
            descendent = (s->ID == id);
            s = s->parent;
        }
        return descendent;
    }

    State* State::getAncestor(unsigned int id)
    {
        State* s = this->parent;
        while (s != nullptr) {
            if (s->ID == id) break;

            s = s->parent;
        }
        return s;
    }

    bool State::transitionExternal(unsigned int target_ID, sTransition *t, fTransitionEffect effect)
    {
        t->sourceID = ID;
        t->targetID = target_ID;
        t->kind = eKIND_EXTERNAL;
        t->effect = effect;
        return true;
    }

    bool State::transitionInternal(sTransition *t, fTransitionEffect effect)
    {
        t->sourceID = ID;
        t->targetID = ID;
        t->kind = eKIND_INTERNAL;
        t->effect = effect;
        return true;
    }

    bool State::transitionLocal(unsigned int target_ID, sTransition *t, fTransitionEffect effect)
    {

#if MICROHSM_ASSERTIONS == 1
        // UML V2.5.1: For local transitions source and target vertex must be different
        MICROHSM_ASSERT(this->ID != target_ID);
        // UML V2.5.1: Local transition source state must be composite
        MICROHSM_ASSERT(this->isComposite());
#endif

        t->sourceID = ID;
        t->targetID = target_ID;
        t->kind = eKIND_LOCAL;
        t->effect = effect;
        return true;
    }

    bool State::match(unsigned int event, sTransition* t, void *ctx)
    {
        UNUSED_ARG_(ctx);
        UNUSED_ARG_(event);
        UNUSED_ARG_(t);
        // By default: no match
        return noTransition();
    }

}
