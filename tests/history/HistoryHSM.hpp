#ifndef _H_MICROHSM_TESTS_HISTORYHSM
#define _H_MICROHSM_TESTS_HISTORYHSM

#include <microhsm/microhsm.hpp>
#include <microhsm/macros.hpp>

using namespace microhsm;

namespace microhsm_tests
{

    HSM_CREATE_EVENT_LIST(e_hevents,
            eHEVENT_A,
            eHEVENT_B,
    )

    HSM_CREATE_ID_LIST(e_hids,
            STATE_H1,
            STATE_H2,
            STATE_H21,
            STATE_H22,
            STATE_U,
            STATE_H_DEEP_HISTORY,
            STATE_H_SHALLOW_HISTORY
    )

    class HistoryHSM
    {
        public:
            HistoryHSM();
    };

}

#endif
