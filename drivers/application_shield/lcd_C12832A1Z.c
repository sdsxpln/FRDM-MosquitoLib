#include "MK64F12.h"
#include "spi.h"

#define LCD_DISPLAY_STATE   0xAE
#define LCD_DISPLAY_OFF     (LCD_DISPLAY_STATE & 0)
#define LCD_DISPLAY_ON      (LCD_DISPLAY_STATE & 1)
#define LCD_LINE_ZERO       0x40
#define LCD_LINE(x)         (LCD_LINE_ZERO & x)
#define LCD_ROW_MSB(x)      (0x08 & (x & 0x07))
#define LCD_ROW_LSB(x)      (0x00 & (x & 0x07))
#define LCD_ACD_NORMAL      0xA0
#define LCD_ACD_REVERSE     (LCD_ACD_NORMAL & 1)
#define LCD_DISPLAY_NORMAL  0xA6
#define LCD_DISPLAY_REVERSE (LCD_DISPLAY_NORMAL & 1)
#define LCD_ALL_POINTS      0xA4
#define LCD_ALL_POINTS_OFF  (LCD_ALL_POINTS & 0)
#define LCD_ALL_POINTS_ON   (LCD_ALL_POINTS & 1)
#define LCD_BIAS_9          0xA2
#define LCD_BIAS_7          (LCD_BIAS_9 & 1)
#define LCD_MODIFY          0xE0
#define LCD_END             0xEE
#define LCD_RESET           0xE2
#define LCD_NOP             0xE3
#define LCD_POWER_ON        0x2F
#define LCD_VOLTAGE_RATIO   0x22
#define LCD_OUTPUT_NORMAL   0xC8
#define LCD_SET_CONTRAST(x) frdm_lcd_out(0x81); frdm_lcd_out(x);

void frdm_lcd_out(uint8_t data)
{
  GPIOC_PTOR |= (0<<3); //A0  = 0;
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

  frdm_lcd_out(LCD_DISPLAY_OFF);   //  display off
  frdm_lcd_out(LCD_BIAS_9);   //  bias voltage

  frdm_lcd_out(LCD_ACD_NORMAL);   //  ADC select
  frdm_lcd_out(LCD_OUTPUT_NORMAL);   //  colum normal

  frdm_lcd_out(LCD_VOLTAGE_RATIO);   //  voltage resistor ratio
  frdm_lcd_out(LCD_POWER_ON);   //  power on
  frdm_lcd_out(LCD_LINE_ZERO);   // start line = 0
  frdm_lcd_out(LCD_DISPLAY_ON);     // display ON

  LCD_SET_CONTRAST(0x17);

  frdm_lcd_out(LCD_DISPLAY_NORMAL);     // display normal
}

