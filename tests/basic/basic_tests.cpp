#include <unity.h>

#include <context/TestCTX.hpp>

#include <basic/basic_tests.hpp>
#include <basic/TestHSM.hpp>

namespace microhsm_tests 
{

    static TestCTX testCTX = TestCTX();
    static TestHSM testHSM = TestHSM();

    /* Private functions */
    void setupTest()
    {
        testCTX.init();
        testHSM.init(static_cast<void*>(&testCTX));
    }

    void test_initial_configuration()
    {
        setupTest();
        TEST_ASSERT_EQUAL(testHSM.getCurrentState()->ID, eSTATE_S1);
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

    void test_state_functions()
    {
        setupTest();
        TEST_ASSERT_EQUAL(testHSM.getCurrentState()->ID, eSTATE_S1);

        State* curState = testHSM.getCurrentState();
        TEST_ASSERT_TRUE(curState->isDescendentOf(eSTATE_S));
        TEST_ASSERT_TRUE(curState->isDescendentOf(eSTATE_S1));
        TEST_ASSERT_FALSE(curState->isDescendentOf(eSTATE_S2));
        TEST_ASSERT_FALSE(curState->isDescendentOf(eSTATE_U));

        State* ancestor = curState->getAncestor(eSTATE_S);
        TEST_ASSERT_NOT_NULL(ancestor);
        TEST_ASSERT_EQUAL(ancestor->ID, eSTATE_S);
        TEST_ASSERT_NULL(curState->getAncestor(eSTATE_S1));
        TEST_ASSERT_NULL(curState->getAncestor(eSTATE_U));

        Vertex* sV = testHSM.getVertex(eSTATE_S);
        Vertex* s2V = testHSM.getVertex(eSTATE_S2);
        TEST_ASSERT_NOT_NULL(sV);
        TEST_ASSERT_NOT_NULL(s2V);
        State* s = static_cast<State*>(sV);
        State* s2 = static_cast<State*>(s2V);

        TEST_ASSERT_TRUE(s->isComposite());
        TEST_ASSERT_FALSE(curState->isComposite());
        TEST_ASSERT_TRUE(s2->isComposite());
        

    }

    void test_anonymous_transitions()
    {
        setupTest();
        eStatus status;
        // EVENT_G: S1 -> U (external)
        status = testHSM.dispatch(eEVENT_G, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_U));

        // EVENT_A: U -> V (external)
        // (anonymous): V -> X (external)
        // (anonymous): X -> S (external)
        status = testHSM.dispatch(eEVENT_A, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_U));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_V));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_X));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));

        TEST_ASSERT_EQUAL(2, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(2, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_v.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_x.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_u.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_v.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_x.getExitCount());
    }

    void test_transition_a()
    {
        setupTest();
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
        setupTest();
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
        setupTest();
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
        setupTest();
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
        setupTest();
        eStatus status;
        status = testHSM.dispatch(eEVENT_E, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_U));
        TEST_ASSERT_EQUAL(2, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_u.getExitCount());
        TEST_ASSERT_TRUE(testCTX.getFlag());

        status = testHSM.dispatch(eEVENT_F, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_U));
        TEST_ASSERT_EQUAL(2, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_u.getExitCount());
        TEST_ASSERT_TRUE(testCTX.getFlag());

        status = testHSM.dispatch(eEVENT_E, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_U));
        TEST_ASSERT_EQUAL(2, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(2, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(0, testHSM.state_u.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());

        status = testHSM.dispatch(eEVENT_E, &testCTX);
        TEST_ASSERT_EQUAL(eOK, status);
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S));
        TEST_ASSERT_TRUE(testHSM.inState(eSTATE_S1));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S2));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S21));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_S22));
        TEST_ASSERT_FALSE(testHSM.inState(eSTATE_U));
        TEST_ASSERT_EQUAL(3, testHSM.state_s.getEntryCount());
        TEST_ASSERT_EQUAL(2, testHSM.state_s1.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getEntryCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_u.getEntryCount());
        TEST_ASSERT_EQUAL(2, testHSM.state_s.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s1.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s2.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s21.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_s22.getExitCount());
        TEST_ASSERT_EQUAL(1, testHSM.state_u.getExitCount());
        TEST_ASSERT_FALSE(testCTX.getFlag());
    }

    void test_transition_f()
    {
        setupTest();
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
        setupTest();
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
    void run_basic_tests(void)
    {
        RUN_TEST(test_initial_configuration);
        RUN_TEST(test_anonymous_transitions);
        RUN_TEST(test_state_functions);
        RUN_TEST(test_transition_a);
        RUN_TEST(test_transition_b);
        RUN_TEST(test_transition_c);
        RUN_TEST(test_transition_d);
        RUN_TEST(test_transition_e);
        RUN_TEST(test_transition_f);
        RUN_TEST(test_transition_g);
    }
}

