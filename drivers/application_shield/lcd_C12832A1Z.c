#include "MK64F12.h"
#include "spi.h"

#define LCD_DISPLAY_STATE   0xAE
#define LCD_DISPLAY_OFF     (LCD_DISPLAY_STATE | 0)
#define LCD_DISPLAY_ON      (LCD_DISPLAY_STATE | 1)
#define LCD_LINE_ZERO       0x40
#define LCD_LINE(x)         (LCD_LINE_ZERO | x)
#define LCD_ROW_MSB(x)      (0x10 | (x & 0x07))
#define LCD_ROW_LSB(x)      (0x00 | (x & 0x07))
#define LCD_ACD_NORMAL      0xA0
#define LCD_ACD_REVERSE     (LCD_ACD_NORMAL | 1)
#define LCD_DISPLAY_NORMAL  0xA6
#define LCD_DISPLAY_REVERSE (LCD_DISPLAY_NORMAL | 1)
#define LCD_ALL_POINTS      0xA4
#define LCD_ALL_POINTS_OFF  (LCD_ALL_POINTS | 0)
#define LCD_ALL_POINTS_ON   (LCD_ALL_POINTS | 1)
#define LCD_BIAS_9          0xA2
#define LCD_BIAS_7          (LCD_BIAS_9 | 1)
#define LCD_MODIFY          0xE0
#define LCD_END             0xEE
#define LCD_RESET           0xE2
#define LCD_NOP             0xE3
#define LCD_POWER_ON        0x2F
#define LCD_VOLTAGE_RATIO   0x22
#define LCD_OUTPUT_NORMAL   0xC8
#define LCD_SET_CONTRAST(x) frdm_as_lcd_out(0x81); frdm_as_lcd_out(x);
#define LCD_PAGE(x)         (0xB0 | x)
#define LCD_BUFFER_SIZE     512
#define LCD_TOTAL_PAGES     4


static uint8_t buffer[LCD_BUFFER_SIZE];

void frdm_as_lcd_out(uint8_t cmd)
{
  GPIOC_PCOR |= (1<<3); //A0  = 0
  GPIOD_PCOR |= (1<<0); //CS  = 0
  frdm_spi_master_write_byte(SPI0, cmd);
  GPIOD_PSOR |= (1<<0); //CS  = 1
}
void frdm_as_lcd_out_data(uint8_t data) {
  GPIOC_PSOR |= (1<<3); //A0  = 1
  GPIOD_PCOR |= (1<<0); //CS  = 0
  frdm_spi_master_write_byte(SPI0, data);
  GPIOD_PSOR |= (1<<0); //CS  = 1
}
/** TODO move to other module **/
void wait_ms(uint32_t ms) {
  volatile uint32_t i;
  for(i=0; i<120000*ms; i++) {
    // do nothing
  }
}
static void frdm_as_lcd_update(void) {
  uint16_t i=0,j=0;
  uint16_t page_size = LCD_BUFFER_SIZE/LCD_TOTAL_PAGES;
  for(j = page_size; j < LCD_BUFFER_SIZE; j++) {
    frdm_as_lcd_out(LCD_ROW_LSB(0));
    frdm_as_lcd_out(LCD_ROW_MSB(0));
    frdm_as_lcd_out(LCD_PAGE(j/page_size-1));
    for(i=j-page_size; i<j; i++) {
      frdm_as_lcd_out_data(buffer[i]);
    }
  }
}
void frdm_as_lcd_init()
{
  PORTC_PCR3 = PORT_PCR_MUX(1);
  PORTD_PCR0 = PORT_PCR_MUX(1);
  GPIOC_PDDR |= (1 << 3);
  GPIOD_PDDR |= (1 << 0);

  memset(buffer, 0, LCD_BUFFER_SIZE);
  struct frdm_spi_mode mode;
  mode.mode = FRDM_SPI_MASTER;
  mode.CPOL = FRDM_SPI_CPOL_FALLING_EDGE;
  mode.CPHA = FRDM_SPI_CPHA_SECOND_EDGE;
  mode.frame = 8;
  frdm_spi_init(SPI0, &mode, 20000000);

  // Overwrite spi port , MISO is used as reset line
  PORTD_PCR3 = PORT_PCR_MUX(1);
  GPIOD_PDDR |= (1 << 3);

  GPIOC_PCOR |= (1<<3); //A0  = 0
  GPIOD_PSOR |= (1<<0); //CS  = 1
  GPIOD_PCOR |= (1<<3); // /R   = 0
  wait_ms(2);
  GPIOD_PSOR |= (1<<3); // /R   = 1
  frdm_as_lcd_out(LCD_DISPLAY_OFF);
  frdm_as_lcd_out(LCD_BIAS_9);

  frdm_as_lcd_out(LCD_ACD_NORMAL);
  frdm_as_lcd_out(LCD_OUTPUT_NORMAL);

  frdm_as_lcd_out(LCD_VOLTAGE_RATIO);
  frdm_as_lcd_out(LCD_POWER_ON);
  frdm_as_lcd_out(LCD_LINE_ZERO);
  frdm_as_lcd_out(LCD_DISPLAY_ON);

  LCD_SET_CONTRAST(0x17);

  frdm_as_lcd_out(LCD_DISPLAY_NORMAL);

  frdm_as_lcd_update();
}

