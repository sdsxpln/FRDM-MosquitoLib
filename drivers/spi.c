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

int frdm_spi_init(SPI_Type *spi, struct frdm_spi_mode *mode, uint32_t hz) {
    if(mode->frame < FRDM_SPI_MIN_FRAME_SIZE) {
      return -1;
    }
    // Enable clock
    SIM_SCGC6 |= SIM_SCGC6_SPI0(FRDM_ENABLE);
    // For debug purpose
    SPI_MCR_REG(spi) &= ~SPI_MCR_CONT_SCKE_MASK;
    SPI_MCR_REG(spi) |= (FRDM_ENABLE<<SPI_MCR_CLR_RXF_SHIFT);
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
    SPI_CTAR_REG(spi,0) |= 0<<SPI_CTAR_DBR_SHIFT;
    
    // Fifo
    SPI_MCR_REG(spi) &= ~SPI_MCR_DIS_TXF_MASK;
    SPI_MCR_REG(spi) |= 0<<SPI_MCR_DIS_TXF_SHIFT;

    //Enable
    SPI_MCR_REG(spi) &= ~SPI_MCR_MDIS_MASK;
    SPI_MCR_REG(spi) |= 0<<SPI_MCR_MDIS_SHIFT;

    // Start hardware
    SPI_MCR_REG(spi) &= ~SPI_MCR_HALT_MASK;
    SPI_MCR_REG(spi) |= 0<<SPI_MCR_HALT_SHIFT;
    return 0;
}

uint16_t frdm_spi_master_write(SPI_Type *spi, uint16_t value) {

    // Wait if FIFO is full
    while(!SPI_BRD_SR_TFFF(spi));
    /** Fill FIFO **/
    //clear flags 
    // Wait if receive FIFO is full
    while(!SPI_BRD_SR_RFDF(spi));
    //Clear flags

    // Return readed data
    return 0;
}


