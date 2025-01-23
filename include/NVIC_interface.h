/**************************************************************************************************/
/* Author  : Kareem Magdy                                                                         */
/* Version : V01                                                                                  */
/* Date    : 16 Jan 2023                                                                           */
/**************************************************************************************************/
#ifndef _NVIC_INTERFACE_H
#define _NVIC_INTERFACE_H


#define NVIC_EXTI0       6
#define NVIC_EXTI1       7 
#define NVIC_EXTI2       8 
#define NVIC_EXTI3       9 
#define NVIC_EXTI4       10
#define NVIC_EXTI5_9     23 
#define NVIC_EXTI10_15   40 
#define NVIC_UART1       37
#define NVIC_ADC12       18


#define Periority_Zero  	0
#define Periority_One   	1
#define Periority_Two   	2
#define Periority_Three 	3
#define Periority_From_Five 4
#define Periority_From_Ten  5

#define Periority_Level0    0


/*
#define FIRST_POSITION		0b0001
#define SECOND_POSITION		0b0010
#define THIRD_POSITION 		0b0100
#define FOURTH_POSITION 	0b1000
*/


void MNVIC_voidEnableInterrupt( u8 Copy_u8IntPeriNumber );
void MNVIC_voidDisableInterrupt( u8 Copy_u8IntPeriNumber );
void MNVIC_voidSetPendingFlag( u8 Copy_u8IntPeriNumber );
void MNVIC_voidClearPendingFlag( u8 Copy_u8IntPeriNumber );

u8 MNVIC_u8ReadActiveFlag( u8 Copy_u8IntPeriNumber );

void MNVIC_u8SetPeriority( s8 Copy_u8IntID, u8 Copy_u8IntPiriority);

#endif
