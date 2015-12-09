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
#ifndef _SPI_H_
#define _SPI_H_

#ifdef MOSQUITO_CONFIG_SPI_ENABLED

#define FRDM_SPI_CPOL_RISING_EDGE   0
#define FRDM_SPI_CPOL_FALLING_EDGE  1
#define FRDM_SPI_CPHA_FIRST_EDGE    0
#define FRDM_SPI_CPHA_SECOND_EDGE   1

// TODO move to other header file
#define FRDM_ENABLE                 1
#define FRDM_DISABLE                0

typedef enum
{
    FRDM_SPI0,
    FRDM_SPI1,
    FRDM_SPI2
} frdm_spi_t;

typedef enum
{
    FRDM_SPI_SLAVE
    FRDM_SPI_MASTER,
} frdm_spi_mode_t;

struct frdm_spi_mode {
    uint8_t CPOL;
    uint8_t CPHA;
};

#endif
#endif

