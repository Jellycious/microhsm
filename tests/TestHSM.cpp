/**
 * @file TestHSM.cpp
 * @brief TestHSM used for testing
 * The HSM is based on an example given in the book:
 *      Practical Statecharts in C/C++ by Miro Samek, Ph.D
 */

#include <microhsm/HSM.hpp>

#include <TestHSM.hpp>
#include <TestCTX.hpp>

namespace microhsm_tests
{

    State* TestHSM::getState(unsigned int ID)
    {
        return this->states[ID];
    }

    unsigned int TestHSM::getStateCount()
    {
        return eSTATE_COUNT;
    }

    void TestState::resetCounts()
    {
        this->entryCount_ = 0;
        this->exitCount_ = 0;
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
        entryCount_++;
    }

    void TestState::exit(void* ctx)
    {
        exitCount_++;
    }

    void TestState::init(void* ctx)
    {
        this->entryCount_ = 0;
        this->exitCount_ = 0;
    }

    bool StateS::match(unsigned int event, sTransition* t, void* ctx)
    {
        switch(event) {
            case eEVENT_B:
                return transitionLocal(eSTATE_S2, t, TestCTX::setFlag);
            case eEVENT_E:
                return transitionExternal(eSTATE_S22, t, TestCTX::setFlag);
            case eEVENT_F:
                return transitionInternal(t, TestCTX::setFlag);
            default:
                break;
        }
        return noTransition();
    }

    bool StateS1::match(unsigned int event, sTransition* t, void* ctx)
    {
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
        switch(event) {
            case eEVENT_C:
                return transitionExternal(eSTATE_S1, t, TestCTX::clearFlag);
            default:
                break;
        }
        return noTransition();
    }

    bool StateS21::match(unsigned int event, sTransition* t, void* ctx)
    {
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
        switch(event) {
            case eEVENT_F:
                return transitionExternal(eSTATE_S21, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
};
