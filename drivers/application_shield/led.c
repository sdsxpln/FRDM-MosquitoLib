#include "MK64F12.h"

void frdm_as_led_init() {
    // Power on
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTA_MASK;
    SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
    SIM_SOPT2 |= SIM_SOPT2_FTM0SRC(1);

    //NOTE: multiplexing port pat0 will cause cpu to hard fault on revision other then E
    // because it's also routed to the debugger
    //PORTA_PCR0 = PORT_PCR_MUX(3);  // FTM0_CH5
    PORTA_PCR2 = PORT_PCR_MUX(3);  // FTM0_CH7
    PORTC_PCR4 = PORT_PCR_MUX(4);  // FTM0_CH4

    FTM0_MOD  = 99;
    FTM0_C4SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;
    //FTM0_C5SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;
    FTM0_C7SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;
    FTM0_SC   = FTM_SC_CMOD(1) | FTM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
}

void frdm_as_led_toggle_R() {
}

void frdm_as_led_toggle_G() {
}

void frdm_as_led_toggle_B() {
}
