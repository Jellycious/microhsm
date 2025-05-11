#include <unity.h>

#include "history_tests.hpp"
#include "HistoryHSM.hpp"

namespace microhsm_tests {

    HistoryHSM historyHSM = HistoryHSM();

    void setup()
    {

    }

    void htest_initial_configuration()
    {
        TEST_ASSERT_TRUE(true);
    }

    void run_history_tests()
    {
        RUN_TEST(htest_initial_configuration);
    }

}
