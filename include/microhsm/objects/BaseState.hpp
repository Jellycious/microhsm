/**
 * @file BaseState.hpp
 * @brief Contains State declaration/definition.
 * @author Jelle Meijer
 * @date 2025-05-23
 */

#ifndef _H_MICROHSM_STATE
#define _H_MICROHSM_STATE

#include <microhsm/objects/Vertex.hpp>

namespace microhsm
{

    // Forward declaration of history nodes
    // Not included to avoid recursive inclusion
    class ShallowHistory;
    class DeepHistory;

    /**
     * @enum eTransitionKind
     * @brief Enumeration of transition kinds
     *
     * Under UML there are three possible kind of transitions.
     * External/local and internal. They have different behaviors
     * in terms of which states are exited and entered.
     *
     *  - External:
     *      Will always exit the source state and then move towards the target state
     *
     *  - Local:
     *      Under most circumstances behaves the same as external transitions.
     *      However, under the cirumstance that the transition is from a composite state
     *      to a substate of that composite state. The composite state will not be exited
     *      and re-entered again.
     *
     *  - Internal:
     *      An internal transition is always a self transition. Will not result in any states from being
     *      exited or entered, but can perform transition effects.
     */
    enum eTransitionKind {
        eKIND_EXTERNAL,     ///< External transition
        eKIND_LOCAL,        ///< Local transition
        eKIND_INTERNAL      ///< Internal (self) transition
    };

    /// Function type of a transition effect
    typedef void (*fTransitionEffect)(void *ctx);

    /**
     * @brief Transition Struct.
     *
     * Used to represent a transition from a source state with `sourceID`
     * to a target state with `targetID`. Under UML there are three kinds
     * of transitions: external/local and internal. This is represented
     * with the `kind` field. Finally, transitions can execute so called
     * effects, which will be triggered when the transition taken.
     * When no effect is desired, the `effect` field can be left as `nullptr`
     */
    typedef struct {
        unsigned int sourceID;          ///< Source of transition (State)
        unsigned int targetID;          ///< Target of transition (State/History)
        eTransitionKind kind;           ///< Type of transition
        void (*effect) (void* ctx);     ///< Effect of transition
    } sTransition;

    /**
     * @class BaseState
     * @brief Base class for all states
     *
     * Every state of the HSM should be derived from `BaseState`.
     * Base state contains the data members which are used for maintaining
     * the state of the hierarchical state machine and during the dispatching
     * procedure.
     *
     * Derived classes must implement the function:
     *  - `bool match(unsigned int event, sTransition* t, void* ctx)`:
     *      This function encodes the possible transitions that can be taken from
     *      this state.
     *
     * Optional functions that overridden:
     *  - `void entry(void* ctx)`: Optional entry function executed upon entering the state
     *  - `void exit(void* ctx)`: Optional exit function executed upon leaving the state
     *  - `void init_(void* ctx)`: Optional hook called after state initialization
     */
    class BaseState : public Vertex
    {
        // Provide deep-coupling with `HSM` for dispatching purposes
        friend class BaseHSM;

        public:

            /**
             * @brief State constructor
             * @param id Unique ID of state
             * @param parent Parent state (leave `nullptr` for top-level state)
             * @param initial Initial state for composite state (leave `nullptr` for non-composite state)
             */
            BaseState(unsigned int id, BaseState* parent, BaseState* initial);

            /**
             * @brief State constructor
             * @param id Unique ID of state
             * @param parent Parent state (leave `nullptr` for top-level state)
             * @param initial Initial state for composite state (leave `nullptr` for non-composite state)
             * @param shallowHistory Pointer to `History` used for shallow history (`nullptr` if no history node needs to be tracked)
             * @param deepHistory Pointer to `History` used for deep history (`nullptr` if no history node needs to be tracked)
             */
            BaseState(unsigned int id, BaseState* parent, BaseState* initial, ShallowHistory* shallowHistory, DeepHistory* deepHistory);

            /**
             * @brief Destructor
             */
            virtual ~BaseState();

