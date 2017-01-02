#include "MK64F12.h"
#include "i2c.h"

#define I2C_MASTER_WRITE 0
#define I2C_MASTER_READ  1
#define ICR              0
#define SCL              1
#define TOTAL_ICR       52

static const uint16_t icr_scl_table[TOTAL_ICR][2] = {
        { 0x00, 20 },
        { 0x01, 22 },
        { 0x02, 24 },
        { 0x03, 26 },
        { 0x04, 28 },
        { 0x05, 30 },
        { 0x09, 32 },
        { 0x06, 34 },
        { 0x0a, 36 },
        { 0x07, 40 },
        { 0x0c, 44 },
        { 0x0d, 48 },
        { 0x0e, 56 },
        { 0x12, 64 },
        { 0x0f, 68 },
        { 0x13, 72 },
        { 0x14, 80 },
        { 0x15, 88 },
        { 0x19, 96 },
        { 0x16, 104 },
        { 0x1a, 112 },
        { 0x17, 128 },
        { 0x1c, 144 },
        { 0x1d, 160 },
        { 0x1e, 192 },
        { 0x22, 224 },
        { 0x1f, 240 },
        { 0x23, 256 },
        { 0x24, 288 },
        { 0x25, 320 },
        { 0x26, 384 },
        { 0x2a, 448 },
        { 0x27, 480 },
        { 0x2b, 512 },
        { 0x2c, 576 },
        { 0x2d, 640 },
        { 0x2e, 768 },
        { 0x32, 896 },
        { 0x2f, 960 },
        { 0x33, 1024 },
        { 0x34, 1152 },
        { 0x35, 1280 },
        { 0x36, 1536 },
        { 0x3a, 1792 },
        { 0x37, 1920 },
        { 0x3b, 2048 },
        { 0x3c, 2304 },
        { 0x3d, 2560 },
        { 0x3e, 3072 },
        { 0x3f, 3840 }
};

static void i2c_set_frequency(struct i2c_mode *i2c)
{
  uint8_t mult,icr;
  uint8_t multiplier;
  uint8_t i,j;
  uint32_t tmp,prev;

  for(j = 0; j <= 2; j++) {
    multiplier = 1<<j;
    for(i = 0; i < TOTAL_ICR; i++) {
      tmp = i2c->bus_clk / (multiplier*icr_scl_table[i][SCL]);
      if(tmp <= i2c->freq) {
        if(i2c->freq-tmp <= i2c->freq-prev || !prev) {
          mult = j;
          icr = icr_scl_table[i][ICR];
          prev = tmp;
        }
      }
    }
  }
  I2C_F_REG(i2c->module) = mult<<6 | icr;
}

static void i2c_stop(I2C_Type *i2c)
{
  volatile int n;
  I2C_C1_REG(i2c) &= ~I2C_C1_MST_MASK;
  I2C_C1_REG(i2c) &= ~I2C_C1_TX_MASK;
  // It seems that there are timing problems
  // when there is no waiting time after a STOP.
  // This wait is also included on the samples
  // code provided with the freedom board
  for (n = 0; n < 200; n++) asm("nop");
}

static void i2c_start(I2C_Type *i2c)
{
  I2C_C1_REG(i2c) |= I2C_C1_MST(1);
}

static void i2c_wait_tx(I2C_Type *i2c)
{
  while((I2C_S_REG(i2c) & I2C_S_IICIF_MASK)==0);
  I2C_S_REG(i2c) &= ~I2C_S_IICIF_MASK;
  while((I2C_S_REG(i2c) & I2C_S_TCF_MASK)==0);
  I2C_S_REG(i2c) &= ~I2C_S_TCF_MASK;
}

static void i2c_wait_rx(I2C_Type *i2c)
{
  while((I2C_S_REG(i2c) & I2C_S_IICIF_MASK)==0);
  I2C_S_REG(i2c) &= ~I2C_S_IICIF_MASK;
}

void i2c_init(struct i2c_mode *i2c)
{
  i2c_set_frequency(i2c);
  I2C_C1_REG(i2c->module) |= I2C_C1_IICEN_MASK;
}

void i2c_write(struct i2c_mode *i2c, uint8_t address, uint8_t reg, uint8_t *data, uint8_t length)
{
  int i = 0;

  address<<=1;
  address |= I2C_MASTER_WRITE;
  // Generate start
  I2C_S_REG(i2c->module) &= ~I2C_S_TCF_MASK;
  i2c_start(i2c->module);
  I2C_C1_REG(i2c->module) |= I2C_C1_TX_MASK;

  I2C_D_REG(i2c->module) = address;
  i2c_wait_tx(i2c->module);
  I2C_D_REG(i2c->module) = reg;
  i2c_wait_tx(i2c->module);
  for(i=0; i<length; i++) {
    I2C_D_REG(i2c->module) = *data;
    data++;
    i2c_wait_tx(i2c->module);
  }
  // Generate stop
  i2c_stop(i2c->module);
}

void i2c_read(struct i2c_mode *i2c, uint8_t address, uint8_t *data, uint8_t length)
{
  int i;
  uint8_t dummy;

  address<<=1;
  address |= I2C_MASTER_READ;
  i2c_start(i2c->module);
  I2C_C1_REG(i2c->module) |= I2C_C1_TX_MASK;
  I2C_D_REG(i2c->module) = address;
  i2c_wait_tx(i2c->module);

  // reveive mode
  I2C_C1_REG(i2c->module) &= (~I2C_C1_TX_MASK);

  // reading this register initiates receiving of the next byte
  for(i=0; i<length; i++) {
    if(i == length-1) {
      I2C_C1_REG(i2c->module) |= I2C_C1_TXAK_MASK;
    }
    if(i) {
      data[i-1] = I2C_D_REG(i2c->module);
    } else {
      dummy = I2C_D_REG(i2c->module);
    }
    i2c_wait_rx(i2c->module);
  }
  // Generate stop
  i2c_stop(i2c->module);

  data[i-1] = I2C_D_REG(i2c->module);
  I2C_C1_REG(i2c->module) &= ~I2C_C1_TXAK_MASK;
}
