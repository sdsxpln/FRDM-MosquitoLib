/*
 * FRDM-MosquitoLibrary 
 * Copyright (C) <2015>  Roel Postelmans <postelmansroel@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "MK64F12.h"
#include "spi.h"

#define FRDM_SPI_MIN_FRAME_SIZE 4
#define SPI_PUSHR_PCS0_ON 0x10000
#define SPI_PUSHR_PCS1_ON 0x20000
#define SPI_PUSHR_PCS2_ON 0x40000
#define SPI_PUSHR_PCS3_ON 0x80000
#define SPI_PUSHR_PCS4_ON 0x100000
#define SPI_PUSHR_PCS5_ON 0x200000
#define SPI_CTAR_FMSZ_8BIT 0x38000000
#define SPI_CTAR_FMSZ_16BIT 0x78000000



static const uint32_t scaler[] = {2,4,6,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
static const uint32_t prescaler[] = { 2,3,5,7};
static void setBaudRate(SPI_Type* spi,uint32_t hz);

void frdm_spi_IRQHandler(SPI_Type *spi) {
}
void SPI0_IRQHandler(void) {
   frdm_spi_IRQHandler(SPI0);
}

void SPI1_IRQHandler(void) {
   frdm_spi_IRQHandler(SPI1);
}
void SPI2_IRQHandler(void) {
   frdm_spi_IRQHandler(SPI2);
}
int frdm_spi_init(SPI_Type *spi, struct frdm_spi_mode *mode, uint32_t hz) {
    unsigned char cmd = 0x01;
    if(mode->frame < FRDM_SPI_MIN_FRAME_SIZE) {
      return -1;
    }


    //clear the status bits (write-1-to-clear
    SPI0_SR = (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK); 
    // Clear all registers
    SPI0_TCR = 0;
    SPI0_RSER = 0;
    SPI0_PUSHR = 0; 
    SPI0_CTAR0 = 0;

    // For debug purpose
    SPI_MCR_REG(spi) &= ~SPI_MCR_CONT_SCKE_MASK;
    SPI_MCR_REG(spi) |= (FRDM_ENABLE<<SPI_MCR_CONT_SCKE_SHIFT);
    // Master/slave
    SPI_MCR_REG(spi) &= ~SPI_MCR_MSTR_MASK;
    SPI_MCR_REG(spi) |= (mode->mode<<SPI_MCR_MSTR_SHIFT);
    if(mode->mode == FRDM_SPI_MASTER) {
      //Frame size
      SPI_CTAR_REG(spi,0) &= ~SPI_CTAR_FMSZ_MASK;
      SPI_CTAR_REG(spi,0) |= (((mode->frame-1) & 0x0F)<<SPI_CTAR_FMSZ_SHIFT);
      // Spi mode
      SPI_CTAR_REG(spi,0) &= ~SPI_CTAR_CPOL_MASK;
      SPI_CTAR_REG(spi,0) |= (mode->CPOL)<<SPI_CTAR_CPOL_SHIFT;
      SPI_CTAR_REG(spi,0) &= ~SPI_CTAR_CPHA_MASK;
      SPI_CTAR_REG(spi,0) |= (mode->CPHA)<<SPI_CTAR_CPHA_SHIFT;
      //Always in msb mode
      SPI_CTAR_REG(spi,0) &= ~SPI_CTAR_LSBFE_MASK;

    } else if(mode->mode == FRDM_SPI_SLAVE) {
      /*TODO*/
    }
    //frequency
    SPI_CTAR_REG(spi,0) &= ~SPI_CTAR_ASC_MASK;
    SPI_CTAR_REG(spi,0) |= 1<<SPI_CTAR_ASC_SHIFT;
    SPI_CTAR_REG(spi,0) &= ~SPI_CTAR_PASC_MASK;
    SPI_CTAR_REG(spi,0) |= 0<<SPI_CTAR_PASC_SHIFT;

    SPI_CTAR_REG(spi,0) &= ~SPI_CTAR_DBR_MASK;
    SPI_CTAR_REG(spi,0) &= ~SPI_CTAR_BR_MASK;
    SPI_CTAR_REG(spi,0) &= ~SPI_CTAR_PBR_MASK;
    setBaudRate(spi,hz);
    // Fifo
    SPI_MCR_REG(spi) &= ~SPI_MCR_DIS_TXF_MASK;

    //Enable
    SPI_MCR_REG(spi) &= ~SPI_MCR_MDIS_MASK;

    // Start hardware
    SPI_MCR_REG(spi) &= ~SPI_MCR_HALT_MASK;




    return 0;
}
static void setBaudRate(SPI_Type* spi,uint32_t hz) {
  uint32_t br;
  uint32_t pbr;
  uint32_t i,j;
  uint32_t tmp;
  uint32_t prev = 0;
  uint32_t source = 120000000;
  for(i = 0; i < 16; i++) {
    for(j = 0; pbr < 4; pbr++) {
      tmp = source/(scaler[i] * prescaler[j]);
      // Dont exceed desired frequency
      if(tmp >= hz) {
        if(tmp-hz <= prev-hz || !prev) {
          br = scaler[i];
          pbr = prescaler[i];
          prev = tmp;
        }
      }
    }
  }
  SPI_CTAR_REG(spi,0) |= br<<SPI_CTAR_BR_SHIFT;
  SPI_CTAR_REG(spi,0) |= pbr<<SPI_CTAR_PBR_SHIFT;
}
uint16_t frdm_spi_master_write(SPI_Type *spi, uint8_t value) {

    SPI_MCR_REG(spi) |=  SPI_MCR_HALT_MASK;
    SPI_MCR_REG(spi) |= (SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK); //flush the fifos
    SPI_SR_REG(spi)  |= (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK); //clear the status bits (write-1-to-clear)

    SPI_TCR_REG(spi) |= SPI_TCR_SPI_TCNT_MASK;
    SPI_MCR_REG(spi) &=  ~SPI_MCR_HALT_MASK;

    // One word transfer
    SPI_PUSHR_REG(spi) = (~SPI_PUSHR_CONT_MASK | SPI_PUSHR_EOQ_MASK | SPI_PUSHR_PCS0_ON | value);
    while(!(SPI_SR_REG(spi) & SPI_SR_TCF_MASK));

    SPI_SR_REG(spi) |= SPI_SR_TFFF_MASK; //clear the status bits (write-1-to-clear)

    return 0;
}


