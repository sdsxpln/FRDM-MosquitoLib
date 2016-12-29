#include "MK64F12.h"
#include "spi.h"
#include "font.h"
#include "lcd_C12832A1Z.h"
#include "system.h"
#include <string.h>

#define LCD_DISPLAY_STATE     0xAE
#define LCD_DISPLAY_OFF       (LCD_DISPLAY_STATE | 0)
#define LCD_DISPLAY_ON        (LCD_DISPLAY_STATE | 1)
#define LCD_LINE_ZERO         0x40
#define LCD_LINE(x)           (LCD_LINE_ZERO | x)
#define LCD_ROW_MSB(x)        (0x10 | (x & 0x07))
#define LCD_ROW_LSB(x)        (0x00 | (x & 0x07))
#define LCD_ACD_NORMAL        0xA0
#define LCD_ACD_REVERSE       (LCD_ACD_NORMAL | 1)
#define LCD_DISPLAY_NORMAL    0xA6
#define LCD_DISPLAY_REVERSE   (LCD_DISPLAY_NORMAL | 1)
#define LCD_ALL_POINTS        0xA4
#define LCD_ALL_POINTS_OFF    (LCD_ALL_POINTS | 0)
#define LCD_ALL_POINTS_ON     (LCD_ALL_POINTS | 1)
#define LCD_BIAS_9            0xA2
#define LCD_BIAS_7            (LCD_BIAS_9 | 1)
#define LCD_MODIFY            0xE0
#define LCD_END               0xEE
#define LCD_RESET             0xE2
#define LCD_NOP               0xE3
#define LCD_POWER_ON          0x2F
#define LCD_VOLTAGE_RATIO     0x22
#define LCD_OUTPUT_NORMAL     0xC8
#define LCD_PAGE(x)           (0xB0 | x)
#define LCD_BUFFER_SIZE       512
#define LCD_TOTAL_PAGES       4
#define LCD_WIDTH             128
#define LCD_HEIGHT            32
#define LCD_CMD_CONTRAST      0x81
#define LCD_PIX_2_INDEX(x,y)  x+((y/8)*LCD_BUFFER_SIZE/LCD_TOTAL_PAGES)

static uint8_t buffer[LCD_BUFFER_SIZE];
static uint16_t char_x = 0;
static uint16_t char_y = 0;


static void lcd_cmd(uint8_t cmd)
{
  GPIOC_PCOR |= (1<<3); //A0  = 0
  GPIOD_PCOR |= (1<<0); //CS  = 0
  frdm_spi_master_write_byte(SPI0, cmd);
  GPIOD_PSOR |= (1<<0); //CS  = 1
}

static void ldc_data(uint8_t data)
{
  GPIOC_PSOR |= (1<<3); //A0  = 1
  GPIOD_PCOR |= (1<<0); //CS  = 0
  frdm_spi_master_write_byte(SPI0, data);
  GPIOD_PSOR |= (1<<0); //CS  = 1
}

static void set_pixel(uint16_t x, uint16_t y, int erase)
{
  if(erase)
    buffer[LCD_PIX_2_INDEX(x,y)] &= ~(1 << (y%8));
  else
    buffer[LCD_PIX_2_INDEX(x,y)] |= (1 << (y%8));
}

static void _putc(uint16_t x, uint16_t y, uint8_t ch)
{
    unsigned int hor,vert,offset,bpl,j,i,b;
    const unsigned char* bitmap;
    unsigned char pixel,width;

    // test char e
    if ((ch < FONT_MIN_CHARACTER) || (ch > FONT_MAX_CHARACTER)) return;

    // read font parameter from start of array
    offset = font[0];                    // bytes / char
    hor = font[1];                       // get hor size of font
    vert = font[2];                      // get vert size of font
    bpl = font[3];                       // bytes per line

    // Verify current character coordinates
    if (char_x + hor > LCD_WIDTH) {
        char_x = 0;
        char_y = char_y + vert;
        if (char_y >= LCD_HEIGHT - vert) {
            char_y = 0;
        }
    }

    // Start of the bitmap
    bitmap = &font[((ch-FONT_MIN_CHARACTER) * offset) + FONT_HEADER_SIZE];
    width = bitmap[0];
    // construct the char into the buffer
    for (j=0; j<vert; j++) {  //  vert line
        for (i=0; i<hor; i++) {   //  horz line
            pixel =  bitmap[bpl * i + 1];
            b = 1 << j%8;
            set_pixel(x+i,y+j, !(pixel&b));
        }
    }
    char_x += width;
}

