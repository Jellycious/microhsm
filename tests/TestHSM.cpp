#include "microhsm/HSM.hpp"
#include <TestHSM.hpp>

#include <cstdio>
#include <iostream>

/*
 * TODO: Setup match function.
 *
 * Setup match function such that it can link to any other state
 *
 */

namespace microhsm_tests
{

    State* TestHSM::getState(unsigned int ID)
    {
        return this->states[ID];
    }

    bool StateS::match(unsigned int event, sTransition* t, void* ctx)
    {
        return noTransition();
    }

    void StateS::entry(void* ctx)
    {
        std::cout << "S(entry)" << std::endl;
    }
    void StateS::exit(void* ctx)
    {
        std::cout << "S(exit)" << std::endl;
    }


    bool StateS1::match(unsigned int event, sTransition* t, void* ctx)
    {
        switch(event) {
            case eEVENT_C:
                return transitionExternal(eSTATE_S2, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
    void StateS1::entry(void* ctx)
    {
        std::cout << "S1(entry)" << std::endl;
    }
    void StateS1::exit(void* ctx)
    {
        std::cout << "S1(exit)" << std::endl;
    }

    bool StateS2::match(unsigned int event, sTransition* t, void* ctx)
    {
        switch(event) {
            case eEVENT_C:
                return transitionExternal(eSTATE_S1, t, nullptr);
            default:
                break;
        }
        return noTransition();
    }
    void StateS2::entry(void* ctx)
    {
        std::cout << "S2(entry)" << std::endl;
    }
    void StateS2::exit(void* ctx)
    {
        std::cout << "S2(exit)" << std::endl;
    }

};
