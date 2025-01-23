/***************************************************************************************************/
/* Author  : Kareem Magdy                                                                          */
/* Version : V01                                                                                   */
/* Date    : 5 Jan 2023                                                                            */
/* Company : AIO                                                                            */
/***************************************************************************************************/

#ifndef _GPIO_INTERFACE_H
#define _GPIO_INTERFACE_H


/* Options: PIN VALUE */
#define  HIGH           1
#define  LOW            0

#define HIGH_PORTL      0x00FF
#define HIGH_PORTH      0xFF00


#define PORTA       0
#define PORTB       1
#define PORTC       2

/* Options: INPUT Direction Modes */
#define INPUT_ANALOG                        0b0000
#define INPUT_FLOATING                      0b0100
#define INPUT_PULLUP_DOWN                   0b1000

/* Options: OUTPUT@10MHZ Direction Modes */
#define OUTPUT_10MHZ_PUSHPULL               0b0001
#define OUTPUT_10MHZ_OPENDRAIN              0b0101
#define OUTPUT_10MHZ_AF_PUSHPULL            0b1001
#define OUTPUT_10MHZ_AF_OPENDRAIN           0b1101

/* Options: OUTPUT@2MHZ Direction Modes */
#define OUTPUT_2MHZ_PUSHPULL                0b0010
#define OUTPUT_2MHZ_OPENDRAIN               0b0110
#define OUTPUT_2MHZ_AF_PUSHPULL             0b1010
#define OUTPUT_2MHZ_AF_OPENDRAIN            0b1110

/* Options: OUTPUT@50MHZ Direction Modes */
#define OUTPUT_50MHZ_PUSHPULL               0b0011
#define OUTPUT_50MHZ_OPENDRAIN              0b0111
#define OUTPUT_50MHZ_AF_PUSHPULL            0b1011
#define OUTPUT_50MHZ_AF_OPENDRAIN           0b1111

#define PIN0            0
#define PIN1            1
#define PIN2            2
#define PIN3            3
#define PIN4            4
#define PIN5            5
#define PIN6            6
#define PIN7            7
#define PIN8            8
#define PIN9            9
#define PIN10           10
#define PIN11           11
#define PIN12           12
#define PIN13           13
#define PIN14           14
#define PIN15           15


void MGPIO_voidSetPinDirection(u8 Copy_u8Port , u8 Copy_u8Pin, u8 Copy_u8Direction );
void MGPIO_voidSetPinValue(u8 Copy_u8Port , u8 Copy_u8Pin, u8 Copy_u8Value );
u8   MGPIO_u8GetPinValue(u8 Copy_u8Port ,u8 Copy_u8Pin );
void MGPIO_VidSetPortDirection( u8 Copy_u8Port , u8 Copy_u8Position , u8 Copy_u8Mode );
void MGPIO_VidSetPortValue( u8 Copy_u8Port , u8 Copy_u8Position , u16 Copy_u8Value );


#endif
