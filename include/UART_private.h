/***************************************************************************************************/
/* Author  : Enjy Mohamed                                                                          */
/* Version : V01                                                                                   */
/* Date    : 10 Feb 2023                                                                            */
/* Company : AIO                                                                            */
/***************************************************************************************************/

#ifndef UART_PRIVATE_H
#define UART_PRIVATE_H


/*  base address for UART1 peripheral  */

#define USART1_base   0x40013800

/*  defining registers used in addressing UART1  */

#define USART1_SR     (*(volatile u32 *)(USART1_base+0x00))  /* Status Register */
#define USART1_DR     (*(volatile u32 *)(USART1_base+0x04))  /* Data Register */
#define USART1_BRR    (*(volatile u32 *)(USART1_base+0x08))  /* Baud Rate Register */
#define USART1_CR1    (*(volatile u32 *)(USART1_base+0x0C))  /* Control Register 1 */
#define USART1_CR2    (*(volatile u32 *)(USART1_base+0x14))  /* Control Register 2 */




#endif /* UART_PRIVATE_H */
