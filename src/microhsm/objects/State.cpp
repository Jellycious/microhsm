#include "microhsm/config.hpp"
#include <microhsm/microhsm.hpp>

#include <microhsm/objects/History.hpp>

#define UNUSED_ARG_(x) (void)x;

namespace microhsm
{
    State::State(unsigned int id, State* parentState, State* initialState) :
        State(id, parentState, initialState, nullptr, nullptr)
    {
    }

    State::State(unsigned int id,
            State* parentState, State* initialState,
            History* shallowHistory, History* deepHistory) :
        Vertex(id, Vertex::eSTATE),
        parent(parentState),
        initial(initialState),
        depth(State::computeDepth_(this)),
        shallowHistory_(shallowHistory),
        deepHistory_(deepHistory)
    {

        if(parent != nullptr) {
#if MICROHSM_ASSERTIONS == 1
            // A composite state must have an initial state
            MICROHSM_ASSERT(parent->initial != nullptr);
#endif
            // Make sure parent is registered as a composite state.
            parent->isComposite_ = true;
        }

#if MICROHSM_ASSERTIONS == 1
        if (initial != nullptr) {
            MICROHSM_ASSERT(initial != this);
        }
#endif


    }

    State::~State()
    {
    }

    void State::init(void* ctx)
    {
#if MICROHSM_ASSERTIONS == 1
        if (initial != nullptr) {
            MICROHSM_ASSERT(initial->isDescendentOf(this->ID));
        }
#endif

        // Initialize history nodes
        if (shallowHistory_ != nullptr) {
            shallowHistory_->init_shallow(this);
        }
        if (deepHistory_ != nullptr) {
            deepHistory_->init_deep(this);
        }
        this->init_(ctx);
    }

    void State::entry(void* ctx)
    {
        UNUSED_ARG_(ctx);
    }

    void State::exit(void* ctx)
    {
        UNUSED_ARG_(ctx);
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

    /* Static functions */
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


}
