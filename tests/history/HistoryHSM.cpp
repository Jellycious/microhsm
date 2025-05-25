#include <history/HistoryHSM.hpp>

#define UNUSED_ARG_(arg) (void)arg;

namespace microhsm_tests
{

    unsigned int HistoryHSM::getMaxID(void)
    {
        return eSTATE_I;
    }

    Vertex* HistoryHSM::getVertex(unsigned int id)
    {
        unsigned int index = id - 14;
        if (index >= (sizeof(this->vertices_) / sizeof(Vertex*))) {
            return nullptr;
        }
        return vertices_[index];
    }

    HSM_DEFINE_STATE_MATCH(StateH)
    {
        UNUSED_ARG_(ctx);
        UNUSED_ARG_(event);
        UNUSED_ARG_(t);
        switch (event) {
            case eHEVENT_B:
                return this->transitionExternal(eSTATE_I, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
    HSM_DEFINE_STATE_MATCH(StateH1)
    {
        UNUSED_ARG_(ctx);
        UNUSED_ARG_(event);
        UNUSED_ARG_(t);
        switch (event) {
            case eHEVENT_A:
                return transitionExternal(eSTATE_H2, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
    HSM_DEFINE_STATE_MATCH(StateH11)
    {
        UNUSED_ARG_(ctx);
        UNUSED_ARG_(event);
        UNUSED_ARG_(t);
        switch (event) {
            case eHEVENT_C:
                return transitionExternal(eSTATE_H12, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
    HSM_DEFINE_STATE_MATCH(StateH12)
    {
        UNUSED_ARG_(ctx);
        UNUSED_ARG_(event);
        UNUSED_ARG_(t);
        return noTransition();
    }
    HSM_DEFINE_STATE_MATCH(StateH2)
    {
        UNUSED_ARG_(ctx);
        UNUSED_ARG_(event);
        UNUSED_ARG_(t);
        return noTransition();
    }
    HSM_DEFINE_STATE_MATCH(StateH21)
    {
        UNUSED_ARG_(ctx);
        UNUSED_ARG_(event);
        UNUSED_ARG_(t);
        switch (event) {
            case eHEVENT_A:
                return transitionExternal(eSTATE_H22, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
    HSM_DEFINE_STATE_MATCH(StateH22)
    {
        UNUSED_ARG_(ctx);
        UNUSED_ARG_(event);
        UNUSED_ARG_(t);
        switch (event) {
            case eHEVENT_A:
                return transitionExternal(eSTATE_I, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
    HSM_DEFINE_STATE_MATCH(StateI)
    {
        UNUSED_ARG_(ctx);
        UNUSED_ARG_(event);
        UNUSED_ARG_(t);
        switch(event) {
            case eHEVENT_A:
                return transitionExternal(eSTATE_H_SHALLOW_HISTORY, t, nullptr);
            case eHEVENT_B:
                return transitionExternal(eSTATE_H_DEEP_HISTORY, t, nullptr);
            case eHEVENT_C:
                return transitionExternal(eSTATE_H, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }


}
