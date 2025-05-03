#include "microhsm/HSM.hpp"
#include <unity.h>

#include <TestHSM.hpp>
#include <TestCTX.hpp>

namespace microhsm_tests
{
    static TestHSM testHSM = TestHSM();
    static TestCTX testCTX = TestCTX();
    
    // Setup and Teardown
    void setUp(void) 
    {
        testCTX.init();
        testHSM.init(static_cast<void*>(&testCTX));
    };

    void tearDown(void) {};
    

    // Test functions
    void test_transitions(void)
    {
        eStatus status;
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));
        // C: S1 -> S2 
        status = testHSM.dispatch(eEVENT_C, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        // C: S2 -> S1 
        status = testHSM.dispatch(eEVENT_C, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));
    }

    // Main
    int main(void)
    {
        UNITY_BEGIN();
        RUN_TEST(test_transitions);
        return UNITY_END();
    }
    

} // microhsm_tests

void setUp(void) {microhsm_tests::setUp();}
void tearDown(void) {microhsm_tests::tearDown();}
int main(void) {microhsm_tests::main();}
