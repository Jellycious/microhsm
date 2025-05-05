#include "microhsm/HSM.hpp"
#include <unity.h>

#include <MacroHSM.hpp>
#include <TestHSM.hpp>
#include <TestCTX.hpp>

namespace microhsm_tests
{
    /// @brief Test HSM
    static TestHSM testHSM = TestHSM();
    /// @brief Test HSM defined using macros (should exhibit same behavior as `TestHSM`)
    static MacroHSM macroHSM = MacroHSM();

    /// @brief Context test object
    static TestCTX testCTX = TestCTX();
    
    // Setup and Teardown
    void setUp(void) {
        testCTX.init();
        testHSM.init(static_cast<void*>(&testCTX));
        macroHSM.init(static_cast<void*>(&testCTX));
        std::cout << "Test initialization done" << std::endl;
    };

    void tearDown(void) {};

    void test_initial_configuration()
    {
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());

        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void test_transition_a()
    {
        eStatus status;
        // EVENT_A: S1 -> S1 (external)
        status = testHSM.dispatch(eEVENT_A, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(2, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void test_transition_b()
    {
        eStatus status;
        // EVENT_B / TestCTX::setFlag: S1 -> S2
        status = testHSM.dispatch(eEVENT_B, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_TRUE(testCTX.getFlag());

        // EVENT_B / TestCTX::clearFlag: S21 -> S2
        status = testHSM.dispatch(eEVENT_B, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(2, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void test_transition_c()
    {
        eStatus status;

        // C: S1 -> S2 
        status = testHSM.dispatch(eEVENT_C, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_TRUE(testCTX.getFlag());

        // C: S2 -> S1 
        status = testHSM.dispatch(eEVENT_C, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(2, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }


    void test_transition_d()
    {
        eStatus status;

        status = testHSM.dispatch(eEVENT_D, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(2, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));
    }

    void test_transition_e()
    {
        eStatus status;
        status = testHSM.dispatch(eEVENT_E, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(2, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_TRUE(testCTX.getFlag());
    }

    void test_transition_f()
    {
        eStatus status;
        status = testHSM.dispatch(eEVENT_F, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());

        status = testHSM.dispatch(eEVENT_F, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());

        status = testHSM.dispatch(eEVENT_F, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getExitCount());
        TEST_ASSERT_TRUE(testCTX.getFlag());
    }

    void test_transition_g()
    {
        eStatus status;
        status = testHSM.dispatch(eEVENT_G, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_U));
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_u.getExitCount());

        TEST_ASSERT_FALSE(testCTX.getFlag());

        status = testHSM.dispatch(eEVENT_G, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_U));
        TEST_ASSERT_TRUE(testCTX.getFlag());
        TEST_ASSERT_EQUAL(2, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_u.getExitCount());
    }

    // Test functions
    void test_regular_hsm(void)
    {
        RUN_TEST(test_initial_configuration);
        RUN_TEST(test_transition_a);
        RUN_TEST(test_transition_b);
        RUN_TEST(test_transition_c);
        RUN_TEST(test_transition_d);
        RUN_TEST(test_transition_e);
        RUN_TEST(test_transition_f);
        RUN_TEST(test_transition_g);
    }

    void mtest_initial_configuration()
    {
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_U));
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void mtest_transition_a()
    {
        eStatus status;
        // EVENT_A: S1 -> S1 (external)
        status = macroHSM.dispatch(eMEVENT_A, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void mtest_transition_b()
    {
        eStatus status;
        // EVENT_B / TestCTX::setFlag: S1 -> S2
        status = macroHSM.dispatch(eMEVENT_B, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_TRUE(testCTX.getFlag());

        // EVENT_B / TestCTX::clearFlag: S21 -> S2
        status = macroHSM.dispatch(eMEVENT_B, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void mtest_transition_c()
    {
        eStatus status;

        // C: S1 -> S2 
        status = macroHSM.dispatch(eMEVENT_C, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_TRUE(testCTX.getFlag());

        // C: S2 -> S1 
        status = macroHSM.dispatch(eMEVENT_C, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void mtest_transition_d()
    {
        eStatus status;

        status = macroHSM.dispatch(eMEVENT_D, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void mtest_transition_e()
    {
        eStatus status;
        status = macroHSM.dispatch(eMEVENT_E, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_TRUE(testCTX.getFlag());
    }

    void mtest_transition_f()
    {
        eStatus status;
        status = macroHSM.dispatch(eMEVENT_F, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(testCTX.getFlag());

        status = macroHSM.dispatch(eMEVENT_F, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(testCTX.getFlag());

        status = macroHSM.dispatch(eMEVENT_F, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_TRUE(testCTX.getFlag());
    }

    void mtest_transition_g()
    {
        eStatus status;
        status = macroHSM.dispatch(eMEVENT_G, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_U));
        TEST_ASSERT_FALSE(testCTX.getFlag());

        status = macroHSM.dispatch(eMEVENT_G, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_U));
        TEST_ASSERT_TRUE(testCTX.getFlag());
    }

    void test_macro_hsm(void)
    {
        RUN_TEST(mtest_initial_configuration);
        RUN_TEST(mtest_transition_a);
        RUN_TEST(mtest_transition_b);
        RUN_TEST(mtest_transition_c);
        RUN_TEST(mtest_transition_d);
        RUN_TEST(mtest_transition_e);
        RUN_TEST(mtest_transition_f);
        RUN_TEST(mtest_transition_g);
    }

    // Main
    int main(void)
    {
        UNITY_BEGIN();
        test_regular_hsm();
        test_macro_hsm();
        return UNITY_END();
    }

} // microhsm_tests

void setUp(void) {microhsm_tests::setUp();}
void tearDown(void) {microhsm_tests::tearDown();}
int main(void) {microhsm_tests::main();}
