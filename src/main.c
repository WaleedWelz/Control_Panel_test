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
#include "AFIO_private.h"

/* Functions used in Interrupts call back */
void INT_NFOV(void);
//void INT_HEP_COAXIAL_GUN(void);
void INT_DAY_THERMAL(void);
void INT_First_Echo_LAST_ECHO(void);
/*void INT_SABOT(void);
void INT_HEAT(void);
void INT_HIGH_EXPO(void);*/



u8 Global_u8DTStateFlag=0;
u8 Global_u8FVState=FV_NO_STATE;
u8 Global_u8EchoState=ECHO_NO_STATE;
u8 Global_u8BulletState=BULLET_NO_STATE;
u8 Global_u8CoaxialReturn=COAXIAL_NO_STATE;
u8 Global_u8Day_Thermal_Flag=0; // Flag to know the switch is on DAY or THERMAL mode
u8 Global_u8Lamp_test_Status_Flag=0; // Flag to know if the Lamp Test button is pressed
u8 Global_u8Echo_Status_Flag=0; // Flag used to detect ECHO_OFF status when the two Echo pins are Unpressed
u8 Global_u8FV_Status_Flag=0; // Flag used to detect NFOV status when the two FOV pins are Unpressed
u8 Global_u8Bullets_Flag=0;
u8 Global_u8WideFlag=0;
u8 Global_u8StartCounter=0; // Counter to detect the correct start message
u8 Global_u8Sabot2=0;
u8 HEP_PINStatus=0;
u8 *Global_u8StartCommand=NULL; // Pointer for the returned string from the UART function
u8 Global_u8CounterForReentrant=0;
u8 Coaxial_FlagIndicator=0;
u8 cnt=0;
u8 Global_u8SwitchsData=0;
u8 Global_u8PressedSwitch=0;

enum Status Current_LED_Echo; // Enum Object to Store the Echo status used in Lightening the corresponding LED at startup
enum Status Current_LED_Bullet;// Enum Object to Store the Bullet status used in Lightening the corresponding LED at startup

