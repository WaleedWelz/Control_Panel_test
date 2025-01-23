/***************************************************************************************************/
/* Author  : Kareem Magdy                                                                          */
/* Version : V01                                                                                   */
/* Date    : 17 Jan 2023                                                                            */
/* Company : AIO                                                                            */
/***************************************************************************************************/



#ifndef _EXTI_PRIVATE_H
#define _EXTI_PRIVATE_H



typedef struct {
    
	volatile u32 IMR;
	volatile u32 EMR;
	volatile u32 RTSR;
	volatile u32 FTSR;
	volatile u32 SWIER;
	volatile u32 PR;
	
	}EXTI;

#define  MEXTI        ((volatile EXTI*)0x40010400)


void MEXTI_voidClearPendingFlag( u8 Copy_u8EXTI_Type );

#endif