            /**
             * @brief Match event to transition
             * @param event Event that occurred
             * @param t Pointer to transition struct
             * @param ctx Context object
             * @retval `true` Event matched a transition
             * @retval `false` Event did not match any transition
             *
             * Override this function and implement the logic that matches an event and
             * inspects guards. Set the `t` parameter to represent the transition.
             *
             * The recommended way of returning is by returning the result of one of these functions:
             *  - `bool noTransition(void)`:
             *      The event did not match to any transition.
             *  - `bool transitionExternal(unsigned int target_ID, sTransition* t, fTransitionEffect effect)`:
             *      An external transition was matched to the event.
             *  - `bool transitionLocal(unsigned int target_ID, sTransition* t, fTransitionEffect effect)`:
             *      A local transition was matched to the event.
             *  - `bool transitionInternal(sTransition* t, fTransitionEffect effect)`:
             *      An internal transition was matched to the event.
             *
             * The recommended way of implementing guards is by having the `ctx` object
             * implement the guard functions. This way one can inspect a guard function
             * of `ctx` and decide appropriately whether to match a transition or not.
             */
            virtual bool match(unsigned int event, sTransition* t, void* ctx) = 0;

            /**
             * @brief Perform state entry
             * @param ctx Context object
             */
            virtual void entry(void* ctx);

            /**
             * @brief Perform state exit
             * @param ctx Context object
             */
            virtual void exit(void* ctx);

            /**
             * @brief Initialization
             * @param ctx Context object
             */
            void init(void* ctx);

            /**
             * @brief Check whether state is a composite state
             *
             * A state with a non-null initial state is considered composite
             *
             * @retval `true` State is composite
             * @retval `false`: State is non-composite
             */
            bool isComposite(void);

            /**
             * @brief Check whether this state is descendent of `state`
             * @param state State to check against
             * @retval `true` This state is descendent of `state`
             * @retval `false` This state is not descendent of `state`
             */
            bool isDescendentOf(unsigned int ID);

            /**
             * @brief Get ancestor with ID
             * @note The state itself is not considered to be an ancestor
             * @param ID Unique identifier of ancestor
             * @return Pointer to ancestor state or nullptr
             */
            BaseState* getAncestor(unsigned int ID);

            /// Parent state (set to `nullptr` if state is not a substate)
            BaseState* const parent;
            /// Initial state (set to `nullptr` for non-composite states)
            BaseState* const initial;
            /// Depth of state
            const unsigned int depth;


        protected:

            /**
             * @brief Initialization callback
             *
             * Called after state initialization. Provides user
             * with a hook to perform actions after initialization.
             *
             * @param ctx Context object
             */
            virtual void init_(void* ctx) {(void)ctx;}

            /**
             * @brief No transition matched
             * @return `false`
             */
            bool noTransition();

            /**
             * @brief Matched an external transition.
             *
             * An external transition will result in exit/entry behavior
             * of the source vertex/target state to be executed.
             * The target and source can be the same vertex.
             *
             * @param target Pointer to target state of transition
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             * @return `true`
             */
            bool transitionExternal(unsigned int target_ID, sTransition* t, fTransitionEffect effect);

            /**
             * @brief Matched a local transition
             *
             * A transition which will not exit its containing state.
             * Local transitions can only exist within composite states.
             * For local transitions, the target and source vertex cannot be the same.
             *
             * @param target Pointer to target state of transition
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             * @return `true`
             */
            bool transitionLocal(unsigned int target_ID, sTransition* t, fTransitionEffect effect);

            /**
             * @brief Matched an internal transition.
             *
             * An internal transition is a type of self-transition that does not
             * result in any entry/exit behaviors from being executed.
             *
             * @param t Pointer to transition object
             * @param effect Effect to execute upon transition
             * @return `true`
             */
            bool transitionInternal(sTransition* t, fTransitionEffect effect);

        private:

            /**
             * @brief Compute depth of state
             * @return Depth of state
             */
            static unsigned int computeDepth_(BaseState* s);

            /**
             * @brief Temporary pointer used for internal purposes
             *
             * This pointer is used to link states temporarily together.
             * This is used by `HSM` during event dispatching to create
             * paths that are traversed during the activation of transitions.
             */
            BaseState* tmp_ = nullptr;

            /// Shallow history pseudostate pointer
            ShallowHistory* shallowHistory_ = nullptr;
            /// Deep history pseudostate pointer
            DeepHistory* deepHistory_ = nullptr;

            /// Whether state is a composite state
            bool isComposite_ = false;
    };
}

#endif /* _H_MICROHSM_STATE */
