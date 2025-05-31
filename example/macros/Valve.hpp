#include <microhsm/microhsm.hpp>
#include <microhsm/macros.hpp>

/*
 * This is an example on how to construct a state machine with the use of macros.
 * Macros greatly reduce the amount of boilerplate required, at the cost of hiding
 * some of the implementation details. For an example without macros see `example/basic`.
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
    HSM_CREATE_VERTEX_LIST(eValveState,
        eSTATE_IDLE,
        eSTATE_RUNNING,
        eSTATE_CLOSED,
        eSTATE_OPEN,
        eSTATE_COUNT
    )

    /*
     * Second we define all possible events that the state machine can have.
     * Every event is of type unsigned int. Important is that the event
     * with value 0 is reserved for anonymous transitions. The dispatcher
     * will use this specific event id to model any anonymous transitions
     * which can be taken after a non-anonymous transition gets triggered.
     */
    HSM_CREATE_EVENT_LIST(eValveEvent,
        eEVENT_START,
        eEVENT_PAUSE,
        eEVENT_TICK
    )


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

    /*
     * We declare a top-level (toplevel has no parent) state `StateIdle`.
     *
     * The first argument is the class name of the state
     * The second argument is the unique state id
     *
     * A constructor of a top-level state has the format
     * `CLASS_NAME(microhsm::BaseState* initialState)`
     * Leave `initialState` as `nullptr` for non-composite (leaf) states.
     *
     * A constructor can also be given history nodes if so desired
     * To provide history nodes to use the constructor
     * `CLASS_NAME(microhsm::BaseState* initialState
     *      microhsm::ShallowHistory* shallowHistory
     *      microhsm::DeepHistory* deepHistory)`
     *  Leave the history node as `nullptr` if the respective history
     *  should not be tracked.
     */
    HSM_DECLARE_STATE_TOP_LEVEL(StateIdle, eSTATE_IDLE)
    HSM_DECLARE_STATE_TOP_LEVEL(StateRunning, eSTATE_RUNNING)

    /*
     * Now we declare two child states of `StateRunning`
     * These are not top-level states, so we declare them using
     * `HSM_DECLARE_STATE`
     *
     * The first argument is the class name of the state
     * The second argumnet is the unique state ID
     * The third argument is the class name of the parent
     *
     * The constructor will have forms
     * `CLASS_NAME(PARENT_NAME* parentState, microhsm::BaseState* initialState)`
     * `CLASS_NAME(PARENT_NAME* parentState, microhsm::BaseState* initialState
     *      microhsm::ShallowHistory* shallowHistory, microhsm::DeepHistory* deepHistory)`
     */
    HSM_DECLARE_STATE(StateClosed, eSTATE_CLOSED, StateRunning,
            /*
             * It is possible to declare functions and variables within a state
             * as if your editing a class.
             * Some convenience macros are included for `entry`/`exit` functions
             */

            // StateClosed has an entry function
            HSM_DECLARE_STATE_ENTRY()
    )

    HSM_DECLARE_STATE(StateOpen, eSTATE_OPEN, StateRunning,
            // Note that it is also possible to add functions manually to a state
            void entry(void* ctx) override;
    )


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
            StateIdle state_idle = StateIdle(nullptr);                          // Argument is the initial state (`nullptr` because state has no children)
            StateRunning state_running = StateRunning(&state_closed);           // Argument is the initial state
            StateOpen state_open = StateOpen(&state_running, nullptr);          // Arguments are parent state and initial state respectively
            StateClosed state_closed = StateClosed(&state_running, nullptr);    // Arguments are parent and initial state respectively
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
