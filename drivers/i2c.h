#ifndef _I2C_H_
#define _I2C_H_

struct i2c_mode {
  I2C_Type *module;
  uint32_t bus_clk;
  uint32_t freq;
};

void i2c_init(struct i2c_mode *i2c);
void i2c_write(struct i2c_mode *i2c, uint8_t address, uint8_t reg, uint8_t *data, uint8_t length);
void i2c_read(struct i2c_mode *i2c, uint8_t address, uint8_t *data, uint8_t length);

#endif
