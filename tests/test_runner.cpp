#include "basic/basic_tests.hpp"
#include "macros/macro_tests.hpp"
#include "history/history_tests.hpp"
#include <unity.h>

namespace microhsm_tests
{
    // Setup and Teardown
    void setUp(void) {};
    void tearDown(void) {};

    // Main
    int main(void)
    {
        UNITY_BEGIN();
        run_basic_tests();
        run_macro_tests();
        run_history_tests();

        return UNITY_END();
    }

} // microhsm_tests

void setUp(void) {return microhsm_tests::setUp();}
void tearDown(void) {return microhsm_tests::tearDown();}
int main(void) {return microhsm_tests::main();}
