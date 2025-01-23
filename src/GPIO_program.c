/***************************************************************************************************/
 /* Author  : Kareem Magdy                                                                          */
/* Version : V01                                                                                   */
/* Date    : 5 Jan 2023                                                                            */
/* Company : AIO                                                                            */
/***************************************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"


#include "GPIO_interface.h"
#include "GPIO_private.h"



void MGPIO_voidSetPinDirection(u8 Copy_u8Port , u8 Copy_u8Pin, u8 Copy_u8Direction ) {
         
		/* Check The PIN number CRL -> (0-7) , CRH ->( 8-15 ) */
		if( Copy_u8Pin <= PIN7 ) {
		 
        switch( Copy_u8Port ) {
	        
			/* Change the Required 4 Bits to 0000 then oring with the required direction value  */
	        case PORTA: GPIO_PORTA -> CRL&=~( (0b1111)<< (Copy_u8Pin * 4 ) ) ;
			            GPIO_PORTA -> CRL|=( Copy_u8Direction << (Copy_u8Pin * 4 )); break; 
			case PORTB: GPIO_PORTB -> CRL&=~( (0b1111)<< (Copy_u8Pin * 4 ) ) ;
			            GPIO_PORTB -> CRL|=( Copy_u8Direction << (Copy_u8Pin * 4 )); break;
			case PORTC: GPIO_PORTC -> CRL&=~( (0b1111)<< (Copy_u8Pin * 4 ) ) ;
			            GPIO_PORTC -> CRL|=( Copy_u8Direction << (Copy_u8Pin * 4 )); break;

        }
		} 
		
		else if ( Copy_u8Pin <= PIN15 ) {
			
			Copy_u8Pin -= 8;
			
		switch( Copy_u8Port ) {
	       
	        case PORTA: GPIO_PORTA -> CRH&=~( (0b1111)<< (Copy_u8Pin * 4 ) ) ;
			            GPIO_PORTA -> CRH|=( Copy_u8Direction << (Copy_u8Pin * 4 )); break; 
			case PORTB: GPIO_PORTB -> CRH&=~( (0b1111)<< (Copy_u8Pin * 4 ) ) ;
			            GPIO_PORTB -> CRH|=( Copy_u8Direction << (Copy_u8Pin * 4 )); break;
			case PORTC: GPIO_PORTC -> CRH&=~( (0b1111)<< (Copy_u8Pin * 4 ) ) ;
			            GPIO_PORTC -> CRH|=( Copy_u8Direction << (Copy_u8Pin * 4 )); break;

        }	
			
		}

}

void MGPIO_voidSetPinValue(u8 Copy_u8Port , u8 Copy_u8Pin, u8 Copy_u8Value ) {
	
	
	switch(Copy_u8Value){
		
       case HIGH:  switch( Copy_u8Port ){ 

		                 case PORTA: GPIO_PORTA->BSRR=(1<<Copy_u8Pin);break;//SET_BIT(GPIO_PORTA->ODR,Copy_u8Pin);break;
						 case PORTB: GPIO_PORTB->BSRR=(1<<Copy_u8Pin);break;//SET_BIT(GPIO_PORTB->ODR,Copy_u8Pin);break;
						 case PORTC: GPIO_PORTC->BSRR=(1<<Copy_u8Pin);break;//SET_BIT(GPIO_PORTC->ODR,Copy_u8Pin);break;
		
		} break;
		
		case LOW:  switch( Copy_u8Port ){ 

		                 case PORTA: GPIO_PORTA->BRR=(1<<Copy_u8Pin);break;//CLR_BIT(GPIO_PORTA->ODR,Copy_u8Pin);break;
						 case PORTB: GPIO_PORTB->BRR=(1<<Copy_u8Pin);break;//CLR_BIT(GPIO_PORTB->ODR,Copy_u8Pin);break;
						 case PORTC: GPIO_PORTC->BRR=(1<<Copy_u8Pin);break;//CLR_BIT(GPIO_PORTC->ODR,Copy_u8Pin);break;
		} break;
	
	
	}

}

u8 MGPIO_u8GetPinValue(u8 Copy_u8Port , u8 Copy_u8Pin ) {
	
	u8 Local_u8PinValue=0;
         switch( Copy_u8Port ){ 

		    case PORTA: Local_u8PinValue= GET_BIT(GPIO_PORTA->IDR,Copy_u8Pin); break;
			case PORTB: Local_u8PinValue= GET_BIT(GPIO_PORTB->IDR,Copy_u8Pin);break;
			case PORTC: Local_u8PinValue= GET_BIT(GPIO_PORTC->IDR,Copy_u8Pin);break;
		
		} 
		
	return Local_u8PinValue;	
			
}



void MGPIO_VidSetPortDirection( u8 Copy_u8Port , u8 Copy_u8Position , u8 Copy_u8Mode ){

	switch( Copy_u8Port ){

	case PORTA:

		if      ( Copy_u8Position == LOW  ) { GPIO_PORTA->CRL = ( 0x11111111 * Copy_u8Mode ) ; }
		else if ( Copy_u8Position == HIGH ) { GPIO_PORTA->CRH = ( 0x11111111 * Copy_u8Mode ) ; }

		break;
	case PORTB:

		if      ( Copy_u8Position == LOW  ) { GPIO_PORTB->CRL = ( 0x11111111 * Copy_u8Mode ) ; }
		else if ( Copy_u8Position == HIGH ) { GPIO_PORTB->CRH = ( 0x11111111 * Copy_u8Mode ) ; }

		break;
	case PORTC:

		if      ( Copy_u8Position == LOW  ) { GPIO_PORTC->CRL = ( 0x11111111 * Copy_u8Mode ) ; }
		else if ( Copy_u8Position == HIGH ) { GPIO_PORTC->CRH = ( 0x11111111 * Copy_u8Mode ) ; }

		break;

	}

}

void MGPIO_VidSetPortValue( u8 Copy_u8Port , u8 Copy_u8Position , u16 Copy_u8Value ){

	switch( Copy_u8Port ){

	case PORTA:

		if      ( Copy_u8Position == LOW  ){ GPIO_PORTA->ODR = (GPIO_PORTA->ODR & 0xFF00 ) | ( (u8)Copy_u8Value ) ; }
		else if ( Copy_u8Position == HIGH ){ GPIO_PORTA->ODR = (GPIO_PORTA->ODR & 0x00FF ) | (     Copy_u8Value ) ; }

		break;
	case PORTB:

		if      ( Copy_u8Position == LOW  ){ GPIO_PORTB->ODR = (GPIO_PORTB->ODR & 0xFF00 ) | ( (u8)Copy_u8Value ) ; }
		else if ( Copy_u8Position == HIGH ){ GPIO_PORTB->ODR = (GPIO_PORTB->ODR & 0x00FF ) | (     Copy_u8Value ) ; }

		break;
	case PORTC:

		if      ( Copy_u8Position == LOW  ){ GPIO_PORTC->ODR = (GPIO_PORTC->ODR & 0xFF00 ) | ( (u8)Copy_u8Value ) ; }
		else if ( Copy_u8Position == HIGH ){ GPIO_PORTC->ODR = (GPIO_PORTC->ODR & 0x00FF ) | (     Copy_u8Value ) ; }

		break;

	}

}

