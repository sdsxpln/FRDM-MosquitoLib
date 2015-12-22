#include "MK64F12.h"
#include "spi.h"


void frdm_lcd_out(uint8_t data)
{
  GPIOC_PTOR &= (0<<3); //A0  = 0;
  frdm_spi_master_write_byte(SPI0, data);
}

void init_LCD()
{
  PORTC_PCR3 = PORT_PCR_MUX(1);
  GPIOC_PDDR |= (1 << 3);
  struct frdm_spi_mode mode;
  mode.mode = FRDM_SPI_MASTER;
  mode.CPOL = FRDM_SPI_CPOL_FALLING_EDGE;
  mode.CPHA = FRDM_SPI_CPHA_SECOND_EDGE;
  mode.frame = 8;
  frdm_spi_init(SPI0, &mode, 20000000);

  frdm_lcd_out(0xAE);   //  display off
  frdm_lcd_out(0xA2);   //  bias voltage

  frdm_lcd_out(0xA0);   //  ADC select
  frdm_lcd_out(0xC8);   //  colum normal

  frdm_lcd_out(0x22);   //  voltage resistor ratio
  frdm_lcd_out(0x2F);   //  power on
  //frdm_lcd_out(0xA4);   //  LCD display ram
  frdm_lcd_out(0x40);   // start line = 0
  frdm_lcd_out(0xAF);     // display ON

  frdm_lcd_out(0x81);   //  set contrast
  frdm_lcd_out(0x17);   //  set contrast

  frdm_lcd_out(0xA6);     // display normal
}

