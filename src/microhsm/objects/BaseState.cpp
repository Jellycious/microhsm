/**
 * @file BaseState.cpp
 * @brief Contains State declaration/definition.
 *
 * @author Jelle Meijer
 * @date 2025-05-23
 */

#include "microhsm/objects/History.hpp"
#include <microhsm/microhsm.hpp>

#define UNUSED_ARG_(x) (void)x;

namespace microhsm
{
    BaseState::BaseState(unsigned int id, BaseState* parentState, BaseState* initialState) :
        BaseState(id, parentState, initialState, nullptr, nullptr)
    {
    }

    BaseState::BaseState(unsigned int id,
            BaseState* parentState, BaseState* initialState,
            ShallowHistory* shallowHistory, DeepHistory* deepHistory) :
        Vertex(id, Vertex::eSTATE),
        parent(parentState),
        initial(initialState),
        depth(BaseState::computeDepth_(this)),
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

    BaseState::~BaseState()
    {
    }

    void BaseState::init(void* ctx)
    {
#if MICROHSM_ASSERTIONS == 1
        if (initial != nullptr) {
            MICROHSM_ASSERT(initial->isDescendentOf(this->ID));
        }
#endif

        // Initialize history nodes
        if (shallowHistory_ != nullptr) {
            shallowHistory_->init(this);
        }
        if (deepHistory_ != nullptr) {
            deepHistory_->init(this);
        }
        this->init_(ctx);
    }

    void BaseState::entry(void* ctx)
    {
        UNUSED_ARG_(ctx);
    }

    void BaseState::exit(void* ctx)
    {
        UNUSED_ARG_(ctx);
    }

    bool BaseState::noTransition()
    {
        return false;
    }

    bool BaseState::isComposite()
    {
        // Any composite state should have its initial state set
        return isComposite_;
    }

    bool BaseState::isDescendentOf(unsigned int id)
    {
        bool descendent = false;
        BaseState* s = this;
        while (!descendent && s != nullptr) {
            descendent = (s->ID == id);
            s = s->parent;
        }
        return descendent;
    }

    BaseState* BaseState::getAncestor(unsigned int id)
    {
        BaseState* s = this->parent;
        while (s != nullptr) {
            if (s->ID == id) break;

            s = s->parent;
        }
        return s;
    }

    bool BaseState::transitionExternal(unsigned int target_ID, sTransition *t, fTransitionEffect effect)
    {
        t->sourceID = ID;
        t->targetID = target_ID;
        t->kind = eKIND_EXTERNAL;
        t->effect = effect;
        return true;
    }

    bool BaseState::transitionInternal(sTransition *t, fTransitionEffect effect)
    {
        t->sourceID = ID;
        t->targetID = ID;
        t->kind = eKIND_INTERNAL;
        t->effect = effect;
        return true;
    }

    bool BaseState::transitionLocal(unsigned int target_ID, sTransition *t, fTransitionEffect effect)
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
    unsigned int BaseState::computeDepth_(BaseState* s)
    {
        BaseState* ancestor = s;
        unsigned int depth = 0;
        while (ancestor->parent != nullptr) {
            depth++;
            ancestor = ancestor->parent;
        }
        return depth;
    }


}
