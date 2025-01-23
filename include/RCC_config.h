/**************************************************************************************************/
/* Author  : Kareem Magdy                                                                         */
/* Version : V01                                                                                  */
/* Date    : 1 Jan 2023                                                                           */
/**************************************************************************************************/

#ifndef _RCC_CONFIG_H
#define _RCC_CONFIG_H


/* HSI - HSE - PLL */
#define SYSTEM_CLOCK           PLL

/* Crystal - RC */
#define HSE_TYPE               CRYSTAL


/* PLL_HSI_DIVIDED_BY_2 - PLL_HSE - PLL_HSE_DIVIDED_BY_2 */
#define PLL_CLOCK_SOURCE         PLL_HSE


/* PLL_CLOCK_MULTIPLE_BY_4 ( 4-9 & 6.5 ) */ 
#define PLL_MULTIPLE_VALUE       PLL_CLOCK_MULTIPLE_BY_9





#endif