void main()
{
	MRCC_voidInitSysClock(); // Adjust clock settings

	MRCC_voidEnableClock(RCC_APB2,GPIO_PORTA_BUS); // Activate PORTA bus
	MRCC_voidEnableClock(RCC_APB2,GPIO_PORTB_BUS); // Activate PORTB bus
	MRCC_voidEnableClock(RCC_APB2,GPIO_PORTC_BUS); // Activate PORTC bus

	MRCC_voidEnableClock(RCC_APB2,AFIO_BUS); // Activate AFIO bus
	MAFIO->MAPR |= (1 << 25);

	Sys_VoidInit(TimerMode); //  Enabling Timer
	USART_Init(); // Initializing UART


	Global_u8StartCommand=USART1_u8ReadString(); // Pointing to the received string



	Control_Panelvoid_Init(); // Initializing each pin's Mode and interrupts levels


	u8 arr_start[]="Start"; // The received Startup message


	/*
	MEXTI_voidEnable(EXTI_PORTA,PIN1,RISING_EDGE); //
	MEXTI_voidEnable(EXTI_PORTA,PIN2,RISING_EDGE);
	MEXTI_voidEnable(EXTI_PORTA,PIN3,RISING_EDGE);
	MEXTI_voidEnable(EXTI_PORTA,PIN4,RISING_EDGE); //
	 */
	MEXTI_voidEnable(EXTI_PORTA,PIN6,RISING_EDGE); //Echo interrupt
	MEXTI_voidEnable(EXTI_PORTA,PIN7,RISING_EDGE);//Echo intrrupt
	MEXTI_voidEnable(EXTI_PORTB,PIN0,RISING_EDGE);//FOV interrupt
	MEXTI_voidEnable(EXTI_PORTA,PIN11,EDGE_CHANGE);//FOV interrupt





	/*
	MEXTINT1_VidSetCallBack(&INT_HEAT);
	MEXTINT2_VidSetCallBack(&INT_SABOT);
	MEXTINT3_VidSetCallBack(&INT_HEP_COAXIAL_GUN);
	MEXTINT4_VidSetCallBack(&INT_HIGH_EXPO);
	 */
	MEXTINT0_VidSetCallBack(&INT_NFOV);
	MEXTINT9_5_VidSetCallBack(&INT_First_Echo_LAST_ECHO);
	MEXTINT15_10_VidSetCallBack(&INT_DAY_THERMAL);


	//Loop to check the startup message
	for(u8 i=0;Global_u8StartCommand[i]!='\0';i++)
	{
		if(Global_u8StartCommand[i]==arr_start[i])
		{
			Global_u8StartCounter++;
		}
		else
		{
			Global_u8StartCounter=0;
		}
	}


	if(Global_u8StartCounter==STARTUP_MESSAGE_LETTERS_NUMBER)
	{
		Control_Panel_voidStartUpLeds(); // Know which Switch position was selected and Light the corresponding LED

		while(1)
		{



			executeActions(); // Check which BUTTON is pressed and handle the Lamp test button states,checks Echo_OFF and NFOV states.

			if( MGPIO_u8GetPinValue(PORTA,PIN5)==1 && Global_u8WideFlag == 0) {
				if(Global_u8Day_Thermal_Flag==0)
				{
					if(Global_u8FVState==WFV_FLAG)
					{
						Global_u8FVState=MFV_FLAG;
						Global_u8WideFlag=1;
						Global_u8FV_Status_Flag=1;
						USART1_VoidWriteString((u8 *)"*WFOV#");
					}
				}
				else if(Global_u8Day_Thermal_Flag==1)
				{
					if(Global_u8FVState==MFV_FLAG)
					{
						Global_u8FVState=WFV_FLAG;
					}
					Global_u8FVState=NFV_FLAG;
					Global_u8WideFlag=1;
					Global_u8FV_Status_Flag=1;
					USART1_VoidWriteString((u8 *)"*WFOV#");

				}



			}
			else if ( MGPIO_u8GetPinValue(PORTA,PIN5)==0 && Global_u8WideFlag == 1 ) {

				Global_u8WideFlag=0;
			}

			Global_u8SwitchsData=ShiftRegister_u8GetData();

			if(GET_BIT(Global_u8SwitchsData,1)==1  && GET_BIT(Global_u8SwitchsData,2)==0  && GET_BIT(Global_u8SwitchsData,3)==0  && GET_BIT(Global_u8SwitchsData,4)==0  && GET_BIT(Global_u8SwitchsData,5)==0 && Global_u8BulletState==HIGH_EXPO_FLAG)
			{
				Global_u8BulletState=HEAT_FLAG;
				USART1_VoidWriteString((u8 *)"*High_Exp.#\n");
				Current_LED_Bullet=High_Exp;
				Control_Panelvoid_Message_For_LED(HIGH_EXP);
				Global_u8Bullets_Flag=0;
			}
			if(GET_BIT(Global_u8SwitchsData,1)==0  && GET_BIT(Global_u8SwitchsData,2)==1  && GET_BIT(Global_u8SwitchsData,3)==0  && GET_BIT(Global_u8SwitchsData,4)==0  && GET_BIT(Global_u8SwitchsData,5)==0 && Global_u8BulletState==HEAT_FLAG )
			{
				Global_u8BulletState=SABOT_FLAG;
				USART1_VoidWriteString((u8 *)"*Heat#\n");
				Current_LED_Bullet=Heat;
				Control_Panelvoid_Message_For_LED(HEAT);
				Global_u8Bullets_Flag=0;
				cnt=0;
			}
			if(GET_BIT(Global_u8SwitchsData,1)==0  && GET_BIT(Global_u8SwitchsData,2)==0  && GET_BIT(Global_u8SwitchsData,3)==1  && GET_BIT(Global_u8SwitchsData,4)==0  && GET_BIT(Global_u8SwitchsData,5)==0 && Global_u8BulletState==SABOT_FLAG)
			{
				Global_u8BulletState=HEAT_FLAG;
				USART1_VoidWriteString((u8 *)"*Sabot#\n");
				Current_LED_Bullet=Sabot;
				Control_Panelvoid_Message_For_LED(SABOT);
				Global_u8Bullets_Flag=0;
			}
			if(GET_BIT(Global_u8SwitchsData,1)==0  && GET_BIT(Global_u8SwitchsData,2)==0  && GET_BIT(Global_u8SwitchsData,3)==0  && GET_BIT(Global_u8SwitchsData,4)==1  && GET_BIT(Global_u8SwitchsData,5)==0 && Global_u8BulletState==HEP_FLAG)
			{
				Global_u8BulletState=COAXIAL_GUN_FLAG;
				USART1_VoidWriteString((u8 *)"*HEP#");
				Global_u8Bullets_Flag=0;
				Current_LED_Bullet=Hep;
				Control_Panelvoid_Message_For_LED(HEP);
			}
			if(GET_BIT(Global_u8SwitchsData,1)==0  && GET_BIT(Global_u8SwitchsData,2)==0  && GET_BIT(Global_u8SwitchsData,3)==0  && GET_BIT(Global_u8SwitchsData,4)==0  && GET_BIT(Global_u8SwitchsData,5)==1 && Global_u8BulletState==COAXIAL_GUN_FLAG)
			{
				Global_u8BulletState=HEP_FLAG;
				Global_u8Bullets_Flag=1;
				USART1_VoidWriteString((u8 *)"*Coaxial Gun#");
				Current_LED_Bullet=Coaxial;
				Control_Panelvoid_Message_For_LED(Coaxial_GUN);
			}


			//MGPIO_voidSetPinValue(PORTB,PIN9,HIGH);   //Coaxial gun


		}
	}

	else
	{
		USART1_VoidWriteString((u8*)"Wrong Input..Reset");
	}
}


