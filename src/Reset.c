/*
 * Reset.c
 *
 *  Created on: Apr 29, 2025
 *      Author: RD17
 */

#include "stm32f10x.h" // or the correct header for your setup

void software_reset(void)
{
    NVIC_SystemReset();
}
