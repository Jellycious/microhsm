/**
 * @file MacroHSM.cpp
 * @brief Test HSM used for testing defined with macros
 */

#include <microhsm/microhsm.hpp>

#include <macros/MacroHSM.hpp>
#include <context/TestCTX.hpp>

#define UNUSED_ARG_(x) (void)x;

namespace microhsm_tests
{

    Vertex* MacroHSM::getVertex(unsigned int ID)
    {
        return this->states[ID];
    }

    unsigned int MacroHSM::getMaxID()
    {
        return e_mstates_COUNT;
    }

    HSM_DEFINE_STATE_INIT(MBaseState)
    {
        UNUSED_ARG_(ctx);
        this->entryCount_ = 0;
        this->exitCount_ = 0;
    }

    HSM_DEFINE_STATE_ENTRY(MBaseState)
    {
        UNUSED_ARG_(ctx);
        this->entryCount_++;
    }

    HSM_DEFINE_STATE_EXIT(MBaseState)
    {
        UNUSED_ARG_(ctx);
        this->exitCount_++;
    }

    HSM_DEFINE_STATE_FUNCTION(MBaseState, unsigned int, getEntryCount)
    {
        return this->entryCount_;
    }

    HSM_DEFINE_STATE_FUNCTION(MBaseState, unsigned int, getExitCount)
    {
        return this->exitCount_;
    }

    HSM_DEFINE_STATE_MATCH(MStateS)
    {
        UNUSED_ARG_(ctx);
        switch(event) {
            case eMEVENT_B:
                return transitionLocal(eMSTATE_S2, t, TestCTX::setFlag);
            case eMEVENT_E:
                return transitionExternal(eMSTATE_S22, t, TestCTX::setFlag);
            case eMEVENT_F:
                return transitionInternal(t, TestCTX::setFlag);
            case eMEVENT_G:
                return transitionExternal(eMSTATE_U, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }

    HSM_DEFINE_STATE_MATCH(MStateS1)
    {
        UNUSED_ARG_(ctx);
        switch(event) {
            case eMEVENT_A:
                return transitionExternal(eMSTATE_S1, t, nullptr);
            case eMEVENT_C:
                return transitionExternal(eMSTATE_S2, t, TestCTX::setFlag);
            case eMEVENT_D:
                return transitionExternal(eMSTATE_S, t, nullptr);
            case eMEVENT_F:
                return transitionExternal(eMSTATE_S22, t, nullptr);
            default:
                break;
        }
        return noTransition();

    }

    HSM_DEFINE_STATE_MATCH(MStateS2)
    {
        UNUSED_ARG_(ctx);
        switch(event) {
            case eMEVENT_C:
                return transitionExternal(eMSTATE_S1, t, TestCTX::clearFlag);
            default:
                break;
        }
        return noTransition();
    }

    HSM_DEFINE_STATE_MATCH(MStateS21)
    {
        UNUSED_ARG_(ctx);
        switch(event) {
            case eMEVENT_B:
                return transitionExternal(eMSTATE_S, t, TestCTX::clearFlag);
            case eMEVENT_E:
                return transitionExternal(eMSTATE_U, t, TestCTX::clearFlag);
            default:
                break;
        }
        return noTransition();
    }

    HSM_DEFINE_STATE_MATCH(MStateS22)
    {
        UNUSED_ARG_(ctx);
        switch(event) {
            case eMEVENT_F:
                return transitionExternal(eMSTATE_S21, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }

    HSM_DEFINE_STATE_MATCH(MStateU)
    {
        UNUSED_ARG_(ctx);
        switch(event) {
            case eMEVENT_G:
                return transitionExternal(eMSTATE_S22, t, TestCTX::setFlag);
            case eMEVENT_E:
                return transitionExternal(eMSTATE_S, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
};

