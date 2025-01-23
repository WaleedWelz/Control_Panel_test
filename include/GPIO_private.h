/***************************************************************************************************/
/* Author  : Kareem Magdy                                                                          */
/* Version : V01                                                                                   */
/* Date    : 5 Jan 2023                                                                            */
/* Company : AIO                                                                            */
/***************************************************************************************************/

#ifndef _GPIO_PRIVATE_H
#define _GPIO_PRIVATE_H

/* Register Definiation   */ 
typedef struct {
	
	u32 CRL;
	u32 CRH;
	u32 IDR;
	u32 ODR;
	u32 BSRR;
	u32 BRR;
	u32 LCKR;
	
}GPIO;


#define GPIO_PORTA ((volatile GPIO*)0x40010800)
#define GPIO_PORTB ((volatile GPIO*)0x40010C00)
#define GPIO_PORTC ((volatile GPIO*)0x40011000)


#endif
