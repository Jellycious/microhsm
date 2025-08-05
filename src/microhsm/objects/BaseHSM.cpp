/**
 * @file BaseHSM.cpp
 * @brief Base for Hierarchical State Machines
 *
 * @author Jelle Meijer
 * @date 2025-05-23
 */

#include <microhsm/microhsm.hpp>

namespace microhsm
{
    BaseHSM::BaseHSM(BaseState& initial) :
        curState(&initial),
        initState(initial)
    {
    };

    BaseHSM::~BaseHSM()
    {
    }

    /* --- Static Functions --- */
    void BaseHSM::performEffect_(const sTransition* t, void* ctx)
    {
        if (t->effect != nullptr) {
            // Perform transition effect
            t->effect(ctx);
        }
    }

    void BaseHSM::updateHistories_(BaseState* newState)
    {
#if MICROHSM_ASSERTIONS == 1
        // `newState` must be 'leaf' state
        bool isComposite = newState->isComposite();
        MICROHSM_ASSERT(isComposite == false);
#endif
        // Set histories
        BaseState* s = newState;
        while (s->parent != nullptr) {
            // Update new history
            if (s->parent->deepHistory_ != nullptr) {
                s->parent->deepHistory_->setHistoryState(newState);
            }
            // Update shallow history
            if (s->parent->shallowHistory_ != nullptr) {
                s->parent->shallowHistory_->setHistoryState(s);
            }
            s = s->parent;
        }

    }

    eStatus BaseHSM::performTransitionInternal_(const sTransition* t, void* ctx)
    {
        // Perform transition effect
        performEffect_(t, ctx);
        return eOK;
    }

    BaseState* BaseHSM::findLCA_(BaseState* a, BaseState* b)
    {
        BaseState* s1 = a;
        BaseState* s2 = b;

        while(s1 != s2) {
            // Move up from deepest state
            if (s1 == nullptr || s2 == nullptr) return nullptr;

            if (s1->depth > s2->depth) {
                s1 = s1->parent;
            }
            else if (s2->depth > s1->depth) {
                s2 = s2->parent;
            }
            else {
                s1 = s1->parent;
                s2 = s2->parent;
            }
        }

        // s1 contains lowest common ancestor or `nullptr
        return s1;
    }
    /* --- End static functions --- */

    /* --- Member functions --- */
    void BaseHSM::init(void* ctx)
    {
        // Initialize all states
        for (unsigned int id = 0; id < this->getMaxID(); id++) {

            Vertex* v = this->getVertex(id);

            if (v != nullptr) {
                if (v->TYPE == Vertex::eSTATE) {
                    BaseState* s = static_cast<BaseState*>(v);
                    s->init(ctx);
                }
            }
        }

        // Perform entry on initial state
        BaseState* s = &this->initState;
        this->enterState_(s, ctx);

        // Walk down until the leaf initial state
        s = enterInitialStates_(s, ctx);

        this->curState = s;

        // Handle any initial anonymous transitions
        this->dispatch(EVENT_ANONYMOUS, ctx);
    }

    eStatus BaseHSM::dispatch(unsigned int event, void* ctx)
    {
        sTransition t;
        eStatus status = eTRANSITION_ERROR;

        // Match event to state
        bool match = this->matchStateOrAncestor_(event, &t, ctx);
        if (!match) {
#if MICROHSM_TRACING == 1
            MICROHSM_TRACE_DISPATCH_IGNORED(event);
#endif
            return eEVENT_IGNORED;
        }

#if MICROHSM_TRACING == 1
            MICROHSM_TRACE_DISPATCH_MATCHED(event, t.sourceID);
#endif

        // Perform transition
        status = this->performTransition_(&t, ctx);
        if (status != eOK) return status;

        // Handle anonymous transitions repeatedly (Run-to-completion)
        match = this->matchStateOrAncestor_(0, &t, ctx);
        while (match) {
#if MICROHSM_TRACING == 1
            MICROHSM_TRACE_DISPATCH_MATCHED(0, t.sourceID);
#endif
            status = this->performTransition_(&t, ctx);
            if (status != eOK) return status;

            match = this->matchStateOrAncestor_(0, &t, ctx);
        }

        return status;
    }

    BaseState* BaseHSM::getCurrentState()
    {
        return this->curState;
    }

    bool BaseHSM::inState(unsigned int ID)
    {
        const BaseState* s = this->curState;
        while (s != nullptr) {
            if (s->ID == ID) return true;
            s = s->parent;
        }
        return false;
    }

    bool BaseHSM::matchStateOrAncestor_(unsigned int event, sTransition* t, void* ctx)
    {
        BaseState* s = this->curState;
        bool match = false;
        while (!match && s != nullptr) {
            match = s->match(event, t, ctx);
            s = s->parent;
        }

        return match;
    }

