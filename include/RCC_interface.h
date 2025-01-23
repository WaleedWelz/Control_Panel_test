/***************************************************************************************************/
/* Author  : Kareem Magdy                                                                          */
/* Version : V01                                                                                   */
/* Date    : 1 Jan 2023                                                                            */
/* Company : AIO                                                                           */
/**************************************************************************************************/

#ifndef _RCC_INTERFACE_H
#define _RCC_INTERFACE_H

#define  AFIO_BUS                 0
#define  GPIO_PORTA_BUS           2
#define  GPIO_PORTB_BUS           3
#define  GPIO_PORTC_BUS           4
#define  UART_PIN                 14
#define  I2C_PIN

#define  RCC_AHB                  0
#define  RCC_APB1                 1
#define  RCC_APB2                 2

#define  HSI                      0
#define  HSE                      1
#define  PLL                      2

#define CRYSTAL                   0
#define RCC                       1

#define PLL_HSI_DIVIDED_BY_2      0
#define PLL_HSE                   1
#define PLL_HSE_DIVIDED_BY_2      2

#define PLL_CLOCK_MULTIPLE_BY_5     0
#define PLL_CLOCK_MULTIPLE_BY_6     1
#define PLL_CLOCK_MULTIPLE_BY_6_5   2
#define PLL_CLOCK_MULTIPLE_BY_7     3
#define PLL_CLOCK_MULTIPLE_BY_8     4
#define PLL_CLOCK_MULTIPLE_BY_9     5




void MRCC_voidInitSysClock(void);
void MRCC_voidEnableClock(u8 Copy_u8BusId, u8 Copy_u8PerifId );
void MRCC_voidDisableClock(u8 Copy_u8BusId, u8 Copy_u8PerifId );



#endif
