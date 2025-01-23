/**************************************************************************************************/
/* Author  : Kareem Magdy                                                                         */
/* Version : V01                                                                                  */
/* Date    : 17 Jan 2023                                                                           */
/**************************************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "EXTI_interface.h"
#include "EXTI_private.h"
#include "RCC_interface.h"
#include "NVIC_interface.h"
#include "AFIO_private.h"


void (*EXT_INT0)      (void);
void (*EXT_INT1)      (void);
void (*EXT_INT2)      (void);
void (*EXT_INT3)      (void);
void (*EXT_INT4)      (void);
void (*EXT_INT9_5)    (void);
void (*EXT_INT15_10)  (void);



/*
void EXTI0_IRQHandler (void)
{
	EXT_INT0() ;
}
*/

// IRQ (short for Interrupt Request)
void EXTI0_IRQHandler (void)     {EXT_INT0() ;  }
void EXTI1_IRQHandler (void)     {EXT_INT1() ;  }
void EXTI2_IRQHandler (void)     {EXT_INT2() ;  }
void EXTI3_IRQHandler (void)     {EXT_INT3() ;  }
void EXTI4_IRQHandler (void)     {EXT_INT4() ;  }
void EXTI9_5_IRQHandler (void)   {EXT_INT9_5() ;  }
void EXTI15_10_IRQHandler (void) {EXT_INT15_10() ;  }


void  MEXTI_voidEnable(u8 Copy_u8EXTI_PORT, u8 Copy_u8PIN, u8 Copy_u8SenseMode ) 
{   
    /*Enable RCC_AFIO Bus*/
    MRCC_voidEnableClock(RCC_APB2,AFIO_BUS);
	
    /*Enable AFIO External Interrupt for The PIN*/

	MAFIO-> EXTICR[ ( Copy_u8PIN /4) ] &=~  ((0b1111)  << ( Copy_u8PIN * 4));
	MAFIO-> EXTICR[ ( Copy_u8PIN /4 )] |=  ( Copy_u8EXTI_PORT  << ( Copy_u8PIN * 4));

	
	switch (Copy_u8SenseMode) { 
	
	case RISING_EDGE:    SET_BIT( MEXTI->RTSR , Copy_u8PIN );break;
	case FALLING_EDGE:   SET_BIT( MEXTI->FTSR , Copy_u8PIN );break;
	case EDGE_CHANGE: SET_BIT( MEXTI->RTSR , Copy_u8PIN ); SET_BIT( MEXTI->FTSR , Copy_u8PIN );break;
	
	} 
	
	/* Enable Line Interrupt ( PIE )*/
	SET_BIT( MEXTI->IMR , Copy_u8PIN );
	
	/* Enable NVIC for EXTI*/
	if( Copy_u8PIN <= 4 )
		
	MNVIC_voidEnableInterrupt(  ( Copy_u8PIN + 6 )  );
	
	else if ( Copy_u8PIN >= 5 && Copy_u8PIN <=9 )
		
	MNVIC_voidEnableInterrupt( NVIC_EXTI5_9 );
	
	else if ( Copy_u8PIN >= 10 && Copy_u8PIN <= 15  )
		
	MNVIC_voidEnableInterrupt( NVIC_EXTI10_15 );
	
	else {};

}
void  MEXTI_voidDisable(u8 Copy_u8PIN ) 
{
	/* Disable Line Interrupt*/
	CLR_BIT( MEXTI->IMR , Copy_u8PIN );
		
}

void  MEXTI_voidSwTrigger(u8 Copy_u8PIN)
{    /*    Enable SW Line Interrupt*/
		SET_BIT( (MEXTI->SWIER) , Copy_u8PIN );
}

void MEXTI_voidClearPendingFlag( u8 Copy_u8EXTI_Type ) {  

/* CLear PIF By Setting 1 on the selected EXT*/
 SET_BIT( MEXTI->PR , Copy_u8EXTI_Type );

} 

/*
void MEXTINT0_VidSetCallBack ( void (*INT_ISR ) (void) )
{
	EXT_INT0=INT_ISR ;
}
*/

void MEXTINT0_VidSetCallBack ( void (*INT_ISR ) (void) ) {  EXT_INT0=INT_ISR ; }
void MEXTINT1_VidSetCallBack ( void (*INT_ISR ) (void) ) {  EXT_INT1=INT_ISR ; }
void MEXTINT2_VidSetCallBack ( void (*INT_ISR ) (void) ) {  EXT_INT2=INT_ISR ; }
void MEXTINT3_VidSetCallBack ( void (*INT_ISR ) (void) ) {  EXT_INT3=INT_ISR ; }
void MEXTINT4_VidSetCallBack ( void (*INT_ISR ) (void) ) {  EXT_INT4=INT_ISR ; }
void MEXTINT9_5_VidSetCallBack   ( void (*INT_ISR ) (void) ) {  EXT_INT9_5=INT_ISR ; }
void MEXTINT15_10_VidSetCallBack ( void (*INT_ISR ) (void) ) {  EXT_INT15_10=INT_ISR ; }


void  MEXTI_voidEnableFast(u8 Copy_u8EXTI_PORT)
{
	SET_BIT( MEXTI->IMR , Copy_u8EXTI_PORT);
}
