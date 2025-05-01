#include <microhsm/HSM.hpp>

namespace microhsm
{

    void HSM::init(void* ctx)
    {
        State* s = this->init_state;

        // Walk down until the lowest initial state
        while (s->initial != nullptr) {
            s = s->initial;
        }

        this->cur_state = s;

        // Handle initial anonymous transitions
        this->dispatch(EVENT_ANONYMOUS, ctx);
    }

    State* HSM::get_current_state()
    {
        return this->cur_state;
    }

    bool HSM::in_state(unsigned int ID)
    {
        const State* s = cur_state;
        while (s != nullptr) {
            if (s->ID == ID) return true;
            s = s->parent;
        }
        return false;
    }

    e_status_dispatch HSM::dispatch(unsigned int signal, void* ctx)
    {
        return DISPATCH_EVENT_IGNORED;
    }

}
