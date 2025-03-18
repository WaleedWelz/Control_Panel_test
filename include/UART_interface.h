/***************************************************************************************************/
/* Author  : Enjy Mohamed                                                                          */
/* Version : V01                                                                                   */
/* Date    : 10 Feb 2023                                                                            */
/* Company : AIO                                                                            */
/***************************************************************************************************/

#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H



/* definitions for USART1_TRANSMITTER state in config */
#define TRANSMITTER_DISABLE     0
#define TRANSMITTER_ENABLE      1


/* definitions for USART1_RECEIVER state in config */
#define RECEIVER_DISBLE         0
#define RECEIVER_ENABLE         1



void USART_Init (void);  /* initialization function for UART1 */
void USART1_IRQHandler (void);   /* Interrupt Handler of UART1 */

/* a cast of type u8 * should be imposed before writing the string */
void USART1_VoidWriteString    (u8 *Copy_u8StringToBeSent);


void USART1_VoidWriteCharacter (u8  Copy_u8CharacterToBeSent);
u8 USART1_u8ReadCharacter  (void);
u8 * USART1_u8ReadString (void);
void USART1_VoidConvertToString(u16 Copy_u16OutputOfConversion);
void USART1_VoidOutputPotent(u16 Copy_u16ReadingOfPotent);
void USART1_VoidOutputTempSensor(u32 Copy_u16ReadingOfTempSensor);


void USART1_VoidWriteNumber(u32 Copy_u32TotalNumber);
void printBinary(u8 num) ;
void printBinary16(u16 num);
u8 USART1_ReceiveByteNonBlocking(u8 *data);
u8* USART1_u8ReadString_NonBlocking();

#endif /* UART_INTERFACE_H_ */

