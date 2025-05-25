/**
 * @file BaseHSM.hpp
 * @brief Base for Hierarchical State Machines
 *
 * @author Jelle Meijer
 * @date 2025-05-23
 */

#ifndef _H_MICROHSM_HSM
#define _H_MICROHSM_HSM

#include <microhsm/objects/BaseState.hpp>

namespace microhsm
{
    #define EVENT_ANONYMOUS 0

    /**
     * @enum eStatus
     * @brief Dispatch return status
     */
    enum eStatus {
        eOK = 0,            ///< Event consumed
        eEVENT_IGNORED,     ///< Event ignored
        eTRANSITION_ERROR,  ///< A critical error occurred
    };

    /**
     * @class BaseHSM
     * @brief Base class for hierarchical state machines
     *
     * The `BaseHSM` is used as the base for any hierarchical state machine.
     * Its function is to hold the states (and other vertices). It tracks the
     * active state and is responsible for dispatching events accordingly.
     *
     * Derived classes must implement two functions:
     *  - `Vertex* getVertex(unsigned int ID)`
     *  - `unsigned int getMaxID(void)`
     *
     *  `getVertex` is used to obtain a `Vertex` based upon a unique ID.
     *  Every ID must be uniquely associated to a single `Vertex`. If the ID
     *  is not associated to any vertex, `nullptr` should be returned.
     *
     *  `getMaxID` is used to obtain the highest possible ID. This is used
     *  by `BaseHSM` to iterate over all possible state. Mainly used during
     *  initialization.
     */
    class BaseHSM
    {
        public:

            /**
             * @brief HSM constructor.
             * @param initState Initial state of HSM
             */
            explicit BaseHSM(BaseState* initial);

            /**
             * @brief Destructor.
             */
            virtual ~BaseHSM();

            /**
             * @brief Get state.
             * @param ID Unique Identifier of state
             * @retval `State*` associated to provided ID
             * @retval `nullptr` If state doesn't exist
             */
            virtual Vertex* getVertex(unsigned int ID) = 0;

            /**
             * @brief Return state ID with highest value.
             * This is used by `HSM` to iterate over all states together
             * with help of the `getVertex(ID)` function.
             * @return Number of states
             */
            virtual unsigned int getMaxID(void) = 0;

            /**
             * @brief Initialize HSM.
             * @param Context object
             * Set the hsm to the underlying states
             */
            void init(void* ctx);

            /**
             * @brief Dispatch event to HSM.
             * @param event Event to dispatch
             * @param ctx Pointer to context object
             * @retval eOK Event matched a transition
             * @retval eDISPATCH_EVENT_IGNORED Event didn't match a transition
             * @retval eTRANSITION_ERROR Error occurred during dispatch
             */
            eStatus dispatch(unsigned int event, void* ctx);

            /**
             * @brief Get current state of HSM.
             * @return Current state of HSM
             */
            BaseState* getCurrentState(void);

            /**
             * @brief Check whether HSM is in state.
             * @param ID ID of state
             * @return Whether the current state or one of its parents has `ID`
             */
            bool inState(unsigned int ID);

        protected:

            /**
             * @brief Initialization callback.
             * Called after initialization of HSM.
             * Provides user with hook for adding custom behavior.
             * @param ctx Context object
             */
            virtual void init_(void* ctx) {(void)ctx;};

            /// @brief Current active state (always a leaf state)
            BaseState* curState;
            /// @brief Initial state (`nullptr` for leaf states)
            BaseState* initState;

        private:

            /* --- Private Static Functions --- */

            /**
             * @brief Perform effect action
             * @note Will ignore effect if t->effect is `nullptr`
             * @param t Pointer to transition descriptor
             * @param ctx Context object
             */
            static void performEffect_(const sTransition* t, void* ctx);

            /**
             * @brief Update the shallow and deep history of parent states
             * @param newState The state that has newly been activated
             */
            static void updateHistories_(BaseState* newState);

            /**
             * @brief Perform internal transition on current state
             * @param t Pointer to transition description
             * @param ctx Context object
             * @return eStatus
             */
            static eStatus performTransitionInternal_(const sTransition* t, void* ctx);

            /**
             * @brief Find least common ancestor
             * @note As side-effect creates a path from LCA to `b`
             * @param a Pointer to first state
             * @param b Pointer to second state
             * @return Least common ancestor, `nullptr` if LCA doesn't exist
             */
            static BaseState* findLCA_(BaseState* a, BaseState* b);

            /* --- Private Member Functions --- */

            /**
             * @brief Exits states until the `target` state is reached.
             * @param start Pointer to state from where to start from
             * @param target Pointer to target state
             * @param ctx Context object
             * @return Pointer to state if found, otherwise `nullptr`
             */
            BaseState* exitUntilTarget_(BaseState* start, BaseState* target, void* ctx);

            /**
             * @brief Perform entry effects until `target` (including)
             * @note Does not perform entry behavior of `startState`
             * @param startState State to start from (can be `nullptr`)
             * @param targetState Final state to enter (cannot be `nullptr`)
             * @param Context object
             * @return `targetState` if successfull, `nullptr` otherwise. 
             */
            BaseState* enterUntilTarget_(BaseState* startState, BaseState* targetState, void* ctx);

            /**
             * @brief Enter a statertementen geldt vaak een VvE-reglement waarin be
             * @param s State
             * @param ctx Context object
             */
            void enterState_(BaseState* s, void* ctx);

            /**
             * @brief Exit state
             * @param s State
             * @param ctx Context object
             */
            void exitState_(BaseState* s, void* ctx);

            /**
             * @brief Enter initial pseudostates
             * @note Will continue until the last possible initial state
             * @param s State
             * @param ctx Context object
             * @return Last state that was entered
             */
            BaseState* enterInitialStates_(BaseState* s, void* ctx);

            /**
             * @brief Get target of transition
             * Determines target by evaluating the `targetID` of transition
             * There is the possibility that the `targetID` is of type:
             * `ePSEUDO_HISTORY` in which case the last set history state
             * will be returned as the transition target.
             * @param targetID of target
             */
            BaseState* getTransitionTarget_(unsigned int targetID);

            /**
             * @brief Try to match event to State or one of its ancestors
             * @param event Event to match
             * @param t Pointer to transition object.
             * @param ctx Context object
             * @retval `false` No match was found
             * @retval `true` Match was found, `t` will contain transition description
             */
            bool matchStateOrAncestor_(unsigned int event, sTransition* t, void* ctx);

            /**
             * @brief Perform transition on current state
             * @param t Pointer to transition description
             * @param ctx Context object
             * @return eStatus
             */
            eStatus performTransition_(const sTransition* t, void* ctx);

            /**
             * @brief Set the new active state of the HSM
             * @param newState New active state
             */
            void setNewActiveState_(BaseState* newState);

    };
}

#endif /* _H_MICROHSM_HSM */
