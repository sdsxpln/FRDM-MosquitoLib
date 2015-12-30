#include "MK64F12.h"
#include "spi.h"
#include "led.h"




int main(int argc, char *argv[]) {
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;    /*Enable Port B Clock Gate Control*/
  SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;    /*Enable Port E Clock Gate Control*/
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;    /*Enable Port C Clock Gate Control*/
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;    /*Enable Port A Clock Gate Control*/
  SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;    /*Enable Port D Clock Gate Control*/
  SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK;

  //PORT D
  PORTD_PCR(0) = PORT_PCR_MUX(2); //PCS0
  PORTD_PCR(1) = PORT_PCR_MUX(2); //SCK
  PORTD_PCR(2) = PORT_PCR_MUX(2); //SOUT
  PORTD_PCR(3) = PORT_PCR_MUX(2); //SIN

  frdm_as_led_init();
  frdm_as_lcd_init();

  while(1) {
    delay(2000);
    frdm_as_led_toggle_R();
  }
  return 0;
}

void delay(int time)
{
	int i = 0;
	for (i = time*1000 ; i !=0; i--) { }
}
