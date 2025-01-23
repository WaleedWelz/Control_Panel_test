/***************************************************************************************************/
/* Author  : Enjy Mohamed                                                                          */
/* Version : V01                                                                                   */
/* Date    : 10 Feb 2023                                                                           */
/* Company : AIO                                                                                   */
/***************************************************************************************************/

#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "UART_private.h"
#include "UART_config.h"
#include "UART_interface.h"
#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "SYS_interface.h"
//#include "ADC_private.h"

void USART_Init (void) {


	/* calculating the integer part of the divider */

	u32 LOC_u64Mantissa = (SystemClkInMHz) / ( 16 * USART1_BAUD_RATE ) ;

	/* calculating the fraction part of the divider */

	u32 LOC_u64Fraction = ( ( (SystemClkInMHz * 100 ) / ( 16 * USART1_BAUD_RATE )  ) % 100 ) * 16 ;

	/*  if fractional part exceeds a certain limit it can be considered as an integer 1  */
	/* applying ceil function without actually using ceil */
	/* because if it exceeds 0.5 it is considered as the next number */

		if( LOC_u64Fraction > 1500 ){ LOC_u64Mantissa += 1 ; LOC_u64Fraction  = 0 ; }

		/* enabling UART clock */

	MRCC_voidEnableClock( RCC_APB2, UART_PIN);

	/* enabling PORTA clock */

	MRCC_voidEnableClock( RCC_APB2, GPIO_PORTA_BUS);

	/* enabling PORTA AF clock for UART function  */

    MRCC_voidEnableClock(RCC_APB2,AFIO_BUS);

    /* clearing status register to clear all flags  */

    USART1_SR = 0;

    /* defining word length as 1 Start bit, 8 Data bits, n Stop bit */

    CLR_BIT(USART1_CR1, PIN12);

    /* bits 12 & 13 in CR2 define stop bits number */
    /* setting them to 00 means 1 stop bit */

    CLR_BIT(USART1_CR2, PIN12);

    CLR_BIT(USART1_CR2, PIN13);

    /* preprocessor to enable or disable TX depending on UART_config */


 #if USART1_TRANSMITTER == TRANSMITTER_ENABLE
    SET_BIT (USART1_CR1, PIN3);


#elif   USART1_TRANSMITTER == TRANSMITTER_DISABLE
    CLR_BIT (USART1_CR1, PIN3);

#endif


    /* preprocessor to enable or disable RX depending on UART_config */


#if USART1_RECEIVER == RECEIVER_ENABLE
          SET_BIT (USART1_CR1, PIN2);


#elif USART1_RECEIVER == RECEIVER_DISABLE
          CLR_BIT (USART1_CR1, PIN2);

#endif


/* setting Baud Rate Register value */

/* Mantissa or integer represents first 4 bits of the register */

/* fraction represents the rest of the bits of the register */


	REG_VALUE(USART1_BRR,( ( LOC_u64Mantissa << 4  ) | ( LOC_u64Fraction / 100 )));


/*  setting bit 13 in CR1: UE bit which is UART enable  */


	SET_BIT (USART1_CR1, PIN13);


/* According to STM mapping,  PA9 is TX of UART1 */
/* According to STM mapping, PA10 is RX of UART1 */
/* PA9 is configured as output pushpull pin */
/* PA10 is configured as input pulled to VCC */


	MGPIO_voidSetPinDirection(PORTA, PIN9, OUTPUT_10MHZ_AF_PUSHPULL);

	MGPIO_voidSetPinDirection(PORTA, PIN10, INPUT_FLOATING);
	MGPIO_voidSetPinValue(PORTA,PIN10,HIGH);

}

void USART1_VoidWriteString(u8 *Copy_u8StringToBeSent){
 /* a cast of type u8 * should be imposed before writing the string */
u32 i = 0;
while (Copy_u8StringToBeSent[i]){
	USART1_VoidWriteCharacter (Copy_u8StringToBeSent[i]);
	i++;
}

}

void USART1_VoidWriteCharacter (u8  Copy_u8CharacterToBeSent){
	CLR_BIT(USART1_SR,PIN6);
	USART1_DR = Copy_u8CharacterToBeSent;
	while (GET_BIT(USART1_SR,PIN6) ==LOW);
	/*  waiting for TXE flag to be raised to indicate that the transmitter is now empty   */
	/*  and hence, transmission is complete  */
}

