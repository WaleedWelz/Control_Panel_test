/***************************************************************************************************/
/* Author  : Kareem Magdy                                                                          */
/* Version : V01                                                                                   */
/* Date    : 17 Jan 2023                                                                            */
/* Company : AIO                                                                           */
/**************************************************************************************************/

#ifndef _EXTI_INTERFACE_H
#define _EXTI_INTERFACE_H

#define   RISING_EDGE        0
#define   FALLING_EDGE       1
#define   EDGE_CHANGE        2

#define  EXTI_PORTA   0b0000
#define  EXTI_PORTB   0b0001
#define  EXTI_PORTC   0b0010

void  MEXTI_voidEnable(u8 Copy_u8EXTI_PORT, u8 Copy_u8PIN, u8 Copy_u8SenseMode );
void  MEXTI_voidDisable(u8 Copy_u8PIN );
void  MEXTI_voidSwTrigger(u8 Copy_u8PIN);
void MEXTI_voidClearPendingFlag( u8 Copy_u8EXTI_Type );

void MEXTINT0_VidSetCallBack ( void (*INT_ISR ) (void) );
void MEXTINT1_VidSetCallBack ( void (*INT_ISR ) (void) );
void MEXTINT2_VidSetCallBack ( void (*INT_ISR ) (void) );
void MEXTINT3_VidSetCallBack ( void (*INT_ISR ) (void) );
void MEXTINT4_VidSetCallBack ( void (*INT_ISR ) (void) );
void MEXTINT9_5_VidSetCallBack   ( void (*INT_ISR ) (void) );
void MEXTINT15_10_VidSetCallBack ( void (*INT_ISR ) (void) );

void  MEXTI_voidEnableFast(u8 Copy_u8EXTI_PORT);

#endif
