#ifndef _LCD_C12832A1Z_H_
#define _LCD_C12832A1Z_H_

void lcd_set_contrast(uint8_t contrast);
void lcd_print(char *text);
void lcd_update(void);
void lcd_putc(int value);
void lcd_locate(int x, int y);
void lcd_reset();
void lcd_init();
void lcd_clear();

#endif
