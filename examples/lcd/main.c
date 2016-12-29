#include <stdio.h>
#include "MK64F12.h"
#include "system.h"
#include "lcd_C12832A1Z.h"

int main(int argc, char *argv[])
{
  uint32_t i = 0;
  char buffer[28];
  system_init();

  /* Enable additional clock sources */
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
  SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK;

  lcd_init();
  lcd_clear();

  while(1) {
    sprintf(buffer, "Hello world! %d\n", i++);
    lcd_locate(0, 0);
    lcd_print(buffer);
    system_delay_us(1000000);
  }
  return 0;
}
