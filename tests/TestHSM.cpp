#include <TestHSM.hpp>

namespace microhsm_tests
{

    
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

    void TestHSM::init(void);
    {

    }


};
