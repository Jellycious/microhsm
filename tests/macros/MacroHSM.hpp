/**
 * @file MacroHSM.hpp
 * @brief Test HSM defined using macros
 */
#ifndef _H_MICROHSM_TESTS_MACROHSM
#define _H_MICROHSM_TESTS_MACROHSM

#include <microhsm/microhsm.hpp>
#include <microhsm/macros.hpp>

using namespace microhsm;

namespace microhsm_tests
{
    /// @brief Event enumerations
    HSM_CREATE_EVENT_LIST(e_mevents,
            eMEVENT_A,
            eMEVENT_B,
            eMEVENT_C,
            eMEVENT_D,
            eMEVENT_E,
            eMEVENT_F,
            eMEVENT_G,
    )

    HSM_CREATE_ID_LIST(e_mstates,
            eMSTATE_S = 8,
            eMSTATE_S1,
            eMSTATE_S2,
            eMSTATE_S21,
            eMSTATE_S22,
            eMSTATE_U
    )

    /* State Declarations */
    // Declare base class
    HSM_DECLARE_BASE_STATE(MBaseState,
        HSM_DECLARE_STATE_INIT()
        HSM_DECLARE_STATE_ENTRY()
        HSM_DECLARE_STATE_EXIT()
        HSM_DECLARE_MEMBER(unsigned int getEntryCount())
        HSM_DECLARE_MEMBER(unsigned int getExitCount())
    private:
        HSM_DECLARE_MEMBER(unsigned int entryCount_ = 0)
        HSM_DECLARE_MEMBER(unsigned int exitCount_ = 0)
    )

    // Declare states
    HSM_DECLARE_STATE_TOP_LEVEL_FROM_BASE(MStateS, MBaseState)
    HSM_DECLARE_STATE_FROM_BASE(MStateS1, MStateS, MBaseState)
    HSM_DECLARE_STATE_FROM_BASE(MStateS2, MStateS, MBaseState)
    HSM_DECLARE_STATE_FROM_BASE(MStateS21, MStateS2, MBaseState)
    HSM_DECLARE_STATE_FROM_BASE(MStateS22, MStateS2, MBaseState)

    HSM_DECLARE_STATE_TOP_LEVEL_FROM_BASE(MStateU, MBaseState)

    /* HSM Declaration */
    class MacroHSM : public HSM
    {
    public:

        MacroHSM() : HSM(&state_s) {};

        Vertex* getVertex(unsigned int ID) override;
        unsigned int getMaxID() override;

        MStateS state_s         = MStateS(eMSTATE_S, &state_s1);
        MStateS1 state_s1       = MStateS1(eMSTATE_S1, &state_s, nullptr);
        MStateS2 state_s2       = MStateS2(eMSTATE_S2, &state_s, &state_s21);
        MStateS21 state_s21     = MStateS21(eMSTATE_S21, &state_s2, nullptr);
        MStateS22 state_s22     = MStateS22(eMSTATE_S22, &state_s2, nullptr);
        MStateU state_u         = MStateU(eMSTATE_U, nullptr);

        State* states[6] = {
            &state_s,
            &state_s1,
            &state_s2,
            &state_s21,
            &state_s22,
            &state_u
        };
    };
}

#endif
