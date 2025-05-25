#include <unity.h>

#include <history/history_tests.hpp>
#include <history/HistoryHSM.hpp>

namespace microhsm_tests {

    HistoryHSM historyHSM = HistoryHSM();

    void setup()
    {
#if MICROHSM_TRACING == 1
        MICROHSM_TRACE_MESSAGE("Initializing history test HSM")
#endif
        historyHSM.init(nullptr);
#if MICROHSM_TRACING == 1
        MICROHSM_TRACE_MESSAGE("Initialized")
#endif

    }

    /**
     * @brief Test history initial configuration
     */
    void htest_initial_configuration()
    {
        setup();
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_I));

        BaseHistory* shallow = static_cast<BaseHistory*>(historyHSM.getVertex(eSTATE_H_SHALLOW_HISTORY));
        BaseHistory* deep = static_cast<BaseHistory*>(historyHSM.getVertex(eSTATE_H_DEEP_HISTORY));

        TEST_ASSERT_TRUE(shallow->getHistoryState() != nullptr);
        TEST_ASSERT_TRUE(deep->getHistoryState() != nullptr);

        // Deep history has no initial state, thus must be set to parent initial state
        TEST_ASSERT_EQUAL(eSTATE_H11, deep->getHistoryState()->ID);
        // Shallow has default state set and therefore must correspond to its default state
        TEST_ASSERT_EQUAL(eSTATE_H2, shallow->getHistoryState()->ID);
    }

    /**
     * @brief Test shallow history default transitions
     */
    void htest_history_default_shallow()
    {
        setup();
        eStatus status = historyHSM.dispatch(eHEVENT_A, nullptr);

        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H11));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));
    }

    /**
     * @brief Test deep history with no default transition
     */
    void htest_history_default_deep() {
        setup();
        eStatus status = historyHSM.dispatch(eHEVENT_B, nullptr);

        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H11));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));
    }

    void htest_stateh12_shallow()
    {
        setup();
        // eEVENT_C: I -> H(H1(H11))
        eStatus status = historyHSM.dispatch(eHEVENT_C, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        // eEVENT_C: H(H1(H11))-> H(H1(H12))
        status = historyHSM.dispatch(eHEVENT_C, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H11));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H12));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));

        // eEVENT_B: H -> I
        status = historyHSM.dispatch(eHEVENT_B, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_I));

        // eEVENT_A: I -> H (shallow history)
        status = historyHSM.dispatch(eHEVENT_A, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        BaseState* final = historyHSM.getCurrentState();
        TEST_ASSERT_EQUAL(eSTATE_H11, final->ID);
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H11));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H12));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));
    }

    void htest_stateh12_deep()
    {
        setup();
        // eEVENT_C: I -> H(H1(H11))
        eStatus status = historyHSM.dispatch(eHEVENT_C, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        // eEVENT_C: H(H1(H11))-> H(H1(H12))
        status = historyHSM.dispatch(eHEVENT_C, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H11));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H12));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));

        // eEVENT_B: H -> I
        status = historyHSM.dispatch(eHEVENT_B, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_I));

        // eEVENT_B: I -> H (deep history)
        status = historyHSM.dispatch(eHEVENT_B, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        BaseState* final = historyHSM.getCurrentState();
        TEST_ASSERT_EQUAL(eSTATE_H12, final->ID);
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H12));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H11));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));
    }

    void htest_stateh21_shallow()
    {
        setup();
        // eEVENT_C: I -> H(H1(H11))
        eStatus status = historyHSM.dispatch(eHEVENT_C, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        // eEVENT_A: H(H1(H11)) -> H(H2(H21))
        status = historyHSM.dispatch(eHEVENT_A, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        // eEVENT_A: H(H2(H21)) -> H(H2(H22))
        status = historyHSM.dispatch(eHEVENT_A, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H22));

        // eEVENT_A: H(H2(H22)) -> I
        status = historyHSM.dispatch(eHEVENT_A, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_I));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H));

        // eEVENT_A: I -> H(shallow history)
        status = historyHSM.dispatch(eHEVENT_A, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H22));
    }

    void htest_stateh21_deep()
    {
        setup();
        // eEVENT_C: I -> H(H1(H11))
        eStatus status = historyHSM.dispatch(eHEVENT_C, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        // eEVENT_A: H(H1(H11)) -> H(H2(H21))
        status = historyHSM.dispatch(eHEVENT_A, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);

        // eEVENT_A: H(H2(H21)) -> H(H2(H22))
        status = historyHSM.dispatch(eHEVENT_A, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H22));

        // eEVENT_A: H(H2(H22)) -> I
        status = historyHSM.dispatch(eHEVENT_A, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_I));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H));

        // eEVENT_B: I -> H(deep history)
        status = historyHSM.dispatch(eHEVENT_B, nullptr);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_I));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H1));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H2));
        TEST_ASSERT_FALSE(historyHSM.inState(eSTATE_H21));
        TEST_ASSERT_TRUE(historyHSM.inState(eSTATE_H22));
    }


    void run_history_tests()
    {
        RUN_TEST(htest_initial_configuration);
        RUN_TEST(htest_history_default_shallow);
        RUN_TEST(htest_history_default_deep);
        RUN_TEST(htest_stateh12_shallow);
        RUN_TEST(htest_stateh12_deep);
        RUN_TEST(htest_stateh21_shallow);
        RUN_TEST(htest_stateh21_deep);
    }

}
