#include <microhsm/microhsm.hpp>

/*
 * This is an example on how to construct a state machine without the use of macros.
 * This is the most expressive form of constructing state machines, but can have quite
 * some boilerplate code for larger machines.
 *
 * The state machine that is modelled here is the Valve state machine as can be found in
 * `example/valve.png`.
 *
 * Features that are illustrated here are:
 *  - Non-composite states
 *  - Composite states
 *  - Entry/exit behavior
 *  - Tranistion effects
 *  - Guards
 */

namespace microhsm_examples {

    /*
     * First we define all possible states IDs that the state machine can have.
     * Every state ID is of type unsigned int. Every state in the state machine
     * should have its own unique ID.
     */
    enum eValveState : unsigned int {
        eSTATE_IDLE = 0,
        eSTATE_RUNNING,
        eSTATE_CLOSED,
        eSTATE_OPEN,
        eSTATE_COUNT
    };

    /*
     * Second we define all possible events that the state machine can have.
     * Every event is of type unsigned int. Important is that the event
     * with value 0 is reserved for anonymous transitions. The dispatcher
     * will use this specific event id to model any anonymous transitions
     * which can be taken after a non-anonymous transition gets triggered.
     */
    enum eValveEvent : unsigned int {
        eEVENT_ANONYMOUS = 0, // Reserved event ID 0 for anonymous events
        eEVENT_START,
        eEVENT_TICK,
        eEVENT_PAUSE,
    };


    /*
     * The third step is to declare all states that a state machine can have.
     *
     * Here we define the first state.
     * This state is a top-level non-composite state.
     * It has neither a parent nor children.
     *
     * Every state must also define a `match` function.
     * This is the function that models the transition table which
     * determines whether an event will result in a transition outgoing
     * from this state.
     *
     * Optionally states can define a function `void entry(void* ctx)`
     * or `void exit(void* ctx)`; these get executed upon entering and
     * exiting the state respectively.
     */
    class StateIdle : public microhsm::BaseState
    {
        public:
            StateIdle() : microhsm::BaseState(
                    eSTATE_IDLE,        // ID of StateIdle
                    nullptr,            // Pointer to parent state of `StateIdle`
                    nullptr)            // Pointer to initial state of `StateIdle`
            {
            };

            // Obligatory `match` function.
            bool match(unsigned int event, microhsm::sTransition* t, void* ctx) override;
    };

    class StateRunning : public microhsm::BaseState
    {
        public:
            explicit StateRunning(microhsm::BaseState* initialState) : microhsm::BaseState(
                    eSTATE_RUNNING,     // ID of `StateRunning`
                    nullptr,            // Pointer to parent state of `StateRunning`
                    initialState)       // Pointer to initial state of `StateRunning`
            {
            };

            bool match(unsigned int event, microhsm::sTransition* t, void* ctx) override;
    };

    class StateOpen : public microhsm::BaseState
    {
        public:
            explicit StateOpen(StateRunning* parentState) : microhsm::BaseState(
                    eSTATE_OPEN,        // ID of `StateOpen`
                    parentState,        // Pointer to parent state of `StateOpen`
                    nullptr)            // Pointer to initial state of `StateOpen`
            {
            };

            bool match(unsigned int event, microhsm::sTransition* t, void* ctx) override;

            // Optional entry function
            void entry(void* ctx) override;
    };

    class StateClosed : public microhsm::BaseState
    {
        public:
            explicit StateClosed(StateRunning* parentState): microhsm::BaseState(
                    eSTATE_CLOSED,      // ID of `StateClosed`
                    parentState,        // Pointer to parent state of `StateClosed`
                    nullptr)            // Pointer to initial state of `StateClosed`
            {
            };

            bool match(unsigned int event, microhsm::sTransition* t, void* ctx) override;

            // Optional entry function
            void entry(void* ctx) override;
    };

    /*
     * The fourth step is to define the HSM itself.
     *
     * Every HSM will contain a set of states. These will be states
     * declared above.
     *
     * It must inherit from microhsm::BaseHSM and override two functions
     * - `microhsm::Vertex* getVertex(unsigned int ID)`
     * - `unsigned int getMaxID()`
     *
     * These functions are used by the dispatcher two appropriately match
     * and execute state transitions.
     *
     * `getVertex` will return a pointer to the state based upon the give id.
     * `getMaxID` will return the highest ID among the HSMs states.
     */
    class ValveHSM : public microhsm::BaseHSM
    {
        public:
            /*
             * The constructor of `ValveHSM`
             * provides the initial state to the `BaseHSM`.
             */
            ValveHSM() : microhsm::BaseHSM(state_idle) {}

            /* Required functions */
            microhsm::Vertex* getVertex(unsigned int ID) override;
            unsigned int getMaxID(void) override;

        private:

            /* States */
            StateIdle state_idle = StateIdle();
            StateRunning state_running = StateRunning(&state_closed);     // Argument is the initial state of StateRunning
            StateOpen state_open = StateOpen(&state_running);           // Argument is the parent state of StateOpen
            StateClosed state_closed = StateClosed(&state_running);     // Argument is the parent state of StateOpen
    };

    /*
     * Lastly we can declare a context object.
     * Context objects are used to interact with elements outside of the HSM.
     *
     * The context object is passed to transition effects, `match`, `entry`, and `exit` functions.
     * The context object is a passed as a void pointer (`void*`).
     *
     * In this example the context object `ValveContext` manages the valves state.
     * One can open and close the valve, but also lock it. When the valve is locked,
     * it is not allowed to open, but still allowed to close.
     */
    class ValveContext
    {
        public:
            /* --- Public API --- */
            /// @brief Lock the valve. When locked is not allowed to open.
            void lock();
            /// @brief Unlock the valve. When unlocked the valve is allowed to open.
            void unlock();

            /* --- Used by HSM --- */
            /**
             * @brief Whether the valve is locked
             * @retval `true` Valve is locked
             * @retval `false` Valve is unlocked
             */
            bool isLocked();
            /**
             * @brief Open valve
             */
            void open();
            /**
             * @brief Close valve
             */
            void close();

        private:
            bool locked_ = false;
    };
}