u8 USART1_u8ReadCharacter(){
	u8 data_to_return;

while (GET_BIT (USART1_SR, PIN5) == LOW);
data_to_return = (u8)USART1_DR ;
//CLR_BIT(USART1_SR,PIN5);
return (data_to_return);
}


u8 String[20];
u8 * USART1_u8ReadString () {


		u8 factor = 0 , last_entered_data ;

/* checking on the condition that enter is not pressed */

/* 13 is the ASCII code for new line */

		while( ( last_entered_data = USART1_u8ReadCharacter() ) != 13 ){

			String[ factor ] = last_entered_data ;
			USART1_VoidWriteCharacter(last_entered_data);
			factor++;
		}

		String[factor] = '\0';

		return ( String );

	}

void USART1_VoidOutputPotent(u16 Copy_u16NumberWanted){
	u16 temp;
	temp = ((Copy_u16NumberWanted * 3.3)/4095)*1000;
USART1_VoidWriteCharacter((temp/1000)+48);

USART1_VoidWriteCharacter('.');

temp = temp % 1000;
USART1_VoidWriteCharacter((temp/100)+48);

temp = temp % 100;
USART1_VoidWriteCharacter((temp/10)+48);

temp = temp % 10;
USART1_VoidWriteCharacter(temp+48);

USART1_VoidWriteString((u8 *)"Volt");
delay_voidXms(300);
}

void USART1_VoidOutputTempSensor(u32 Copy_u16ReadingOfTempSensor){
	u32 temp2;
	temp2 = Copy_u16ReadingOfTempSensor;

	if (temp2/100 <10) {
		USART1_VoidWriteCharacter((temp2/100)+48);
		USART1_VoidWriteCharacter('.');
		temp2 = temp2 % 100;
		USART1_VoidWriteCharacter((temp2/10)+48);
		temp2 = temp2 % 10;
		USART1_VoidWriteCharacter(temp2+48);
		USART1_VoidWriteCharacter ('C');
		delay_voidXms(300);

}
	else if (temp2/100 >=10){
		USART1_VoidWriteCharacter((temp2/1000)+48);
		temp2 = temp2 % 1000;
		USART1_VoidWriteCharacter((temp2/100)+48);
				USART1_VoidWriteCharacter('.');
				temp2 = temp2 % 100;
				USART1_VoidWriteCharacter((temp2/10)+48);
				temp2 = temp2 % 10;
				USART1_VoidWriteCharacter(temp2+48);
				USART1_VoidWriteCharacter ('C');
				delay_voidXms(300);
	}
/*	USART1_VoidWriteCharacter((temp2/10000)+48);

	USART1_VoidWriteCharacter('.');

	temp2 = temp2 % 10000;
	USART1_VoidWriteCharacter((temp2/100)+48);

	temp2 = temp2 % 100;
	USART1_VoidWriteCharacter((temp2/10)+48);

	temp2 = temp2 % 10;
	USART1_VoidWriteCharacter(temp2+48);

	USART1_VoidWriteString((u8 *)"Volt");
	delay_voidXms(500);*/
}


void USART1_VoidWriteNumber(u32 Copy_u32TotalNumber)
{
	u8 Counter= 0, ReminderOne = 0;
	u32 Temp = Copy_u32TotalNumber;

	if (Copy_u32TotalNumber == 0) {
		USART1_VoidWriteCharacter('0');
		return; // Early return to avoid further execution
	}

	while (Temp != 0)  // Loop to know the number of digits
	{
		Temp = Temp / 10;
		Counter++;
	}

	u8 ArrIntegerValues[Counter];

	Temp = Copy_u32TotalNumber; // Reset Temp to TotalNumber

	// Store digits in reverse order
	for (u8 s = 0; s < Counter; s++)
	{
		ReminderOne = Temp % 10;
		ArrIntegerValues[s] = ReminderOne;
		Temp = Temp / 10;
	}

	// Send digits in correct order
	for (s8 s = Counter - 1; s >= 0; s--)
	{
		USART1_VoidWriteCharacter(ArrIntegerValues[s] + '0'); // Adding '0' converts digit to ASCII
	}
}

void printBinary(u8 num) {
    for (s8 i = 7; i >= 0; i--) { // Loop from the most significant bit (MSB) to the least significant bit (LSB)
        u8 bit = (num >> i) & 1;  // Extract each bit
        USART1_VoidWriteCharacter(bit ? '1' : '0'); // Send the bit as a character using USART
    }
    USART1_VoidWriteCharacter('\n'); // Add a newline for better formatting
}
