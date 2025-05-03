#include <cstdio>
#include <microhsm/microhsm_config.hpp>
#include <microhsm/State.hpp>
#include <microhsm/HSM.hpp>

namespace microhsm
{

    void HSM::init(void* ctx)
    {
        State* s = this->initState;

        // Walk down until the lowest initial state
        while (s->initial != nullptr) {
            s = s->initial;
        }

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
        const State* s = curState;
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

    eStatus HSM::dispatch(unsigned int signal, void* ctx)
    {
        sTransition t;
        eStatus status = eTRANSITION_ERROR;

        // Match event to state
        bool match = this->matchStateOrAncestor_(signal, &t, ctx);
        if (!match) {
            return eEVENT_IGNORED;
        }

        // Perform transition
        status = this->performTransition_(&t, ctx);
        if (status != eOK) {
            return status;
        }

        // Handle anonymous transitions repeatedly (Run-to-completion)
        match = this->matchStateOrAncestor_(0, &t, ctx);
        while (match) {
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
         *      9. Update `curState`
         */

        State* s = nullptr;
        State* source = this->getState(t->sourceID);
        State* target = this->getState(t->targetID);

        // 1. Handle internal transition
        if (t->kind == eKIND_INTERNAL) return performTransitionInternal_(t, ctx);

        // 2. Bubble up to source state and exit along the way
        source = exitUntilTarget_(this->curState, source, ctx);
        if (source == nullptr) return eTRANSITION_ERROR;

        // 3. Find LCA
        State* lca = this->findLCA_(source, target);

        // 4. Bubble up to LCA
        s = exitUntilTarget_(source, lca, ctx);
        if (s == nullptr) return eTRANSITION_ERROR;

        // 5. Handle exit of source (local v.s. external)
        if(t->kind == eKIND_EXTERNAL && lca == source) {
            lca->exit(ctx);
        }

        // 6. Perform transition effect
        if (t->effect != nullptr) {
            t->effect(ctx);
        }

        // 7. Handle re-entry of source (local v.s. external)
        if(t->kind == eKIND_EXTERNAL && lca == source) {
            lca->entry(ctx);
        }
        
        // 8. Enter until reaching target state
        s = enterUntilTarget_(lca, target, ctx);
        if (s == nullptr) return eTRANSITION_ERROR;

        // 9. Update state
        this->curState = target;

        return eOK;
    }

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
            if (s->ID == target->ID) break;

            s->exit(ctx);
            s = s->parent;
        }
        return s;
    }

    State* HSM::enterUntilTarget_(State* start, State* target, void* ctx)
    {
        State* s = target;

        // Create path from start to target
        while (s != start && s != nullptr) {
            if (s->parent != nullptr) {
                s->parent->tmp = s;
            }
            s = s->parent;
        }
        if (s == nullptr) return s;

        // Perform enter effects until target state
        while (s != target) {
            s = s->tmp;
            s->entry(ctx);
        }
        return s;
    }

    State* HSM::findLCA_(State* a, State* b)
    {
        State* s1 = a;
        State* s2 = b;

        while(s1 != s2) {
            // Move up from deepest state 
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

        return s1;
    }

}
