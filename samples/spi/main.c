#include "MK64F12.h"
#include "spi.h"



int main(int argc, char *argv[]) {
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;    /*Enable Port B Clock Gate Control*/
  SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;    /*Enable Port E Clock Gate Control*/
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;    /*Enable Port C Clock Gate Control*/
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;    /*Enable Port A Clock Gate Control*/

  struct frdm_spi_mode mode;
  mode.mode = FRDM_SPI_MASTER;
  mode.CPOL = FRDM_SPI_CPOL_RISING_EDGE;
  mode.CPHA = FRDM_SPI_CPHA_FIRST_EDGE;
  mode.frame = 4;

  frdm_spi_init(SPI0,&mode,0);

  while(1) {
    // Do nothing
  }
  return 0;
}
