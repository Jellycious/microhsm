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
        return noTransition();
    }

    bool StateS1::match(unsigned int event, sTransition* t, void* ctx)
    {
        TestCTX* c = static_cast<TestCTX*>(ctx);
        switch(event) {
            case eEVENT_C:
                return transitionExternal(eSTATE_S2, t, c->effect1);
            case eEVENT_D:
                return transitionExternal(eSTATE_S, t, c->effect2);
            default:
                break;
        }
        return noTransition();
    }

    bool StateS2::match(unsigned int event, sTransition* t, void* ctx)
    {
        TestCTX* c = static_cast<TestCTX*>(ctx);
        switch(event) {
            case eEVENT_C:
                return transitionExternal(eSTATE_S1, t, c->effect1);
            default:
                break;
        }
        return noTransition();
    }
};
