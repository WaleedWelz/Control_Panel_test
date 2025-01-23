/**************************************************************************************************/
/* Author  : Kareem Magdy                                                                         */
/* Version : V01                                                                                  */
/* Date    : 16 Jan 2023                                                                           */
/**************************************************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"


#include "NVIC_interface.h"
#include "NVIC_private.h"
#include "NVIC_config.h"

void MNVIC_voidEnableInterrupt( u8 Copy_u8IntPeriNumber )
{ 

	if( Copy_u8IntPeriNumber <= 31 ){

		NVIC_ISER0 = ( 1 << Copy_u8IntPeriNumber );

	}		
	else if( Copy_u8IntPeriNumber >= 32 &&  Copy_u8IntPeriNumber <= 59 ) {

		Copy_u8IntPeriNumber -=32;

		NVIC_ISER1 = ( 1 << Copy_u8IntPeriNumber );

	} 		
} 

void MNVIC_voidDisableInterrupt( u8 Copy_u8IntPeriNumber )
{ 

	if( Copy_u8IntPeriNumber <= 31 ){

		NVIC_ICER0 = ( 1 << Copy_u8IntPeriNumber );

	}		
	else if( Copy_u8IntPeriNumber >= 32 &&  Copy_u8IntPeriNumber <= 59 ) {

		Copy_u8IntPeriNumber -=32;

		NVIC_ICER1 = ( 1 << Copy_u8IntPeriNumber );

	} 		
} 

void MNVIC_voidSetPendingFlag( u8 Copy_u8IntPeriNumber )
{ 

	if( Copy_u8IntPeriNumber <= 31 ){

		NVIC_ISPR0 = ( 1 << Copy_u8IntPeriNumber );

	}		
	else if( Copy_u8IntPeriNumber >= 32 &&  Copy_u8IntPeriNumber <= 59 ) {

		Copy_u8IntPeriNumber -=32;

		NVIC_ISPR1 = ( 1 << Copy_u8IntPeriNumber );

	} 		
} 
void MNVIC_voidClearPendingFlag( u8 Copy_u8IntPeriNumber )
{

	if( Copy_u8IntPeriNumber <= 31 ){

		NVIC_ICPR0 = ( 1 << Copy_u8IntPeriNumber );

	}		
	else if( Copy_u8IntPeriNumber >= 32 &&  Copy_u8IntPeriNumber <= 59 ) {

		Copy_u8IntPeriNumber -=32;

		NVIC_ICPR1 = ( 1 << Copy_u8IntPeriNumber );

	}


}

u8 MNVIC_u8ReadActiveFlag( u8 Copy_u8IntPeriNumber )
{ 
	u8 Local_FlagStatus=0;

	if( Copy_u8IntPeriNumber <= 31 ){

		Local_FlagStatus = GET_BIT(NVIC_IABR0,Copy_u8IntPeriNumber);

	}		
	else if( Copy_u8IntPeriNumber >= 32 &&  Copy_u8IntPeriNumber <= 59 ) {

		Copy_u8IntPeriNumber -=32;

		Local_FlagStatus = GET_BIT( NVIC_IABR1 , Copy_u8IntPeriNumber);

	}

	return Local_FlagStatus;
}



void MNVIC_u8SetPeriority( s8 Copy_u8IntID, u8 Copy_u8IntPiriority)
{


	if(Copy_u8IntID>=0)
	{
			NVIC_IPR[Copy_u8IntID]= Copy_u8IntPiriority <<4;
	}

}
