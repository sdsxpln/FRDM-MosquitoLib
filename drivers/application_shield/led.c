#include "MK64F12.h"

/*******************************************************
 * Led module
 * The led is routed as followed:
 * R       PTA0
 * G       PTC4
 * B       PTA2
 *
 * FlexTimers with PWM functionallity are used to controll
 * the intensity of the colors
 **********************************************************/
static uint32_t _R = 0;
static uint32_t _G = 0;
static uint32_t _B = 0;

static inline void RGB_LED(uint32_t red, uint32_t green, uint32_t blue) {
    //FTM0_C5V = red;
    FTM0_C3V = green;
    FTM0_C7V = blue;
    _R = red;
    _G = green;
    _B = blue;
}

void frdm_as_led_init() {
    // Power on
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTA_MASK;
    SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
    SIM_SOPT4 |= SIM_SOPT4_FTM0TRG0SRC(1);

    //NOTE: multiplexing port pat0 will cause cpu to hard fault on revision other then E
    // because it's also routed to the debugger
    //PORTA_PCR0 = PORT_PCR_MUX(3);  // FTM0_CH5
    PORTC_PCR4 = PORT_PCR_MUX(4);  // FTM0_CH3
    PORTA_PCR2 = PORT_PCR_MUX(3);  // FTM0_CH7

    RGB_LED(0,0,0);
    // Make write_protected registers writable
    FTM0_MODE |= FTM_MODE_WPDIS_MASK;
    FTM0_MODE &= ~ FTM_MODE_FTMEN_MASK;
    // Overflow at 100
    FTM0_MOD  = 100;
    FTM0_CNTIN = 0;
    FTM0_CNT = 0;
    FTM0_C3SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK;
    //FTM0_C5SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;
    FTM0_C7SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK;
    FTM0_SC   = FTM_SC_CLKS(1) | FTM_SC_PS(7);     /* Edge Aligned PWM running from BUSCLK / 128 */
}

void frdm_as_led_toggle_G() {
    RGB_LED(_R,_G>0 ? 0 : 50, _B);
}
void frdm_as_led_toggle_R() {
    RGB_LED(_R>0 ? 0 : 50, _G, _B);
}
void frdm_as_led_toggle_B() {
    RGB_LED(_R,_G,_B>0 ? 0 : 50);
}