    eStatus BaseHSM::performTransition_(const sTransition* t, void* ctx)
    {
        /*
         * Steps:
         *      0. Determine source and target.
         *      1. Internal Transition?
         *          Yes:    Perform internal transitions and return
         *          No:     Continue to body of function
         *      2. Bubble up to source state
         *      3. Find LCA
         *      4. Bubble up to LCA
         *      5. Handle exit source (kind = external)
         *      6. Perform transition effect
         *      7. Handle re-entry source (kind = external)
         *      8. Enter until reaching target state
         *      9. Enter initial pseudo state(s)
         *      10. Update `curState`
         */

        BaseState* s = nullptr;

        // 0. Determine source and target
        Vertex* sourceV = this->getVertex(t->sourceID);
#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(sourceV != nullptr);
        MICROHSM_ASSERT(sourceV->TYPE == Vertex::eSTATE);
#endif
        BaseState* source = static_cast<BaseState*>(sourceV);
        BaseState* target = getTransitionTarget_(t->targetID);

        // 1. Handle internal transition
        if (t->kind == eKIND_INTERNAL) return performTransitionInternal_(t, ctx);

        // 2. Bubble up to source state and exit along the way
        source = exitUntilTarget_(this->curState, source, ctx);
#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT((source != nullptr)); // source state could not be reached
#endif
        // 3. Find LCA
        BaseState* lca = this->findLCA_(source, target);

        // 4. Bubble up to LCA
        exitUntilTarget_(source, lca, ctx);

        // 5. Handle exit of source (local v.s. external)
        if(t->kind == eKIND_EXTERNAL && lca == source) exitState_(lca, ctx);

        // 6. Perform transition effect
        performEffect_(t, ctx);

        // 7. Handle re-entry of source (local v.s. external)
        if(t->kind == eKIND_EXTERNAL && lca == source) enterState_(lca, ctx);

        // 8. Enter until reaching target state
        s = enterUntilTarget_(lca, target, ctx);
#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(s != nullptr);  // target could not be reached
#endif

        // 9. Enter initial pseudo state(s)
        s = enterInitialStates_(s, ctx);

        // 10. Update state
        this->setNewActiveState_(s);
        return eOK;
    }

    void BaseHSM::setNewActiveState_(BaseState* s)
    {
        this->curState = s;
        updateHistories_(s);
    }

    BaseState* BaseHSM::getTransitionTarget_(unsigned int targetID)
    {
        Vertex* targetV = this->getVertex(targetID);
        switch (targetV->TYPE) {
            case Vertex::eSTATE:
                return static_cast<BaseState*>(targetV);
            case Vertex::ePSEUDO_HISTORY: {
                BaseHistory* h = static_cast<BaseHistory*>(targetV);
                return h->getHistoryState();
            }
            default: {
                return nullptr;
            }
        }
    }

    BaseState* BaseHSM::exitUntilTarget_(BaseState* startState, const BaseState* target, void* ctx)
    {
        BaseState* s = startState;
        while (s != nullptr) {
            if (s == target) break;
            exitState_(s, ctx);
            s = s->parent;
        }
        return s;
    }

    BaseState* BaseHSM::enterUntilTarget_(const BaseState* start, BaseState* target, void* ctx)
    {
        // Edge case where we are already in the target
        if (start == target) return target;

        BaseState* s = target;

        // Create path from start to target
        while (s->parent != start) {
            s->parent->tmp_ = s;
            s = s->parent;
        }

        // Walk path and perform entries
        while(s != target) {
            enterState_(s, ctx);
            s = s->tmp_;
        }
        enterState_(s, ctx);

        return s;
    }

    void BaseHSM::enterState_(BaseState* s, void* ctx)
    {
        // Assign current state to newly entered state
        this->curState = s;
#if MICROHSM_TRACING == 1
        MICROHSM_TRACE_ENTRY(s->ID);
#endif
        // Perform entry effect
        s->entry(ctx);
    }

    void BaseHSM::exitState_(BaseState* s, void* ctx)
    {
#if MICROHSM_TRACING == 1
        MICROHSM_TRACE_EXIT(s->ID);
#endif
        // Perform exit effect
        s->exit(ctx);
        // Assign current state to parent of state we just left
        this->curState = s->parent;
    }

    BaseState* BaseHSM::enterInitialStates_(BaseState* state, void* ctx)
    {
        BaseState* s = state;
        while (s->initial != nullptr) {
            // Traverse initial states until reaching a 'leaf' state
            s = s->initial;
            enterState_(s, ctx);
        }
        return s;
    }

}