void lcd_set_contrast(uint8_t contrast)
{
  lcd_cmd(LCD_CMD_CONTRAST);
  lcd_cmd(0x3F & contrast);
}

void lcd_print(char *text)
{
  while(*text != '\0') {
    lcd_putc(*text);
    text++;
  }
  lcd_update();
}

void lcd_update(void)
{
  uint16_t i=0,j=0;
  uint16_t page_size = LCD_BUFFER_SIZE/LCD_TOTAL_PAGES;
  for(j = 0; j < LCD_BUFFER_SIZE; j+=page_size){
    lcd_cmd(LCD_ROW_LSB(0));
    lcd_cmd(LCD_ROW_MSB(0));
    lcd_cmd(LCD_PAGE(j/page_size));
    for(i=j; i<j+page_size; i++) {
      ldc_data(buffer[i]);
    }
  }
}

void lcd_putc(int value)
{
  if (value == '\n') {    // new line
    char_x = 0;
    char_y = char_y + font[2];
    if (char_y > LCD_HEIGHT - font[2]) {
        char_y = 0;
    }
  } else {
    _putc(char_x, char_y, value);
    lcd_update();
  }
}

void lcd_locate(int x, int y)
{
  char_x = x;
  char_y = y;
}

void lcd_clear()
{
  memset(buffer, 0, LCD_BUFFER_SIZE);
  lcd_update();
}

void lcd_reset()
{
  //Reset
  GPIOD_PCOR |= (1<<3);
  GPIOD_PSOR |= (1<<3);

  lcd_cmd(LCD_DISPLAY_OFF);
  lcd_cmd(LCD_BIAS_9);

  lcd_cmd(LCD_ACD_NORMAL);
  lcd_cmd(LCD_OUTPUT_NORMAL);

  lcd_cmd(LCD_VOLTAGE_RATIO);
  lcd_cmd(LCD_POWER_ON);
  lcd_cmd(LCD_LINE_ZERO);
  lcd_cmd(LCD_DISPLAY_ON);

  lcd_set_contrast(0x17);

  lcd_cmd(LCD_DISPLAY_NORMAL);
}

void lcd_init()
{
  struct frdm_spi_mode mode;

  PORTD_PCR(1) = PORT_PCR_MUX(2); //SCK
  PORTD_PCR(2) = PORT_PCR_MUX(2); //SOUT

  PORTC_PCR3 = PORT_PCR_MUX(1);
  PORTD_PCR0 = PORT_PCR_MUX(1);
  GPIOC_PDDR |= (1 << 3);
  GPIOD_PDDR |= (1 << 0);

  mode.mode = FRDM_SPI_MASTER;
  mode.CPOL = FRDM_SPI_CPOL_FALLING_EDGE;
  mode.CPHA = FRDM_SPI_CPHA_SECOND_EDGE;
  mode.frame = 8;
  mode.bus_clk = system_get_busclk();
  mode.baud = 20000000;
  frdm_spi_init(SPI0, &mode);

  // Overwrite spi port , MISO is used as reset line
  PORTD_PCR3 = PORT_PCR_MUX(1);
  GPIOD_PDDR |= (1 << 3);

  GPIOC_PSOR |= (1<<3); //A0  = 0
  GPIOD_PSOR |= (1<<0); //CS  = 1

  lcd_reset();
  lcd_clear();
  lcd_print("LCD INITIALISED!\n");
}

