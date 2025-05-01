#ifndef _H_MICROHSM_HSM
#define _H_MICROHSM_HSM

#include <microhsm/State.hpp>

namespace microhsm
{
    #define EVENT_ANONYMOUS 0

    /// @brief Dispatch return status
    enum e_status_dispatch {
        DISPATCH_EVENT_IGNORED,
        DISPATCH_EVENT_CONSUMED,
        DISPATCH_ERROR,
    };

    class HSM
    {
        public:

            explicit HSM(State* init_state) :
                cur_state(init_state),
                init_state(init_state) {};

            /**
             * @brief Dispatch event to HSM
             * @param signal Event to dispatch
             * @param ctx Pointer to context object
             * @retval DISPATCH_EVENT_IGNORED Event didn't match a transition
             * @retval DISPATCH_EVENT_CONSUMED Event matched a transition
             * @retval DISPATCH_ERROR Error occurred during dispatch
             */
            e_status_dispatch dispatch(unsigned int signal, void* ctx);

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
            virtual State* get_state(unsigned int ID) = 0;

            /**
             * @brief Get current state of HSM
             * @return Current state of HSM
             */
            State* get_current_state(void);

            /**
             * @brief Check whether HSM is in state
             * @param ID ID of state
             * @return Whether the cur_state or one of its parents is `ID`
             */
            bool in_state(unsigned int ID);

        protected:

            State* init_state;
            State* cur_state;
    };
}

#endif /* _H_MICROHSM_HSM */
