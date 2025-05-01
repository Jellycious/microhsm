#ifndef _H_MICROHSM_HSM
#define _H_MICROHSM_HSM

#include <microhsm/State.hpp>

namespace microhsm
{

    /// @brief Dispatch return status
    enum e_status_dispatch {
        DISPATCH_EVENT_IGNORED,
        DISPATCH_EVENT_CONSUMED,
        DISPATCH_ERROR,
    };

    class HSM
    {
        public:

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
             */
            virtual void init(void) = 0;

            virtual State* get_state(unsigned int ID) = 0;

        protected:
            State* cur_state;
    };
}

#endif /* _H_MICROHSM_HSM */
