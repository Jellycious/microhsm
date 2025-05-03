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

            explicit HSM(State* initState) :
                curState(initState),
                initState(initState) {};

            /**
             * @brief Dispatch event to HSM
             * @param signal Event to dispatch
             * @param ctx Pointer to context object
             * @retval eOK Event matched a transition
             * @retval eDISPATCH_EVENT_IGNORED Event didn't match a transition
             * @retval eTRANSITION_ERROR Error occurred during dispatch
             */
            eStatus dispatch(unsigned int signal, void* ctx);

            /**
             * @brief Initialize HSM
             * @param Context object
             * Set the hsm to the underlying states
             */
            void init(void* ctx);

            /**
             * @brief Get state
             * @param ID Unique Identifier of state
             * @return State associated to ID
             */
            virtual State* getState(unsigned int ID) = 0;

            /**
             * @brief Get current state of HSM
             * @return Current state of HSM
             */
            State* getCurrentState(void);

            /**
             * @brief Check whether HSM is in state
             * @param ID ID of state
             * @return Whether the cur_state or one of its parents is `ID`
             */
            bool inState(unsigned int ID);

        protected:

            State* initState;
            State* curState;

        private:

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

            /**
             * @brief Exits states until the `target` state is reached.
             * @param start Pointer to state from where to start from
             * @param target Pointer to target state
             * @param ctx Context object
             * @return Pointer to state if found, otherwise `nullptr`
             */
            static State* exitUntilTarget_(State* start, State* target, void* ctx);

            /**
             * @brief Perform enter effects up to and including target state
             * @note Does not perform enter effect of startState
             * @param startState Pointer to state to start from
             * @param targetState Final state to enter
             * @param Context object
             * @return `targetState` if successfull, `nullptr` otherwise. 
             */
            static State* enterUntilTarget_(State* startState, State* targetState, void* ctx);

            /**
             * @brief Find least common ancestor
             * @param a Pointer to first state
             * @param b Pointer to second state
             * @return Least common ancestor, `nullptr` if LCA doesn't exist
             */
            static State* findLCA_(State* a, State* b);



            
    };
}

#endif /* _H_MICROHSM_HSM */
