#include <microhsm/HSM.hpp>
#include <microhsm/State.hpp>

using namespace microhsm;

namespace microhsm_tests
{
    /// @brief Event enumerations
    enum e_events : unsigned int {
        eEVENT_ANONYMOUS = 0,
        eEVENT_A,
        eEVENT_B,
        eEVENT_C,
    };

    /// @brief State enumerations
    enum e_states : unsigned int {
        eSTATE_S = 0,
        eSTATE_S1,
        eSTATE_S2,
        eSTATE_COUNT,
    };


    /* State Declarations */
    class StateS : public State
    {
        public:
            StateS(State* initial) : State(eSTATE_S, nullptr, initial) {};
    };

    class StateS1 : public State
    {
        public:
            StateS1(StateS* parent) : State(eSTATE_S1, parent, nullptr) {};
    };

    class StateS2 : public State
    {
        public:
            StateS2(StateS* parent) : State(eSTATE_S2, parent, nullptr) {};
    };

    /* HSM Declaration */
    class TestHSM : public HSM
    {
    public:

        void init(void) override;

        State* get_state(unsigned int ID) override
        {
            if (ID >= eSTATE_COUNT) {
                return nullptr;
            }
            else {
                return states[ID];
            }
        }

        StateS state_s = StateS(&state_s1);
        StateS1 state_s1 = StateS1(&state_s);
        StateS2 state_s2 = StateS2(&state_s);

        State* states[eSTATE_COUNT] = {
            &state_s,
            &state_s1,
            &state_s2
        };
    };
}
