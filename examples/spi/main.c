#include "MK64F12.h"
#include "spi.h"


#define led_blue_init() { PORTB_PCR21 = PORT_PCR_MUX(1); GPIOB_PDDR |= (1 << 21); }
#define led_blue_toogle() { GPIOB_PTOR |= (1 << 21); }



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
	led_blue_init();
	led_blue_toogle();
  
  struct frdm_spi_mode mode;
  mode.mode = FRDM_SPI_MASTER;
  mode.CPOL = FRDM_SPI_CPOL_RISING_EDGE;
  mode.CPHA = FRDM_SPI_CPHA_FIRST_EDGE;
  mode.frame = 8;

  frdm_spi_init(SPI0,&mode,1000000);

  while(1) {
	 led_blue_toogle();
   delay(2000);
   frdm_spi_master_write(SPI0, 0xAA);
  }
  return 0;
}

void delay(int time)
{
	int i = 0;
	for (i = time*1000 ; i !=0; i--) { }
}
