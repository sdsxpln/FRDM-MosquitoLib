#include "MK64F12.h"
#include "system.h"
#include "i2c.h"
#include "lcd_C12832A1Z.h"

int main(int argc, char *argv[])
{
  struct i2c_mode i2c;
  char buffer[28];
  system_init();

  /* Enable additional clock sources */
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
  SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
  SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK;

  PORTE_PCR24 = PORT_PCR_MUX(5);
  PORTE_PCR25 = PORT_PCR_MUX(5);

  lcd_init();
  lcd_clear();

  i2c.freq = 7000;
  i2c.bus_clk = system_get_busclk();
  i2c.module = I2C0;
  i2c_init(&i2c);

  while(1) {
    uint8_t buff[2]={0,0};
    uint8_t reg = 0;
    uint16_t value;
    //Select the register
    i2c_write(&i2c, 0x48, reg, 0,0);
    i2c_read(&i2c, 0x48, buff, 2);
    value = buff[0] | buff[1]<<8;
    value>>=5;
    //Sign extend negative numbers
    if (value & (1 << 10))
        value |= 0xFC00;

    sprintf(buffer, " %d\n", value/8);
    lcd_locate(0, 0);
    lcd_print(buffer);
   system_delay_us(1000);
  }
  return 0;
}
