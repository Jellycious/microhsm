/**
 * @file TestHSM.cpp
 * @brief TestHSM used for testing
 * The HSM is based on an example given in the book:
 *      Practical Statecharts in C/C++ by Miro Samek, Ph.D
 */

#include "microhsm/objects/State.hpp"
#include <microhsm/microhsm.hpp>

#include <context/TestCTX.hpp>
#include <basic/TestHSM.hpp>

#define UNUSED_ARG(x) (void)x;

namespace microhsm_tests
{

    Vertex* TestHSM::getVertex(unsigned int ID)
    {
        return this->vertices[ID];
    }

    unsigned int TestHSM::getMaxID()
    {
        return eSTATE_COUNT;
    }

    unsigned int TestState::getEntryCount()
    {
        return entryCount_;
    }
    unsigned int TestState::getExitCount()
    {
        return exitCount_;
    }

    void TestState::entry(void* ctx)
    {
        UNUSED_ARG(ctx);
        entryCount_++;
    }

    void TestState::exit(void* ctx)
    {
        UNUSED_ARG(ctx);
        exitCount_++;
    }

    void TestState::init_(void* ctx)
    {
        UNUSED_ARG(ctx);
        this->entryCount_ = 0;
        this->exitCount_ = 0;
    }

    bool StateS::match(unsigned int event, sTransition* t, void* ctx)
    {
        UNUSED_ARG(ctx);
        switch(event) {
            case eEVENT_B:
                return transitionLocal(eSTATE_S2, t, TestCTX::setFlag);
            case eEVENT_E:
                return transitionExternal(eSTATE_S22, t, TestCTX::setFlag);
            case eEVENT_F:
                return transitionInternal(t, TestCTX::setFlag);
            case eEVENT_G:
                return transitionExternal(eSTATE_U, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }

    bool StateS1::match(unsigned int event, sTransition* t, void* ctx)
    {
        UNUSED_ARG(ctx);
        switch(event) {
            case eEVENT_A:
                return transitionExternal(eSTATE_S1, t, nullptr);
            case eEVENT_C:
                return transitionExternal(eSTATE_S2, t, TestCTX::setFlag);
            case eEVENT_D:
                return transitionExternal(eSTATE_S, t, nullptr);
            case eEVENT_F:
                return transitionExternal(eSTATE_S22, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }

    bool StateS2::match(unsigned int event, sTransition* t, void* ctx)
    {
        UNUSED_ARG(ctx);
        switch(event) {
            case eEVENT_C:
                return transitionExternal(eSTATE_S1, t, TestCTX::clearFlag);
            case eEVENT_E:
                return transitionExternal(eSTATE_U, t, TestCTX::clearFlag);
            default:
                break;
        }
        return noTransition();
    }

    bool StateS21::match(unsigned int event, sTransition* t, void* ctx)
    {
        UNUSED_ARG(ctx);
        switch(event) {
            case eEVENT_B:
                return transitionExternal(eSTATE_S, t, TestCTX::clearFlag);
            default:
                break;
        }
        return noTransition();
    }

    bool StateS22::match(unsigned int event, sTransition* t, void* ctx)
    {
        UNUSED_ARG(ctx);
        switch(event) {
            case eEVENT_F:
                return transitionExternal(eSTATE_S21, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }

    bool StateU::match(unsigned int event, sTransition* t, void* ctx)
    {
        UNUSED_ARG(ctx);
        switch(event) {
            case eEVENT_A:
                return transitionExternal(eSTATE_V, t, nullptr);
            case eEVENT_G:
                return transitionExternal(eSTATE_S22, t, TestCTX::setFlag);
            case eEVENT_E:
                return transitionExternal(eSTATE_S, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }

    bool StateV::match(unsigned int event, sTransition* t, void* ctx)
    {
        UNUSED_ARG(ctx);
        switch(event) {
            case eEVENT_ANONYMOUS:
                return transitionExternal(eSTATE_X, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }

    bool StateX::match(unsigned int event, sTransition* t, void* ctx)
    {
        UNUSED_ARG(ctx);
        switch(event) {
            case eEVENT_ANONYMOUS:
                return transitionExternal(eSTATE_S, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
};
