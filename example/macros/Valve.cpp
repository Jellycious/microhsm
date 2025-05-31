#include "Valve.hpp"
#include "microhsm/macros.hpp"

#include <iostream>

/*
 * This is an example on how to define the following elements required
 * for hierarchical state machines with the `microhsm` library
 *
 *  1. Implementing required functions for a class derived from `BaseHSM`
 *  2. Implementing state transition using `match` function.
 *  3. Implementing `entry`/`exit` functions for states
 *  4. Implementing transition effects
 *  5. Implementing a mock HSM context object
 *
 */

#define UNUSED_ARG(x) (void) x

namespace microhsm_examples {

    /*
     * First we define the two obligatory functions for the HSM
     *
     * One only has to define two functions for a HSM to be derived:
     *
     * `unsigned int getMaxID()`
     *      Return the highest possible ID of a state
     * `Vertex* getVertex(unsigned int id)`
     *      Return a pointer to a vertex based on the ID, if the vertex does
     *      not exist, then nullptr should be returned.
     *
     *  With these functions definitions the event dispatcher will know
     *  how to properly dispatch events
     */
    unsigned int ValveHSM::getMaxID()
    {
        // Here we simply return the highest possible ID for a state
        return static_cast<unsigned int>(eSTATE_COUNT) - 1;
    }

    /*
     * The `getVertex` function is used to obtain a pointer to a state
     * associated to an ID.
     */
    microhsm::Vertex* ValveHSM::getVertex(unsigned int id)
    {
        // In this case modelled as a simple switch case
        switch(id) {
            case eSTATE_IDLE: return &this->state_idle;
            case eSTATE_RUNNING: return &this->state_running;
            case eSTATE_OPEN: return &this->state_open;
            case eSTATE_CLOSED: return &this->state_closed;
            default: return nullptr;
        }
    }

    /*
     * Second we define the `match` and `entry`/`exit` functions for the state machines
     *
     * The `match` function is responsible for handling events passed to a state.
     * It should check guards and set the transition object `sTransition` to the desired
     * transition. Utility functions are provided to facilitate setting the three types of
     * transitions. If no transition is desired, one can simply return false.
     *
     * One has access to the following parameters in the `match` function:
     *
     * - 'unsigned int event`
     * - `microhsm::sTransition* t`
     * - `void* ctx`
     */
    /* --- `StateIdle` --- */
    HSM_DEFINE_STATE_MATCH(StateIdle)
    {

        UNUSED_ARG(ctx); // prevent compiler warning about unused `ctx` parameter
        switch(event) {
            case eEVENT_START:
                /*
                 * External transition from `StateIdle` to state with ID
                 * `eSTATE_RUNNING` (which is `StateRunning`).
                 *
                 * `t` is the transition object, which will contain the
                 * description of the transition that is desired.
                 *
                 * The last argument is the transition effect. Since no effect
                 * is desired, it is left as `nullptr`.
                 */
                return transitionExternal(eSTATE_RUNNING, t, nullptr);
            default:
                /*
                 * All other events should be ignored and not result in a transition
                 */
                return noTransition();
        }
    }

    /* --- `StateRunning` --- */
    HSM_DEFINE_STATE_MATCH(StateRunning)
    {

        UNUSED_ARG(ctx); // Prevent compiler warning about unused argument

        /*
         * One way of defining effects is by making them as captureless
         * lambda functions.
         *
         * An alternative approach would be to define a static function on `ValveHSM`
         * That takes a void* as argument and does the same thing as this lambda.
         * This is the preferred approach if the effect should be shared among states.
         */
        auto effect = [](void* context) {
            ValveContext* valve = static_cast<ValveContext*>(context);
            valve->close();
        };

        // Define transition table
        switch(event) {
            case eEVENT_PAUSE:
                // Transition from `StateRunning` to `StateIdle` with effect of closing the valve
                return transitionExternal(eSTATE_IDLE, t, effect);
            default:
                // Ignore other events
                return noTransition();
        }
    }

    /* --- `StateClosed` --- */
    HSM_DEFINE_STATE_MATCH(StateOpen)
    {
        UNUSED_ARG(ctx);
        switch(event) {
            case eEVENT_TICK:
                return transitionExternal(eSTATE_CLOSED, t, nullptr);
            default:
                return noTransition();
        }
    }

    /*
     * Here we use a macro to define a state's entry function.
     * For this to be possible on needs to have declared the State class
     * with an entry function. See header file "Valve.hpp".
     *
     * One has access to the following parameters in the `entry` function:
     *
     * - `void* ctx`
     */
    HSM_DEFINE_STATE_ENTRY(StateOpen)
    {
        ValveContext* context = static_cast<ValveContext*>(ctx);
        context->open();
    }

    /* --- `StateClosed` --- */
    HSM_DEFINE_STATE_MATCH(StateClosed)
    {
        ValveContext* valve = static_cast<ValveContext*>(ctx);
        // Define transition table
        switch(event) {
            case eEVENT_TICK:
                // Guards must be implemented manually by checking them in the `match` function.
                if (!valve->isLocked()) {
                    // Transition from `StateClosed` to `StateOpen`, if guard valve->canOpen succeeds.
                    return transitionExternal(eSTATE_OPEN, t, nullptr);
                }
                break;
            default:
                break;
        }
        return noTransition();
    }

    HSM_DEFINE_STATE_ENTRY(StateClosed)
    {
        ValveContext* context = static_cast<ValveContext*>(ctx);
        context->close();
    }

    /*
     * Thirdly we define the ValveContext. This is just for illustration purposes on
     * how a context object could be used. For this example we have the valve as an
     * object that can open and close the valve. Furthermore, the valve can be locked
     * using the `lock` function. If the valve is locked, it is not allowed to go into
     * the open state.
     */
    void ValveContext::lock()
    {
        std::cout << "ValveContext::lock()" << std::endl;
        locked_ = true;
    }
    void ValveContext::unlock()
    {
        std::cout << "ValveContext::unlock()" << std::endl;
        locked_ = false;
    }
    bool ValveContext::isLocked()
    {
        return locked_;
    }
    void ValveContext::open()
    {
        std::cout << "ValveContext::open()" << std::endl;
    }
    void ValveContext::close()
    {
        std::cout << "ValveContext::close()" << std::endl;
    }
}
