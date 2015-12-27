/*******************************************************
 * Led module (application shield)
 * The led is routed as followed:
 * R       PTA2
 * G       PTC4
 * B       PTA0
 *
 * FlexTimers with PWM functionallity are used to controll
 * the intensity of the colors
 *
 * @ known issues: PTA0 (blue) is also routed for some debugger
 *                 functionallity on board revisions other than E.
 *                 Multiplexing this port as PTA0 while debugging,
 *                 will cause the CPU to hard fault.
 *
 *                 In order to prevent some hard to find bugs
 *                 , the blue led is currently
 *                 unsupported
 **********************************************************/
#include "MK64F12.h"
#include "as_led.h"

static uint8_t _R = 0;
static uint8_t _G = 0;
static uint8_t _B = 0;

static inline void RGB_LED(uint8_t red, uint8_t green, uint8_t blue) {
    FTM0_C7V = red;
    FTM0_C3V = green;
    //FTM0_C5V = blue;
    _R = red;
    _G = green;
    _B = blue;
}

void frdm_as_led_init() {
    // Power on
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTA_MASK;
    SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
    SIM_SOPT4 |= SIM_SOPT4_FTM0TRG0SRC(1);

    PORTA_PCR2 = PORT_PCR_MUX(3);  // FTM0_CH7
    PORTC_PCR4 = PORT_PCR_MUX(4);  // FTM0_CH3
    //PORTA_PCR0 = PORT_PCR_MUX(3);  // FTM0_CH5

    RGB_LED(0,0,0);
    // Make write_protected registers writable
    FTM0_MODE |= FTM_MODE_WPDIS_MASK;
    FTM0_MODE &= ~ FTM_MODE_FTMEN_MASK;
    // Overflow at 100
    FTM0_MOD  = 100;
    FTM0_CNTIN = 0;
    FTM0_CNT = 0;
    FTM0_C3SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK;
    FTM0_C5SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;
    FTM0_C7SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK;
    FTM0_SC   = FTM_SC_CLKS(1) | FTM_SC_PS(7);     /* Edge Aligned PWM running from BUSCLK / 128 */
}

void frdm_as_led_toggle_G(uint8_t intensity) {
    RGB_LED(_R,_G>0 ? 0 : intensity, _B);
}
void frdm_as_led_toggle_R(uint8_t intensity) {
    RGB_LED(_R>0 ? 0 : intensity, _G, _B);
}
void frdm_as_led_toggle_B(uint8_t intensity) {
    RGB_LED(_R,_G,_B>0 ? 0 : intensity);
}
