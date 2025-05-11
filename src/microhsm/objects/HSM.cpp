#include <microhsm/objects/State.hpp>
#include <microhsm/objects/History.hpp>
#include <microhsm/microhsm.hpp>

namespace microhsm
{
    HSM::HSM(State* initial) :
        curState(initial),
        initState(initial) 
    {
    };

    HSM::~HSM()
    {
    }

    void HSM::init(void* ctx)
    {
        // Initialize all states
        for (unsigned int id = 0; id < this->getMaxID(); id++) {

            Vertex* v = this->getVertex(id);

            if (v != nullptr) {
                if (v->TYPE == Vertex::eSTATE) {
                    State* s = static_cast<State*>(v);
                    s->init(ctx);
                }
            }
        }

        // Perform entry on initial state
        State* s = this->initState;
        this->performEntry_(s, ctx);

        // Walk down until the leaf initial state
        s = enterInitialStates_(s, ctx);

        this->curState = s;

        // Handle any initial anonymous transitions
        this->dispatch(EVENT_ANONYMOUS, ctx);
    }

    State* HSM::getCurrentState()
    {
        return this->curState;
    }

    bool HSM::inState(unsigned int ID)
    {
        const State* s = this->curState;
        while (s != nullptr) {
            if (s->ID == ID) return true;
            s = s->parent;
        }
        return false;
    }

    bool HSM::matchStateOrAncestor_(unsigned int event, sTransition* t, void* ctx)
    {
        State* s = this->curState;
        bool match = false;
        while (!match && s != nullptr) {
            match = s->match(event, t, ctx);
            s = s->parent;
        }

        return match;
    }

    eStatus HSM::dispatch(unsigned int event, void* ctx)
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
        if (status != eOK) {
            return status;
        }

        // Handle anonymous transitions repeatedly (Run-to-completion)
        match = this->matchStateOrAncestor_(0, &t, ctx);
        while (match) {

#if MICROHSM_TRACING == 1
            MICROHSM_TRACE_DISPATCH_MATCHED(0, t.sourceID);
#endif

            status = this->performTransition_(&t, ctx);
            if (status != eOK) {
                return status;
            }

            match = this->matchStateOrAncestor_(0, &t, ctx);
        }

        return status;
    }

    eStatus HSM::performTransition_(const sTransition* t, void* ctx)
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

        State* s = nullptr;
        
        // 0. Determine source and target
        Vertex* sourceV = this->getVertex(t->sourceID);
#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT(sourceV != nullptr);
        MICROHSM_ASSERT(sourceV->TYPE == Vertex::eSTATE);
#endif
        State* source = static_cast<State*>(sourceV);
        State* target = getTransitionTarget_(t->targetID);

        // 1. Handle internal transition
        if (t->kind == eKIND_INTERNAL) return performTransitionInternal_(t, ctx);

        // 2. Bubble up to source state and exit along the way
        source = exitUntilTarget_(this->curState, source, ctx);
#if MICROHSM_ASSERTIONS == 1
        MICROHSM_ASSERT((source != nullptr)); // source state could not be reached
#endif
        // 3. Find LCA
        State* lca = this->findLCA_(source, target);

        // 4. Bubble up to LCA
        s = exitUntilTarget_(source, lca, ctx);

        // 5. Handle exit of source (local v.s. external)
        if(t->kind == eKIND_EXTERNAL && lca == source) performExit_(lca, ctx);

        // 6. Perform transition effect
        if (t->effect != nullptr) t->effect(ctx);

        // 7. Handle re-entry of source (local v.s. external)
        if(t->kind == eKIND_EXTERNAL && lca == source) performEntry_(lca, ctx);
        
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

    void HSM::setNewActiveState_(State* s)
    {
        this->curState = s; 
        updateHistories_(s);
    }

    State* HSM::getTransitionTarget_(unsigned int targetID)
    {
        Vertex* targetV = this->getVertex(targetID);
        switch (targetV->TYPE) {
            case Vertex::eSTATE:
                return static_cast<State*>(targetV);
            case Vertex::ePSEUDO_HISTORY: {
                History* h = static_cast<History*>(targetV);
                return h->getHistoryState();
            }
            default:
                break;
        }
        return nullptr;
    }

    /* --- Static Functions --- */
    eStatus HSM::performTransitionInternal_(const sTransition* t, void* ctx)
    {
        // Perform transition effect
        if (t->effect != nullptr) {
            t->effect(ctx);
        }

        return eOK;
    }

    State* HSM::exitUntilTarget_(State* startState, State* target, void* ctx)
    {
        State* s = startState;
        while (s != nullptr) {
            if (s == target) break;
            performExit_(s, ctx);
            s = s->parent;
        }
        return s;
    }

    State* HSM::enterUntilTarget_(State* start, State* target, void* ctx)
    {
        // Edge case where we are already in the target
        if (start == target) return target;

        State* s = target;

        // Create path from start to target
        while (s->parent != start) {
            s->parent->tmp_ = s;
            s = s->parent;
        }

        // Walk path and perform entries
        while(s != target) {
            performEntry_(s, ctx);
            s = s->tmp_;
        }
        performEntry_(s, ctx);

        return s;
    }

    State* HSM::findLCA_(State* a, State* b)
    {
        State* s1 = a;
        State* s2 = b;

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

    void HSM::performEntry_(State* s, void* ctx)
    {
#if MICROHSM_TRACING == 1
        MICROHSM_TRACE_ENTRY(s->ID);
#endif
        s->entry(ctx);
    }

    void HSM::performExit_(State* s, void* ctx)
    {
#if MICROHSM_TRACING == 1
        MICROHSM_TRACE_EXIT(s->ID);
#endif
        s->exit(ctx);
    }

    State* HSM::enterInitialStates_(State* state, void* ctx)
    {
        State* s = state;
        while (s->initial != nullptr) {
            // Traverse initial states until reaching a 'leaf' state
            s = s->initial;
            performEntry_(s, ctx);
        }
        return s;
    }

    void HSM::performEffect_(sTransition* t, void* ctx)
    {
        if (t->effect != nullptr) {
            // Perform transition effect
            t->effect(ctx);
        }
    }

    void HSM::updateHistories_(State* newState)
    {
    }
    /* --- End static functions --- */


}
