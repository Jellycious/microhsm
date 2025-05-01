#include "microhsm/HSM.hpp"
#include <TestHSM.hpp>

/*
 * TODO: Setup match function.
 *
 * Setup match function such that it can link to any other state
 *
 */

namespace microhsm_tests
{

    State* TestHSM::get_state(unsigned int ID)
    {
        return this->states[ID];
    }

    bool StateS::match(unsigned int event, s_transition* t, void* ctx)
    {
        return false;
    }

    bool StateS1::match(unsigned int event, s_transition* t, void* ctx)
    {
        return false;
    }

    bool StateS2::match(unsigned int event, s_transition* t, void* ctx)
    {
        return false;
    }

};
