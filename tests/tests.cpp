#include "microhsm/HSM.hpp"
#include <unity.h>

#include <TestHSM.hpp>
#include <TestCTX.hpp>

namespace microhsm_tests
{
    static TestHSM testHSM = TestHSM();
    static TestCTX testCTX = TestCTX();
    
    // Setup and Teardown
    void setUp(void) {
        testCTX.init();
        testHSM.init(static_cast<void*>(&testCTX));
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

    // Test functions
    void test_transitions(void)
    {
        RUN_TEST(test_initial_configuration);
        RUN_TEST(test_transition_a);
        RUN_TEST(test_transition_b);
        RUN_TEST(test_transition_c);
        RUN_TEST(test_transition_d);
        RUN_TEST(test_transition_e);
        RUN_TEST(test_transition_f);

    }

    // Main
    int main(void)
    {
        UNITY_BEGIN();
        test_transitions();
        return UNITY_END();
    }
    

} // microhsm_tests

void setUp(void) {microhsm_tests::setUp();}
void tearDown(void) {microhsm_tests::tearDown();}
int main(void) {microhsm_tests::main();}
