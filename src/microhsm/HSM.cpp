#include <microhsm/HSM.hpp>

namespace microhsm
{

    void HSM::init(void)
    {
        return;
    }

    e_status_dispatch HSM::dispatch(unsigned int signal, void* ctx)
    {
        return DISPATCH_EVENT_IGNORED;
    }
}
