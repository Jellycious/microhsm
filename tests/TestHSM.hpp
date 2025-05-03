/**
 * @file TestHSM.hpp
 * @brief TestHSM used for testing
 * The HSM is based on an example given in the book:
 *      Practical Statecharts in C/C++ by Miro Samek, Ph.D
 */
#include <microhsm/HSM.hpp>
#include <microhsm/State.hpp>

using namespace microhsm;

namespace microhsm_tests
{
    /// @brief Event enumerations
    enum e_events : unsigned int {
        eEVENT_ANONYMOUS = 0, /// @brief 0 is reserved for anonymous events
        eEVENT_A,
        eEVENT_B,
        eEVENT_C,
        eEVENT_D,
        eEVENT_E,
        eEVENT_F,
    };

    /// @brief State enumerations
    enum e_states : unsigned int {
        eSTATE_S = 0,
        eSTATE_S1,
        eSTATE_S2,
        eSTATE_S21,
        eSTATE_S22,
        eSTATE_COUNT,
    };

    class TestState : public State
    {
        public:

            TestState(unsigned int ID, State* parent, State* initial) :
                State(ID, parent, initial) {};

            void entry(void* ctx) override;
            void exit(void* ctx) override;
            void init(void* ctx) override;

            void resetCounts();
            unsigned int getEntryCount();
            unsigned int getExitCount();

        private:
            unsigned int entryCount_ = 0;
            unsigned int exitCount_ = 0;
    };


    /* State Declarations */
    class StateS : public TestState
    {
        public:
            StateS(State* initial) : TestState(eSTATE_S, nullptr, initial) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateS1 : public TestState
    {
        public:
            StateS1(State* parent) : TestState(eSTATE_S1, parent, nullptr) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateS2 : public TestState
    {
        public:
            StateS2(State* parent, State* initial) : TestState(eSTATE_S2, parent, initial) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateS21 : public TestState
    {
        public:
            StateS21(State* parent) : TestState(eSTATE_S21, parent, nullptr) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    class StateS22 : public TestState
    {
        public:
            StateS22(State* parent) : TestState(eSTATE_S22, parent, nullptr) {};
            bool match(unsigned int event, sTransition* t, void* ctx) override;
    };

    /* HSM Declaration */
    class TestHSM : public HSM
    {
    public:

        TestHSM() : HSM(&state_s) {};

        State* getState(unsigned int ID) override;
        unsigned int getStateCount() override;

        StateS state_s = StateS(&state_s1);
        StateS1 state_s1 = StateS1(&state_s);
        StateS2 state_s2 = StateS2(&state_s, &state_s21);
        StateS21 state_s21 = StateS21(&state_s2);
        StateS22 state_s22 = StateS22(&state_s2);

        State* states[eSTATE_COUNT] = {
            &state_s,
            &state_s1,
            &state_s2,
            &state_s21,
            &state_s22
        };
    };
}
