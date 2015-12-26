#include "MK64F12.h"
#include "led.h"

#define led_blue_init() { SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; PORTB_PCR21 = PORT_PCR_MUX(1); GPIOB_PDDR |= (1 << 21); }
#define led_blue_toogle() { GPIOB_PTOR |= (1 << 21); }

void delay(int time);

int main(void)
{
  uint32_t pattern = 0b10101101;
  frdm_as_led_init();
  led_blue_init();
  led_blue_toogle();

  for (;;) {
    //Blink led on board
    led_blue_toogle();
    // Blink red led on application shield
    frdm_as_led_toggle_R(FRDM_AS_LED_INT_NORMAL);
    delay(2000);
  }
  return 0;
}

void delay(int time)
{
  int i = 0;
  for (i = time*1000 ; i !=0; i--) { }
}
