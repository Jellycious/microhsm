/**
 * @file TestHSM.hpp
 * @brief TestHSM used for testing
 * The HSM is based on an example given in the book:
 *      Practical Statecharts in C/C++ by Miro Samek, Ph.D
 */
#ifndef _H_MICROHSM_TESTS_TESTHSM
#define _H_MICROHSM_TESTS_TESTHSM

#include <microhsm/microhsm.hpp>
#include <microhsm/macros.hpp>

using namespace microhsm;

namespace microhsm_tests
{
    /// @brief Event enumerations
    enum e_events : unsigned int {
        eEVENT_ANONYMOUS = 0,
        eEVENT_A,
        eEVENT_B,
        eEVENT_C,
        eEVENT_D,
        eEVENT_E,
        eEVENT_F,
        eEVENT_G,
    };

    enum e_states : unsigned int {
        eSTATE_S = 0,
        eSTATE_S1,
        eSTATE_S2,
        eSTATE_S21,
        eSTATE_S22,
        eSTATE_U,
        eSTATE_V,
        eSTATE_X,
        eSTATE_COUNT
    };

    class TestState : public State
    {
        public:

            TestState(unsigned int stateID, State* parentState, State* initialState) :
                State(stateID, parentState, initialState) {};

            void entry(void* ctx) override;
            void exit(void* ctx) override;

            unsigned int getEntryCount();
            unsigned int getExitCount();

        protected:

            void init_(void* ctx) override;

        private:
            unsigned int entryCount_ = 0;
            unsigned int exitCount_ = 0;
    };


    /* State Declarations */
    class StateS : public TestState
    {
        public:
            StateS(State* initialState) : TestState(eSTATE_S, nullptr, initialState) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateS1 : public TestState
    {
        public:
            StateS1(StateS* parentState) : TestState(eSTATE_S1, parentState, nullptr) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateS2 : public TestState
    {
        public:
            StateS2(StateS* parentState, State* initialState) : TestState(eSTATE_S2, parentState, initialState) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateS21 : public TestState
    {
        public:
            StateS21(StateS2* parentState) : TestState(eSTATE_S21, parentState, nullptr) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateS22 : public TestState
    {
        public:
            StateS22(StateS2* parentState) : TestState(eSTATE_S22, parentState, nullptr) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateU : public TestState
    {
        public:
            StateU() : TestState(eSTATE_U, nullptr, nullptr) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateV : public TestState
    {
        public:
            StateV() : TestState(eSTATE_V, nullptr, nullptr) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateX : public TestState
    {
        public:
            StateX() : TestState(eSTATE_X, nullptr, nullptr) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    /* HSM Declaration */
    class TestHSM : public HSM
    {
    public:

        TestHSM() : HSM(&state_s) {};

        Vertex* getVertex(unsigned int ID) override;
        unsigned int getMaxID() override;

        StateS state_s = StateS(&state_s1);
        StateS1 state_s1 = StateS1(&state_s);
        StateS2 state_s2 = StateS2(&state_s, &state_s21);
        StateS21 state_s21 = StateS21(&state_s2);
        StateS22 state_s22 = StateS22(&state_s2);
        StateU state_u = StateU();
        StateV state_v = StateV();
        StateX state_x = StateX();

        Vertex* vertices[eSTATE_COUNT] = {
            &state_s,
            &state_s1,
            &state_s2,
            &state_s21,
            &state_s22,
            &state_u,
            &state_v,
            &state_x
        };
    };
}

#endif
