/*
 * Control_Panel.h
 *
 *  Created on: Aug 11, 2024
 *      Author: RD17
 */

#ifndef CONTROL_PANEL_H_
#define CONTROL_PANEL_H_



/*
 * Control_Panel.c
 *
 *  Created on: Aug 11, 2024
 *      Author: RD17
 */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "GPIO_private.h"
#include "SYS_interface.h"
#include "NVIC_interface.h"
#include "UART_interface.h"
#include "EXTI_interface.h"

#define STARTUP_MESSAGE_LETTERS_NUMBER    5
#define PORT_PINS_COUNTER                 15

#define PORT_PIN0                         0
#define PORT_PIN1                         1
#define PORT_PIN2                         2
#define PORT_PIN5                         3
#define PORT_PIN6                         4
#define PORT_PIN11                        5
#define PORT_PIN12                        6
#define PORT_PIN13                        7
#define PORT_PIN14                        8
#define PORT_PIN15                        9

#define FV_NO_STATE   0
#define NFV_FLAG   1
#define MFV_FLAG   2
#define WFV_FLAG   1



#define ECHO_NO_STATE   0
#define LECHO_FLAG   1
#define FECHO_FLAG   2
#define OFF_FLAG     1


#define COAXIAL_NO_STATE 0
#define HEP_IS_SEECTED 1
#define HEP_IS_NOT_SELECTED 2

#define BULLET_NO_STATE   0
#define HIGH_EXPO_FLAG    1
#define HEAT_FLAG         2
#define SABOT_FLAG        1
#define HEP_FLAG          2
#define COAXIAL_GUN_FLAG  1


#define FIRST_ECHO         0
#define LAST_ECHO          1
#define ECHO_OFF           2
#define Coaxial_GUN        3
#define HEP                4
#define SABOT              5
#define HEAT               6
#define HIGH_EXP           7
#define NFOV			   8
#define MFOV			   9
#define WFOV               10

#define ON                 0
#define OFF				   1


enum Status
{
	First_Echo,
	Last_Echo,
	Off,
	Coaxial,
	Hep,
	Sabot,
	Heat,
	High_Exp,
	Nfov,
	Mfov,
	Wfov

};


// Define constants for pin numbers and messages
#define COLOR_PIN           13
#define FOCUS_MINUS_PIN     15
#define FOCUS_PLUS_PIN      14
#define ROUND_IN_BARREL_PIN 10
#define FIELD_RANGE_PIN     11
#define FULL_SCREEN_PIN     5
//#define SHUTDOWN_PIN        6
#define CONTRAST_PLUS_PIN   8
#define CONTRAST_MINUS_PIN  7
#define BRIGHT_PLUS_PIN     1
#define BRIGHT_MINUS_PIN    6
#define EXPOSURE_PLUS_PIN   11
#define EXPOSURE_MINUS_PIN  5
#define AUTO_PIN            4
#define LAMP_TEST_PIN       12

#define COAXIAL_GUN_PIN     11
#define HEP_PIN             12
#define SABOT_PIN           13
#define HEAT_PIN            14
#define HIGH_EXP_PIN        15

#define CLK_PIN   2
#define SH_LD_PIN 3
#define QH_PIN    4

/* Struct to define the pin to loop over it in initialization,
 * the message that will be sent,
 * The function that will be called and executed */

typedef struct {
	u8 pin;
	const char *message;
	void (*action)(void);
} PinAction;



/* Struct to define the pin to loop over it in initialization,
 * the message that will be sent,
 * the type of bullet to use it in startup,
 * The function that will be called and executed
 * The value(i.e Copy_u8Message)that will be sent to the function (i.e Control_Panelvoid_Message_For_LED)*/
typedef struct {
	u8 pin2;
	const char *message2;
	enum Status Bullet_type;
	void (*action2)(u8);
	u8 Value;
} PinAction2;



// Function prototypes
void handleLampTest(void);
void resetLamps(void);
void executeActions(void);
void executeActions2(void);
void executeAction(PinAction action);
void executeAction2(PinAction2 action2);
void Control_Panelvoid_Init(void);
void Control_Panel_voidClr_Flags(void);
void Control_Panel_voidStartUpLeds(void);
void Control_Panelvoid_Message_For_LED(u8 Copy_u8Message);
void LED_voidLampTest(u8 Copy_u8status);
u16  ShiftRegister_u16GetData(void);
void DataEntry_VoidWriteStringAndNumbers(u8 *Copy_u8StringToBeSent,u32 Copy_u32RealNumber);






#endif /* CONTROL_PANEL_H_ */
