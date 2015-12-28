#include "MK64F12.h"
#include "as_led.h"
#include "led.h"

void delay(int time);

int main(void)
{
  uint32_t pattern = 0b10101101;
  frdm_as_led_init();
  frdm_led_init();

  for (;;) {
    //Blink led on board and application shield
    frdm_led_toggle_G();
    frdm_as_led_toggle_G(FRDM_AS_LED_INT_NORMAL);
    delay(2000);
    frdm_led_toggle_G();
    frdm_led_toggle_B();
    frdm_as_led_toggle_G(FRDM_AS_LED_INT_NORMAL);
    frdm_as_led_toggle_B(FRDM_AS_LED_INT_NORMAL);
    delay(2000);
    frdm_led_toggle_B();
    frdm_led_toggle_R();
    frdm_as_led_toggle_B(FRDM_AS_LED_INT_NORMAL);
    frdm_as_led_toggle_R(FRDM_AS_LED_INT_NORMAL);
    delay(2000);
    frdm_led_toggle_R();
    frdm_as_led_toggle_R(FRDM_AS_LED_INT_NORMAL);
  }
  return 0;
}

void delay(int time)
{
  int i = 0;
  for (i = time*1000 ; i !=0; i--) { }
}