void INT_DAY_THERMAL(void){



	u8 Local_u8Pin1_value=MGPIO_u8GetPinValue(PORTA,PIN11);


	if(Local_u8Pin1_value==1 && Global_u8DTStateFlag==1)
	{
		USART1_VoidWriteString((u8 *)"*Thermal#");
		delay_voidXms(10);
		Global_u8Day_Thermal_Flag=0;
		Global_u8DTStateFlag=0;
	}
	else if(Local_u8Pin1_value==0 && Global_u8DTStateFlag==0)
	{
		USART1_VoidWriteString((u8 *)"*Day#");
		delay_voidXms(10);
		Global_u8Day_Thermal_Flag=1;
		Global_u8DTStateFlag=1;

	}
	//delay_voidXms(10);
	MEXTI_voidClearPendingFlag(11);

}


void INT_NFOV(void)
{
	if(Global_u8Day_Thermal_Flag==0)
	{
		if(Global_u8FVState==NFV_FLAG)
		{
			Global_u8FVState=MFV_FLAG;
			Global_u8FV_Status_Flag=1;
			USART1_VoidWriteString((u8 *)"*NFOV#");

		}


	}
	else if(Global_u8Day_Thermal_Flag==1)
	{
		if(Global_u8FVState==MFV_FLAG)
		{
			Global_u8FVState=WFV_FLAG;
		}
		if(Global_u8FVState==WFV_FLAG)
		{
			Global_u8FVState=NFV_FLAG;
			Global_u8FV_Status_Flag=1;
			USART1_VoidWriteString((u8 *)"*NFOV#");

		}

	}
	MEXTI_voidClearPendingFlag(0);
}


void INT_First_Echo_LAST_ECHO(void)
{
	MEXTI_voidDisable(PIN6);
	MEXTI_voidDisable(PIN7);

	u8 Local_u8Pin6_value=MGPIO_u8GetPinValue(PORTA,PIN6);
	u8 Local_u8Pin7_value=MGPIO_u8GetPinValue(PORTA,PIN7);

	if(Local_u8Pin6_value==1 && Global_u8EchoState==LECHO_FLAG)
	{
		Global_u8EchoState=FECHO_FLAG;
		Global_u8Echo_Status_Flag=1;
		Control_Panelvoid_Message_For_LED(LAST_ECHO);
		Current_LED_Echo=Last_Echo;
		USART1_VoidWriteString((u8 *)"*Last Echo#");
	}
	else if(Local_u8Pin7_value==1 && Global_u8Echo_Status_Flag==0 && Global_u8EchoState==OFF_FLAG)
	{
		Global_u8EchoState=FECHO_FLAG;
		Global_u8Echo_Status_Flag=1;
		Control_Panelvoid_Message_For_LED(ECHO_OFF);
		Current_LED_Echo=Off;
		USART1_VoidWriteString((u8 *)"*Echo Off#");
	}

	MEXTI_voidClearPendingFlag(6);
	MEXTI_voidClearPendingFlag(7);

	MEXTI_voidEnable(PORTA,PIN6,RISING_EDGE);
	MEXTI_voidEnable(PORTA,PIN7,RISING_EDGE);

}

/*


void INT_SABOT(void)
{
	Global_u8BulletState=SABOT_FLAG;
	Global_u8Bullets_Flag=0;
	MEXTI_voidClearPendingFlag(2);
}




void INT_HEAT(void)
{
	Global_u8BulletState=HEAT_FLAG;
	Global_u8Bullets_Flag=0;
	MEXTI_voidClearPendingFlag(1);
}





void INT_HIGH_EXPO(void)
{
	Global_u8BulletState=HIGH_EXPO_FLAG;
	Global_u8Bullets_Flag=0;
	MEXTI_voidClearPendingFlag(4);
}

void INT_HEP_COAXIAL_GUN(void)
{
	Global_u8BulletState=HEP_FLAG;
	Global_u8Bullets_Flag=0;
	MEXTI_voidClearPendingFlag(3);
}

 */
