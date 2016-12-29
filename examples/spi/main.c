#include "MK64F12.h"
#include "spi.h"
#include "system.h"

#define led_blue_init() { PORTB_PCR21 = PORT_PCR_MUX(1); GPIOB_PDDR |= (1 << 21); }
#define led_blue_togle() { GPIOB_PTOR |= (1 << 21); }

int main(int argc, char *argv[])
{
  uint8_t value[2] = {0xAA, 0x0F};

  system_init();

  /* Enable additional clock sources */
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
  SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK;

  //PORT D
  PORTD_PCR(0) = PORT_PCR_MUX(2); //PCS0
  PORTD_PCR(1) = PORT_PCR_MUX(2); //SCK
  PORTD_PCR(2) = PORT_PCR_MUX(2); //SOUT
  PORTD_PCR(3) = PORT_PCR_MUX(2); //SIN
  led_blue_init();

  struct frdm_spi_mode mode;
  mode.mode = FRDM_SPI_MASTER;
  mode.CPOL = FRDM_SPI_CPOL_RISING_EDGE;
  mode.CPHA = FRDM_SPI_CPHA_FIRST_EDGE;
  mode.frame = 8;
  mode.bus_clk = system_get_busclk();
  mode.baud = 1000000;

  frdm_spi_init(SPI0, &mode);

  while(1) {
   led_blue_togle();
   frdm_spi_master_write(SPI0, value, 2);
   system_delay_us(10);
  }
  return 0;
}
