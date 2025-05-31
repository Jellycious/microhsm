#ifndef _H_MICROHSM_TESTS_HISTORYHSM
#define _H_MICROHSM_TESTS_HISTORYHSM

#include "microhsm/objects/History.hpp"
#include <microhsm/microhsm.hpp>
#include <microhsm/macros.hpp>

using namespace microhsm;

namespace microhsm_tests
{

    HSM_CREATE_EVENT_LIST(e_hevents,
            eHEVENT_A,
            eHEVENT_B,
            eHEVENT_C,
    )

    HSM_CREATE_VERTEX_LIST(e_hids,
            eSTATE_H = 14,
            eSTATE_H_SHALLOW_HISTORY,
            eSTATE_H_DEEP_HISTORY,
            eSTATE_H1,
            eSTATE_H11,
            eSTATE_H12,
            eSTATE_H2,
            eSTATE_H21,
            eSTATE_H22,
            eSTATE_I
    )

    HSM_DECLARE_STATE_TOP_LEVEL(StateH, eSTATE_H)
    HSM_DECLARE_STATE(StateH1, eSTATE_H1, StateH)
    HSM_DECLARE_STATE(StateH11, eSTATE_H11, StateH1)
    HSM_DECLARE_STATE(StateH12, eSTATE_H12, StateH1)
    HSM_DECLARE_STATE(StateH2, eSTATE_H2, StateH)
    HSM_DECLARE_STATE(StateH21, eSTATE_H21, StateH2)
    HSM_DECLARE_STATE(StateH22, eSTATE_H22, StateH2)

    HSM_DECLARE_STATE_TOP_LEVEL(StateI, eSTATE_I)

    class HistoryHSM : public BaseHSM
    {
        public:
            HistoryHSM() : BaseHSM(stateI_) {};

            Vertex* getVertex(unsigned int id) override;
            unsigned int getMaxID(void) override;

        private:
            StateH stateH_ = StateH(&stateH1_, &stateHShallowHistory_, &stateHDeepHistory_);
            ShallowHistory stateHShallowHistory_ = ShallowHistory(eSTATE_H_SHALLOW_HISTORY, &stateH2_);
            DeepHistory stateHDeepHistory_ = DeepHistory(eSTATE_H_DEEP_HISTORY, nullptr);
            StateH1 stateH1_ = StateH1(&stateH_, &stateH11_);
            StateH11 stateH11_ = StateH11(&stateH1_, nullptr);
            StateH12 stateH12_ = StateH12(&stateH1_, nullptr);
            StateH2 stateH2_ = StateH2(&stateH_, &stateH21_);
            StateH21 stateH21_ = StateH21(&stateH2_, nullptr);
            StateH22 stateH22_ = StateH22(&stateH2_, nullptr);
            StateI stateI_ = StateI(nullptr);

            /**
             * @brief Mapping of IDs to states
             * `getVertex(unsigned int)` expects this list entries
             * to be stored in the same order as the enumerations
             */
            Vertex* vertices_[10] = {
                &stateH_,
                &stateHShallowHistory_,
                &stateHDeepHistory_,
                &stateH1_,
                &stateH11_,
                &stateH12_,
                &stateH2_,
                &stateH21_,
                &stateH22_,
                &stateI_,
            };
    };
}

#endif
