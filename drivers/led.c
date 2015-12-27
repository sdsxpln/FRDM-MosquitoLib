/*******************************************************
 * Led module
 * The led is routed as followed:
 * RED       PTB22
 * GREE      PTE26
 * BLUE      PTB21
 *
 * The hardware doesn't has any PWM routed to these ports,
 * so unfortunaly, no intensity control is possible
 **********************************************************/
#include "MK64F12.h"
#include "led.h"

void frdm_led_init() {
    // Power on
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTB_MASK;

    PORTB_PCR22 = PORT_PCR_MUX(1);
    PORTE_PCR26 = PORT_PCR_MUX(1);
    PORTB_PCR21 = PORT_PCR_MUX(3);

    // Set port as output
    GPIOB_PDDR |= (1 << 22);
    GPIOE_PDDR |= (1 << 26);
    GPIOB_PDDR |= (1 << 21);

    GPIOB_PSOR |= (1 << 22);
    GPIOE_PSOR |= (1 << 26);
    GPIOB_PSOR |= (1 << 21);
}

void frdm_led_toggle_G() {
    GPIOE_PTOR |= (1 << 26);
}
void frdm_led_toggle_R() {
    GPIOB_PTOR |= (1 << 22);
}
void frdm_led_toggle_B() {
    GPIOB_PTOR |= (1 << 21);
}
