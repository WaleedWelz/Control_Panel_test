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
#include "Control_Panel.h"



extern u8 Global_u8DTStateFlag;
extern u8 Global_u8FVState;
extern u8 Global_u8EchoState;
extern u8 Global_u8BulletState;

extern u8 Global_u8Day_Thermal_Flag;
extern u8 Global_u8Lamp_test_Status_Flag;
extern u8 Global_u8Echo_Status_Flag;
extern u8 Global_u8FV_Status_Flag;
extern enum Status Current_LED_Echo;
extern enum Status Current_LED_Bullet;
extern u8 Global_u8Bullets_Flag;
/*
 * Breif : This Function is Initializing The Control Panel pins.
 * Parameters :  Nothing
 * return : Nothing
 */
void Control_Panelvoid_Init(void)
{
	// New config

	/* Initialize PORTA as INPUT PULLDOWN
	 *   pins */

	MGPIO_voidSetPinDirection(PORTA,4,INPUT_PULLUP_DOWN);






	MGPIO_voidSetPinDirection(PORTA,PIN2,OUTPUT_10MHZ_PUSHPULL);
	MGPIO_voidSetPinDirection(PORTA,PIN3,OUTPUT_10MHZ_PUSHPULL);

	MGPIO_voidSetPinDirection(PORTA,PIN12,OUTPUT_10MHZ_PUSHPULL); // EXPOSURE+

	MGPIO_voidSetPinDirection(PORTA,PIN0,INPUT_PULLUP_DOWN); // First Echo LED
	MGPIO_voidSetPinDirection(PORTA,PIN8,INPUT_PULLUP_DOWN); // Contrast+
	MGPIO_voidSetPinDirection(PORTA,PIN11,INPUT_PULLUP_DOWN); // BRIGHT+
	MGPIO_voidSetPinDirection(PORTA,PIN15,INPUT_PULLUP_DOWN); // FULL SCREEN

	MGPIO_voidSetPinValue(PORTA,PIN0,LOW);
	MGPIO_voidSetPinValue(PORTA,PIN8,LOW);
	MGPIO_voidSetPinValue(PORTA,PIN11,LOW);
	MGPIO_voidSetPinValue(PORTA,PIN15,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN3,OUTPUT_10MHZ_PUSHPULL); // SABOT LED
	MGPIO_voidSetPinDirection(PORTB,PIN8,OUTPUT_10MHZ_PUSHPULL); // Last Echo LED
	MGPIO_voidSetPinDirection(PORTB,PIN9,OUTPUT_10MHZ_PUSHPULL); // Last Echo LED


	MGPIO_voidSetPinDirection(PORTC,PIN13,OUTPUT_10MHZ_PUSHPULL); // HEAT LED
	MGPIO_voidSetPinDirection(PORTC,PIN14,OUTPUT_10MHZ_PUSHPULL); // HEP LED
	MGPIO_voidSetPinDirection(PORTC,PIN15,OUTPUT_10MHZ_PUSHPULL); // HIGH EXPO. LED


	/* Initialize PORTB as OUTPUT PUSHPULL
	 *   pins */

	MGPIO_voidSetPinDirection(PORTB,PIN0,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinDirection(PORTB,PIN1,INPUT_PULLUP_DOWN);

	MGPIO_voidSetPinValue(PORTB,PIN0,LOW);
	MGPIO_voidSetPinValue(PORTB,PIN1,LOW);


	MGPIO_voidSetPinDirection(PORTB,PIN4,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN4,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN5,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN5,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN6,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN6,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN7,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN7,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN7,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN7,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN10,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN10,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN11,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN11,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN12,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN12,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN13,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN13,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN14,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN14,LOW);

	MGPIO_voidSetPinDirection(PORTB,PIN15,INPUT_PULLUP_DOWN);
	MGPIO_voidSetPinValue(PORTB,PIN15,LOW);




}


/*
 * Breif : This Function is called when interrupt 0 is occurred. it sends Day/Thermal sentence, setting the global flag, Enabling the LED, Set the Enum Object to the corresponding Status, clear the pending flag.
 * Parameters :  The required LED to be ON based on the Switch position
 * return : Nothing
 */
void Control_Panelvoid_Message_For_LED(u8 Copy_u8Message)
{
	switch(Copy_u8Message)
	{
	case FIRST_ECHO:
		MGPIO_voidSetPinValue(PORTA,PIN12,HIGH);
		MGPIO_voidSetPinValue(PORTB,PIN8,LOW);
		break;

	case LAST_ECHO:
		MGPIO_voidSetPinValue(PORTA,PIN12,LOW);
		MGPIO_voidSetPinValue(PORTB,PIN8,HIGH);break;

	case ECHO_OFF:
		MGPIO_voidSetPinValue(PORTA,PIN12,LOW);
		MGPIO_voidSetPinValue(PORTB,PIN8,LOW);break;

	case Coaxial_GUN:

		MGPIO_voidSetPinValue(PORTB,PIN3,LOW);   //Sabot
		MGPIO_voidSetPinValue(PORTB,PIN9,HIGH);   //Coaxial gun
		MGPIO_voidSetPinValue(PORTC,PIN13,LOW);   //heat
		MGPIO_voidSetPinValue(PORTC,PIN14,LOW);   //hep
		MGPIO_voidSetPinValue(PORTC,PIN15,LOW);   //high expo.
		break;

	case HEP:
		MGPIO_voidSetPinValue(PORTB,PIN3,LOW); //Sabot
		MGPIO_voidSetPinValue(PORTB,PIN9,LOW); //Coaxial gun
		MGPIO_voidSetPinValue(PORTC,PIN13,LOW); //heat
		MGPIO_voidSetPinValue(PORTC,PIN14,HIGH); //hep
		MGPIO_voidSetPinValue(PORTC,PIN15,LOW); //high expo.
		break;

	case SABOT:
		MGPIO_voidSetPinValue(PORTB,PIN3,HIGH);  //Sabot
		MGPIO_voidSetPinValue(PORTB,PIN9,LOW);  //Coaxial gun
		MGPIO_voidSetPinValue(PORTC,PIN13,LOW);  //heat
		MGPIO_voidSetPinValue(PORTC,PIN14,LOW);  //hep
		MGPIO_voidSetPinValue(PORTC,PIN15,LOW);  //high expo.
		break;

	case HEAT:

		MGPIO_voidSetPinValue(PORTB,PIN3,LOW);   //Sabot
		MGPIO_voidSetPinValue(PORTB,PIN9,LOW);   //Coaxial gun
		MGPIO_voidSetPinValue(PORTC,PIN13,HIGH);   //heat
		MGPIO_voidSetPinValue(PORTC,PIN14,LOW);   //hep
		MGPIO_voidSetPinValue(PORTC,PIN15,LOW);   //high expo.
		break;

	case HIGH_EXP :
		MGPIO_voidSetPinValue(PORTB,PIN3,LOW);   //Sabot
		MGPIO_voidSetPinValue(PORTB,PIN9,LOW);   //Coaxial gun
		MGPIO_voidSetPinValue(PORTC,PIN13,LOW);   //heat
		MGPIO_voidSetPinValue(PORTC,PIN14,LOW);   //hep
		MGPIO_voidSetPinValue(PORTC,PIN15,HIGH);   //high expo.
		break;
	}
}

// An array that stores the elements of the struct
PinAction actions[] = {
		{PIN0,NULL,NULL},
		{BRIGHT_PLUS_PIN,     "*Bright+#\n",        NULL},
		{PIN2,NULL,NULL},
		{PIN3,NULL,NULL},
		{AUTO_PIN,              NULL,               NULL},
		{EXPOSURE_MINUS_PIN,  "*Exposure-#\n",      NULL},
		{BRIGHT_MINUS_PIN,    "*Bright-#\n",        NULL},
		{CONTRAST_MINUS_PIN,  "*Contrast-#\n",      NULL},
		{CONTRAST_PLUS_PIN,   "*Contrast+#\n",      NULL},
		{PIN9,NULL,NULL},
		{ROUND_IN_BARREL_PIN, "*Round In Barrel#\n",NULL},
		{FIELD_RANGE_PIN,     "*Field Range#\n",    NULL},
		{LAMP_TEST_PIN,         NULL ,      handleLampTest},
		{COLOR_PIN,           "*Color#\n",          NULL},
		{FOCUS_PLUS_PIN,       "*Focus+#\n",        NULL},
		{FOCUS_MINUS_PIN,      "*Focus-#\n",         NULL}
};

void executeActions(void)
{
/*
	for (u8 i = 0;i<sizeof(actions)/sizeof(actions[0]); i++)
	{
		if( MGPIO_u8GetPinValue(PORTB, actions[i].pin) == 1)
		{
			if(i==14)
			{
				if(Global_u8Day_Thermal_Flag==0)
				{
					USART1_VoidWriteString((u8 *)"*Focus+#");
					delay_voidXms(1000);
				}
				else
				{

				}
			}
			else if(i==15)
			{
				if(Global_u8Day_Thermal_Flag==0) // thermal status
				{
					USART1_VoidWriteString((u8 *)"*Focus-#");
					delay_voidXms(1000);
				}
				else
				{

				}
			}

			else if(i==4)
			{

				USART1_VoidWriteString((u8 *)"*Auto#");
				delay_voidXms(1000);
			}
			else if(i==12)
			{
				Global_u8Lamp_test_Status_Flag=0;
			}
			else if(i==0 || i==1 || i==2 || i==3 ||i==8 || i==9)
			{
				continue;
			}
			else
			{
				executeAction(actions[i]);
				delay_voidXms(1000);
				//while (MGPIO_u8GetPinValue(PORTB, actions[i].pin)); // For debouncing
			}

		}
		else if(MGPIO_u8GetPinValue(PORTB, 12) == 1 && Global_u8Lamp_test_Status_Flag==0)// Remain the recent ON LEDs... Lamp test button is pressed
		{
			Global_u8Lamp_test_Status_Flag=1;
			handleLampTest();
		}
		else if(MGPIO_u8GetPinValue(PORTB, 12) == 0 && Global_u8Lamp_test_Status_Flag==1)
		{
			Global_u8Lamp_test_Status_Flag=0;
			resetLamps();
			//Restart the past ON LEDs again
			switch(Current_LED_Echo)
			{
			case First_Echo:Control_Panelvoid_Message_For_LED(FIRST_ECHO);break;
			case Last_Echo: Control_Panelvoid_Message_For_LED(LAST_ECHO);break;
			case Off: Control_Panelvoid_Message_For_LED(ECHO_OFF);break;
			default: break;

			}
			switch(Current_LED_Bullet)
			{
			case Coaxial: Control_Panelvoid_Message_For_LED(Coaxial_GUN);break;
			case Hep: Control_Panelvoid_Message_For_LED(HEP);break;
			case Sabot: Control_Panelvoid_Message_For_LED(SABOT);break;
			case Heat: Control_Panelvoid_Message_For_LED(HEAT);break;
			case High_Exp: Control_Panelvoid_Message_For_LED(HIGH_EXP);break;
			default: break;
			}
		}
		USART1_VoidWriteString("SS");
			resetLamps(); // ShutDown the LEDs if Lamp test button is pressed
			Global_u8Lamp_test_Status_Flag=1;
			 Restart the past ON LEDs again
			switch(Current_LED_Echo)
			{
			case First_Echo:Control_Panelvoid_Message_For_LED(FIRST_ECHO);break;
			case Last_Echo: Control_Panelvoid_Message_For_LED(LAST_ECHO);break;
			case Off: Control_Panelvoid_Message_For_LED(ECHO_OFF);break;
			default: break;

			}
			switch(Current_LED_Bullet)
			{
			case Coaxial: Control_Panelvoid_Message_For_LED(Coaxial_GUN);break;
			case Hep: Control_Panelvoid_Message_For_LED(HEP);break;
			case Sabot: Control_Panelvoid_Message_For_LED(SABOT);break;
			case Heat: Control_Panelvoid_Message_For_LED(HEAT);break;
			case High_Exp: Control_Panelvoid_Message_For_LED(HIGH_EXP);break;
			default: break;
			}

			delay_voidXms(1000);

		}
		else
		{

		}


	}
	if(MGPIO_u8GetPinValue(PORTA, 6) == 0 && MGPIO_u8GetPinValue(PORTA, 7) == 0 && Global_u8Echo_Status_Flag==1 && Global_u8EchoState==FECHO_FLAG)
	{
		Global_u8EchoState=LECHO_FLAG;
		Global_u8Echo_Status_Flag=0;
		Control_Panelvoid_Message_For_LED(FIRST_ECHO);
		USART1_VoidWriteString((u8 *)"*First Echo#");
		Current_LED_Echo=First_Echo;
	}*/
	if(MGPIO_u8GetPinValue(PORTB,0) == 0 && MGPIO_u8GetPinValue(PORTA,5) == 0 && Global_u8FV_Status_Flag==1 && Global_u8Day_Thermal_Flag==0 && Global_u8FVState==MFV_FLAG)
	{
		Global_u8FV_Status_Flag=0;
		Global_u8FVState=NFV_FLAG;
		USART1_VoidWriteString((u8 *)"*MFOW#");
	}


	if(MGPIO_u8GetPinValue(PORTA,8) == 1)
	{
		USART1_VoidWriteString((u8 *)"*Contrast+#");
		delay_voidXms(1000);
	}
	else if(MGPIO_u8GetPinValue(PORTB,1) == 1)
	{
		USART1_VoidWriteString((u8 *)"*Bright+#");

		delay_voidXms(1000);
	}
	else if(MGPIO_u8GetPinValue(PORTA,0) == 1)
	{

		USART1_VoidWriteString((u8 *)"*Exposure+#");

		delay_voidXms(1000);
	}
	else if(MGPIO_u8GetPinValue(PORTA,15) == 1)
	{
		USART1_VoidWriteString((u8 *)"*Full Screen#");

		delay_voidXms(1000);
	}


}

/*
 * Breif : Based on the array of struct index, it access the message element and send it,and
 * execute the corresponding function.
 * Parameters :Takes the array of struct.
 * return : Nothing
 */
void executeAction(PinAction action)
{
	USART1_VoidWriteString((u8 *)action.message);
	if (action.action != NULL)
	{
		action.action();
	}
}


void handleLampTest(void){
	MGPIO_voidSetPinValue(PORTB,PIN3,HIGH);   //Sabot
	MGPIO_voidSetPinValue(PORTB,PIN9,HIGH);   //Coaxial gun
	MGPIO_voidSetPinValue(PORTC,PIN13,HIGH);   //heat
	MGPIO_voidSetPinValue(PORTC,PIN14,HIGH);   //hep
	MGPIO_voidSetPinValue(PORTC,PIN15,HIGH);   //high expo.
	MGPIO_voidSetPinValue(PORTA,PIN12,HIGH);   //first echo
	MGPIO_voidSetPinValue(PORTB,PIN8,HIGH);   //last echo
}


void resetLamps(void)
{
	MGPIO_voidSetPinValue(PORTB,PIN3,LOW);   //Sabot
	MGPIO_voidSetPinValue(PORTB,PIN9,LOW);   //Coaxial gun
	MGPIO_voidSetPinValue(PORTC,PIN13,LOW);   //heat
	MGPIO_voidSetPinValue(PORTC,PIN14,LOW);   //hep
	MGPIO_voidSetPinValue(PORTC,PIN15,LOW);   //high expo.
	MGPIO_voidSetPinValue(PORTA,PIN12,LOW);   //first echo
	MGPIO_voidSetPinValue(PORTB,PIN8,LOW);   //last echo
}



void Control_Panel_voidStartUpLeds(void)
{

	u8 Data=ShiftRegister_u8GetData();
	for(u8 i=4;i<9;i++)
	{
		if(GET_BIT(Data,i)==1)
		{
			switch(i)
			{
			case 8: Control_Panelvoid_Message_For_LED(Coaxial_GUN);Current_LED_Bullet=Coaxial;Global_u8BulletState=COAXIAL_GUN_FLAG;break;
			case 7: Control_Panelvoid_Message_For_LED(HEP);Current_LED_Bullet=Hep;Global_u8Bullets_Flag=0;Global_u8BulletState=HEP_FLAG;break;
			case 6:	Control_Panelvoid_Message_For_LED(SABOT);Current_LED_Bullet=Sabot;Global_u8BulletState=SABOT_FLAG;break;
			case 5: Control_Panelvoid_Message_For_LED(HEAT); Current_LED_Bullet=Heat;Global_u8BulletState=HEAT_FLAG; break;
			case 4: Control_Panelvoid_Message_For_LED(HIGH_EXP);Current_LED_Bullet=High_Exp;Global_u8BulletState=HIGH_EXPO_FLAG;break;
			}

		}
	}

/*
	 Loop to Check the last status of Switches before startup
	for (u8 i = 5; i <=7; i++)
	{
		if (MGPIO_u8GetPinValue(PORTA, i) == 1) //Check if the Switch is selected
		{
			switch (i)
			{
			case 5: USART1_VoidWriteString((u8 *)"*WFOV#"); Global_u8FV_Status_Flag=1;Global_u8FVState=MFV_FLAG; break;
		//	case 6: USART1_VoidWriteString((u8 *)"*Last Echo#");	Control_Panelvoid_Message_For_LED(LAST_ECHO);Global_u8EchoState=FECHO_FLAG;	Current_LED_Echo=Last_Echo;Global_u8Echo_Status_Flag=1;break;
			//case 7:	USART1_VoidWriteString((u8 *)"*Echo Off#");Control_Panelvoid_Message_For_LED(ECHO_OFF);Global_u8EchoState=FECHO_FLAG;Current_LED_Echo=Off;break;

			default:break;
			}
		}
	}

//	if(MGPIO_u8GetPinValue(PORTA,PIN1)==0 && MGPIO_u8GetPinValue(PORTA,PIN2)==0 && MGPIO_u8GetPinValue(PORTA,PIN3)==0 && MGPIO_u8GetPinValue(PORTA,PIN4)==0)
//	{
//		Global_u8Bullets_Flag=1;
//		Control_Panelvoid_Message_For_LED(Coaxial_GUN);Current_LED_Bullet=Coaxial;Global_u8BulletState=COAXIAL_GUN_FLAG;
//	}
	if(MGPIO_u8GetPinValue(PORTB,0) == 0 && MGPIO_u8GetPinValue(PORTA,5) == 0)
	{
		Global_u8FVState=WFV_FLAG;
		USART1_VoidWriteString((u8 *)"*MFOV#"); Global_u8FV_Status_Flag=1;
	}
	if(MGPIO_u8GetPinValue(PORTA,PIN11) == 1)
	{
		USART1_VoidWriteString((u8 *)"*Thermal#");Global_u8Day_Thermal_Flag=0;Global_u8DTStateFlag=0;
	}
	if(MGPIO_u8GetPinValue(PORTA, PIN11) == 0) // if the switch is open it will read Thermal as its single position switch
	{
		USART1_VoidWriteString((u8 *)"*Day#");Global_u8Day_Thermal_Flag=1;Global_u8DTStateFlag=1;
	}
	if(MGPIO_u8GetPinValue(PORTA, PIN6) == 0 && MGPIO_u8GetPinValue(PORTA, 7) == 0) // If PIN6 and PIN7 are not pressed Echo off will be selected
	{
		USART1_VoidWriteString((u8 *)"*First Echo#");	Control_Panelvoid_Message_For_LED(FIRST_ECHO);Global_u8EchoState=LECHO_FLAG;	Current_LED_Echo=First_Echo;Global_u8Echo_Status_Flag=1;
	}
	if(MGPIO_u8GetPinValue(PORTB, PIN0) == 1)
	{
		Global_u8FVState=MFV_FLAG;
		Global_u8FV_Status_Flag=1;
		USART1_VoidWriteString((u8 *)"*NFOW#\n");

	}*/
}


u8 ShiftRegister_u8GetData(void)
{
    u8 Data = 0;

    // Load parallel data into the shift register
    MGPIO_voidSetPinValue(PORTA, SH_LD_PIN, LOW);
    delay_voidXms(1);  // Use minimal required delay
    MGPIO_voidSetPinValue(PORTA, SH_LD_PIN, HIGH);

    // Read serial data
    for (u8 i = 0; i < 8; i++)
    {
        MGPIO_voidSetPinValue(PORTA, CLK_PIN, LOW);  // Ensure stable clock
        delay_voidXms(10);
        MGPIO_voidSetPinValue(PORTA, CLK_PIN, HIGH); // Pulse clock

        Data <<= 1;
        Data |= MGPIO_u8GetPinValue(PORTA, QH_PIN);
    }

    MGPIO_voidSetPinValue(PORTA, CLK_PIN, LOW); // Ensure clock is LOW after operation

    return Data;
}


void DataEntry_VoidWriteStringAndNumbers(u8 *Copy_u8StringToBeSent,u32 Copy_u32RealNumber)
{


	u8 ReminderOne = 0;
	u32 Temp = Copy_u32RealNumber,i=0;


	while (Copy_u8StringToBeSent[i]){
		USART1_VoidWriteCharacter(Copy_u8StringToBeSent[i]);
		i++;
	}
	/*if (Copy_u32RealNumber == 0) {
		USART1_VoidWriteCharacter('0');
		return; // Early return to avoid further execution
	}*/


	u8 ArrIntegerValues[2]={0};

	Temp = Copy_u32RealNumber; // Reset Temp to TotalNumber

	// Store digits in reverse order
	for (u8 s = 0; s <2; s++)
	{
		ReminderOne = Temp % 10;
		ArrIntegerValues[s] = ReminderOne;
		Temp = Temp / 10;
	}

	// Send digits in correct order
	for (s8 s = 2 - 1; s >= 0; s--)
	{
		USART1_VoidWriteCharacter(ArrIntegerValues[s] + '0'); // Adding '0' converts digit to ASCII
	}


}
