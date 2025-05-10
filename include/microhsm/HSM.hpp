#ifndef _H_MICROHSM_HSM
#define _H_MICROHSM_HSM

#include <microhsm/State.hpp>

namespace microhsm
{
    #define EVENT_ANONYMOUS 0

    /// @brief Dispatch return status
    enum eStatus {
        eOK = 0,
        eEVENT_IGNORED,
        eTRANSITION_ERROR,
        eSTRUCTURE_ERROR
    };

    class HSM
    {
        public:

            /**
             * @brief HSM constructor
             * @param initState Initial state of HSM
             */
            explicit HSM(State* initial) :
                curState(initial),
                initState(initial) {};

            /**
             * @brief Get state
             * @param ID Unique Identifier of state
             * @retval `State*` associated to provided ID
             * @retval `nullptr` If state doesn't exist
             */
            virtual State* getState(unsigned int ID) = 0;

            /**
             * @brief Return state ID with highest value
             * This is used by `HSM` to iterate over all states together
             * with help of the `getState(ID)` function.
             * @return Number of states 
             */
            virtual unsigned int getMaxStateID(void) = 0;

            /**
             * @brief Dispatch event to HSM
             * @param event Event to dispatch
             * @param ctx Pointer to context object
             * @retval eOK Event matched a transition
             * @retval eDISPATCH_EVENT_IGNORED Event didn't match a transition
             * @retval eTRANSITION_ERROR Error occurred during dispatch
             */
            eStatus dispatch(unsigned int event, void* ctx);

            /**
             * @brief Initialize HSM
             * @param Context object
             * Set the hsm to the underlying states
             */
            void init(void* ctx);

            /**
             * @brief Get current state of HSM
             * @return Current state of HSM
             */
            State* getCurrentState(void);

            /**
             * @brief Check whether HSM is in state
             * @param ID ID of state
             * @return Whether the cur_state or one of its parents has `ID`
             */
            bool inState(unsigned int ID);

        protected:

            /**
             * @brief Initialization callback
             * Called after initialization of HSM.
             * Provides user with hook for addi
             * behavior provided by user
             * @param ctx Context object
             */
            virtual void init_(void* ctx) {(void)ctx;};

            /// @brief Current active state (always a leaf state)
            State* curState;
            /// @brief Initial state (`nullptr` for leaf states)
            State* initState;

        private:

            /**
             * @brief Exits states until the `target` state is reached.
             * @param start Pointer to state from where to start from
             * @param target Pointer to target state
             * @param ctx Context object
             * @return Pointer to state if found, otherwise `nullptr`
             */
            static State* exitUntilTarget_(State* start, State* target, void* ctx);

            /**
             * @brief Perform entry effects until `target` (including)
             * @note Does not perform entry behavior of `startState`
             * @param startState State to start from (can be `nullptr`)
             * @param targetState Final state to enter (cannot be `nullptr`)
             * @param Context object
             * @return `targetState` if successfull, `nullptr` otherwise. 
             */
            static State* enterUntilTarget_(State* startState, State* targetState, void* ctx);

            /**
             * @brief Find least common ancestor
             * @note As side-effect creates a path from LCA to `b`
             * @param a Pointer to first state
             * @param b Pointer to second state
             * @return Least common ancestor, `nullptr` if LCA doesn't exist
             */
            static State* findLCA_(State* a, State* b);

            /**
             * @brief Perform entry action
             * @param s State
             * @param ctx Context object
             */
            static void performEntry_(State* s, void* ctx);

            /**
             * @brief Perform exit action
             * @param s State
             * @param ctx Context object
             */
            static void performExit_(State* s, void* ctx);

            /**
             * @brief Enter initial pseudostates
             * @note Will continue until the last possible initial state
             * @param s State
             * @param ctx Context object
             * @return Last state that was entered
             */
            static State* enterInitialStates_(State* s, void* ctx);

            /**
             * @brief Perform effect action
             * @param t Pointer to transition descriptor
             * @param ctx Context object
             */
            static void performEffect_(sTransition* t, void* ctx);

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
             * @brief Perform internal transition on current state
             * @param t Pointer to transition description
             * @param ctx Context object
             * @return eStatus
             */
            eStatus performTransitionInternal_(const sTransition* t, void* ctx);
    };
}

#endif /* _H_MICROHSM_HSM */
