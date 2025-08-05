#include "microhsm_config.hpp"
#include <unity.h>

#include <context/TestCTX.hpp>
#include <macros/macro_tests.hpp>
#include <macros/MacroHSM.hpp>

namespace microhsm_tests
{

    static MacroHSM macroHSM = MacroHSM();
    static TestCTX testCTX = TestCTX();

    void testSetup()
    {
        MICROHSM_TEST_MESSAGE("Initializing macro test HSM")
        testCTX.init();
        macroHSM.init(static_cast<void*>(&testCTX));
        MICROHSM_TEST_MESSAGE("Initialized")
    }

    void mtest_initial_configuration()
    {
        testSetup();
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
        testSetup();
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
        testSetup();
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
        testSetup();
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
        testSetup();
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
        testSetup();
        eStatus status;
        status = macroHSM.dispatch(eMEVENT_E, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_U));
        TEST_ASSERT_EQUAL(2, macroHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_u.getExitCount());
        TEST_ASSERT_TRUE(testCTX.getFlag());

        status = macroHSM.dispatch(eMEVENT_F, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_U));
        TEST_ASSERT_EQUAL(2, macroHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_u.getExitCount());
        TEST_ASSERT_TRUE(testCTX.getFlag());

        status = macroHSM.dispatch(eMEVENT_E, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_U));
        TEST_ASSERT_EQUAL(2, macroHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(2, macroHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(0, macroHSM.state_u.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());

        status = macroHSM.dispatch(eMEVENT_E, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S));
        TEST_ASSERT_TRUE(macroHSM.inState(eMSTATE_S1));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S2));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S21));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_S22));
        TEST_ASSERT_FALSE(macroHSM.inState(eMSTATE_U));
        TEST_ASSERT_EQUAL(3, macroHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(2, macroHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(2, macroHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(1, macroHSM.state_u.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void mtest_transition_f()
    {
        testSetup();
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
        testSetup();
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

    void run_macro_tests(void)
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
}
