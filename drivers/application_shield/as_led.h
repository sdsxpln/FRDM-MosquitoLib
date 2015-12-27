#ifndef _AS_LED_H_
#define _AS_LED_H_

#define FRDM_AS_LED_INT_NORMAL  50
#define FRDM_AS_LED_INT_HIGH   100

extern void frdm_as_led_init();

extern void frdm_as_led_toggle_R(uint8_t intensity);
extern void frdm_as_led_toggle_G(uint8_t intensity);
extern void frdm_as_led_toggle_B(uint8_t intensity);

#endif
