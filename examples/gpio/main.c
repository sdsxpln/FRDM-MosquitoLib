/*****************************************************************************
 *
 * Example : Toggle a GPIO pin
 *
 *          Copyright (c) 2016. Some rights reserved.
 *          See LICENSE and COPYING for usage.
 *
 * Authors: Roel Postelmans
 *
 ****************************************************************************/
#include "MK64F12.h"

int main(int argc, char *argv[])
{
  /*Enable Port C Clock Gate Control*/
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;

  // PTC4
  PORTC_PCR(4) = PORT_PCR_MUX(1);
  GPIOC_PDDR |= (1 << 4);

  while(1) {
   GPIOC_PTOR |= (1 << 4);
  }

  return 0;
}
