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

#include "SYS_private.h"
#include "SYS_config.h"

#include "Timer2_StopWatch.h"
#include "Timer3_StopWatch.h"
#include "Reset.h"

/* Functions used in Interrupts call back */
void INT_NFOV(void);
//void INT_HEP_COAXIAL_GUN(void);
void INT_DAY_THERMAL(void);
void INT_First_Echo_LAST_ECHO(void);
/*void INT_SABOT(void);
void INT_HEAT(void);
void INT_HIGH_EXPO(void);*/
void CheckSwitchState();



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
u8 Global_u8ResetCounter=0;
u8 Global_u8Sabot2=0;
u8 HEP_PINStatus=0;
u8 Global_u8StartCommand=0; // Pointer for the returned string from the UART function
u8 Global_u8CounterForReentrant=0;
u8 Coaxial_FlagIndicator=0;
u8 cnt=0;
u16 Global_u16SwitchsData=0;
u8 Global_u8PressedSwitch=0;
u8 Global_u8SentenceIsTaken=0;
u8 Global_u8StartThePanel=0;
u8 Global_u8BulletsOn=0;
u8 Global_u8BulletCounter=0;
u8 Global_u8Counter=0;
u8 Global_u8F_T=0;
u8 Global_u8BulletsEnteredOnce=0;

enum Status Current_LED_Echo; // Enum Object to Store the Echo status used in Lightening the corresponding LED at startup
enum Status Current_LED_FOV;
enum Status Current_LED_Bullet;// Enum Object to Store the Bullet status used in Lightening the corresponding LED at startup
enum Status LastBullet=	NOM;
u8 Global_TimerEnteredOnce=0;
u8 Global_Timer3EnteredOnce=0;
u8 Executed_Flag = 0;  // Flag to track execution
u32 current_time=0;
u32 current_time3=0;
extern Global_u8FirstBullet;

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
	TIM2_voidInit();
	TIM3_Init();





	Control_Panelvoid_Init(); // Initializing each pin's Mode and interrupts levels


	/*
	u8 arr_start[]="S"; // The received Startup message
	u8 arr_Reset[]="R";

	 */

	/*
	MEXTI_voidEnable(EXTI_PORTA,PIN1,RISING_EDGE); //
	MEXTI_voidEnable(EXTI_PORTA,PIN2,RISING_EDGE);
	MEXTI_voidEnable(EXTI_PORTA,PIN3,RISING_EDGE);
	MEXTI_voidEnable(EXTI_PORTA,PIN4,RISING_EDGE); //
	 */
	MEXTI_voidEnable(EXTI_PORTA,PIN6,RISING_EDGE); //Echo interrupt
	MEXTI_voidEnable(EXTI_PORTA,PIN7,RISING_EDGE);//Echo intrrupt
	MEXTI_voidEnable(EXTI_PORTB,PIN0,RISING_EDGE);//FOV interrupt
	MEXTI_voidEnable(EXTI_PORTA,PIN15,EDGE_CHANGE);//D/T interrupt





	/*
	MEXTINT1_VidSetCallBack(&INT_HEAT);
	MEXTINT2_VidSetCallBack(&INT_SABOT);
	MEXTINT3_VidSetCallBack(&INT_HEP_COAXIAL_GUN);
	MEXTINT4_VidSetCallBack(&INT_HIGH_EXPO);
	 */
	MEXTINT0_VidSetCallBack(&INT_NFOV);
	MEXTINT9_5_VidSetCallBack(&INT_First_Echo_LAST_ECHO);
	MEXTINT15_10_VidSetCallBack(&INT_DAY_THERMAL);


	while(1)
	{


		Global_u8StartCommand=USART1_u8ReadChar_NonBlocking(); // Pointing to the received string
		/*

		if (Global_u8StartCommand[0] != NULL) // Ensure a new string is received
		{
			Global_u8StartCounter = 0; // Reset counter before checking the new input

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
		}

		 */

		if (Global_u8StartCommand!= 0) // Ensure a new string is received
		{

			if(Global_u8StartCommand== 'R')
			{
				software_reset();
			}
		}



		if (Global_u8StartCommand!= 0) // Ensure a new string is received
		{
			if(Global_u8StartCommand =='S')
			{
				USART1_VoidWriteString((u8*)"*S,");

				Control_Panel_voidStartUpLeds(); // Know which Switch position was selected and Light the corresponding LED

				Global_u8StartThePanel=1;
				Global_u8StartCounter = 0;

			}
		}

		/*	if(MGPIO_u8GetPinValue(PORTB,8) == 1)
		{
			delay_voidXms(500);

			USART1_VoidWriteString((u8*)"*S,");

			Control_Panel_voidStartUpLeds(); // Know which Switch position was selected and Light the corresponding LED

			Global_u8StartThePanel=1;
			Global_u8StartCounter = 0;

			delay_voidXms(500);
		}
		 */

		if(Global_u8StartThePanel==1)
		{

			if(MGPIO_u8GetPinValue(PORTB,PIN14)==1)
			{
				if(Global_u8Day_Thermal_Flag==0)
				{
					USART1_VoidWriteString((u8 *)"*F+#");
					delay_voidXms(500);
				}
				else
				{
				}
			}

			if(MGPIO_u8GetPinValue(PORTC,PIN13)==0)
			{

				USART1_VoidWriteString((u8 *)"*E-#");
				delay_voidXms(500);
			}
			if(MGPIO_u8GetPinValue(PORTB,PIN12)==1)
			{
				Global_u8Lamp_test_Status_Flag=0;
			}
			if(MGPIO_u8GetPinValue(PORTB,PIN4)==1)
			{
				USART1_VoidWriteString((u8 *)"*A#");
				delay_voidXms(500);
			}
			if(MGPIO_u8GetPinValue(PORTB,8) == 1)
			{
				USART1_VoidWriteString((u8*)"*E+#");
				delay_voidXms(500);
			}
			if(MGPIO_u8GetPinValue(PORTB,11) == 1)
			{
				USART1_VoidWriteString((u8 *)"*C+#");
				delay_voidXms(500);
			}
			if(MGPIO_u8GetPinValue(PORTB,7) == 1)
			{
				USART1_VoidWriteString((u8 *)"*C-#");
				delay_voidXms(500);
			}

			if(MGPIO_u8GetPinValue(PORTB,13) == 1)
			{
				USART1_VoidWriteString((u8 *)"*TAF#");

				delay_voidXms(500);
			}
			if(MGPIO_u8GetPinValue(PORTA,8) == 1)
			{
				USART1_VoidWriteString((u8 *)"*BTLRNG#");

				delay_voidXms(500);
			}
			if(MGPIO_u8GetPinValue(PORTA,11) == 1)
			{
				USART1_VoidWriteString((u8 *)"*B+#");

				delay_voidXms(500);
			}
			if(MGPIO_u8GetPinValue(PORTB,6) == 1)
			{
				USART1_VoidWriteString((u8 *)"*B-#");

				delay_voidXms(500);
			}
			if(MGPIO_u8GetPinValue(PORTB,1) == 1)
			{
				USART1_VoidWriteString((u8 *)"*CLR#");
				delay_voidXms(500);
			}

			if(MGPIO_u8GetPinValue(PORTB, 12) == 1 && Global_u8Lamp_test_Status_Flag==0)// Remain the recent ON LEDs... Lamp test button is pressed
			{
				Global_u8Lamp_test_Status_Flag=1;
				handleLampTest();
			}
			else if(MGPIO_u8GetPinValue(PORTB, 12) == 0 && Global_u8Lamp_test_Status_Flag==1)
			{
				Global_u8Lamp_test_Status_Flag=0;
				resetLamps();
				//Restart the past ON LEDs again
				switch(Current_LED_FOV)
				{
				case Nfov:Control_Panelvoid_Message_For_LED(NFOV);break;
				case Mfov:Control_Panelvoid_Message_For_LED(MFOV);break;
				case Wfov:Control_Panelvoid_Message_For_LED(WFOV);break;

				}
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


			if(MGPIO_u8GetPinValue(PORTA, 6) == 0 && MGPIO_u8GetPinValue(PORTA, 7) == 0 && Global_u8Echo_Status_Flag==1 && Global_u8EchoState==FECHO_FLAG)
			{
				Global_u8EchoState=LECHO_FLAG;
				Global_u8Echo_Status_Flag=0;
				Control_Panelvoid_Message_For_LED(FIRST_ECHO);
				USART1_VoidWriteString((u8 *)"*FE#");
				Current_LED_Echo=First_Echo;
			}
			if(MGPIO_u8GetPinValue(PORTB,0) == 0 && MGPIO_u8GetPinValue(PORTA,5) == 0 && Global_u8FVState==MFV_FLAG)
			{
				Global_u8FV_Status_Flag=0;
				Control_Panelvoid_Message_For_LED(MFOV);
				Current_LED_FOV=Mfov;
				Global_u8FVState=NFV_FLAG;
				USART1_VoidWriteString((u8 *)"*N#");

			}


			if(Global_u8FVState==MFV_FLAG)
			{
				Global_u8WideFlag=0;
			}

			if( MGPIO_u8GetPinValue(PORTA,PIN5)==1 && Global_u8WideFlag == 0)
			{

				if(Global_u8FVState==WFV_FLAG)
				{
					Global_u8FV_Status_Flag=1;

					Control_Panelvoid_Message_For_LED(WFOV);
					Current_LED_FOV=Wfov;
					Global_u8FVState=MFV_FLAG;
					USART1_VoidWriteString((u8 *)"*W#");
					current_time3=0;
					Global_u8WideFlag=1;
				}
			}

			else if ( MGPIO_u8GetPinValue(PORTA,PIN5)==0 && Global_u8WideFlag == 1 ) {

				Global_u8WideFlag=0;
			}



			Global_u16SwitchsData=ShiftRegister_u16GetData();

			CheckSwitchState();

			if(GET_BIT(Global_u16SwitchsData,7)==1)
			{
				if(Global_u8Day_Thermal_Flag==0) // thermal status
				{
					USART1_VoidWriteString((u8 *)"*F-#");
					delay_voidXms(500);
				}
				else
				{

				}
			}

			if(GET_BIT(Global_u16SwitchsData,6)==1)
			{
				USART1_VoidWriteString((u8 *)"*LRFF#");
				delay_voidXms(500);
			}
			if(GET_BIT(Global_u16SwitchsData,5)==1)
			{
				USART1_VoidWriteString((u8 *)"*LL#");
				delay_voidXms(500);
			}

		}
	}
}




void INT_DAY_THERMAL(void){



	u8 Local_u8Pin1_value=MGPIO_u8GetPinValue(PORTA,PIN15);


	if(Local_u8Pin1_value==1 && Global_u8DTStateFlag==1)
	{
		USART1_VoidWriteString((u8 *)"*T#");
		delay_voidXms(10);
		Global_u8Day_Thermal_Flag=0;
		Global_u8DTStateFlag=0;


		/*if(Global_u8F_T==1)
		{
			Global_u8FVState=MFV_FLAG;
			Global_u8FV_Status_Flag=1;
			USART1_VoidWriteString((u8 *)"*F#");
			Current_LED_FOV=Nfov;
			Control_Panelvoid_Message_For_LED(NFOV);
			Global_u8F_T=0;
		}*/
	}
	else if(Local_u8Pin1_value==0 && Global_u8DTStateFlag==0)
	{
		USART1_VoidWriteString((u8 *)"*D#");
		delay_voidXms(10);
		Global_u8Day_Thermal_Flag=1;
		Global_u8DTStateFlag=1;

	}
	//delay_voidXms(10);
	MEXTI_voidClearPendingFlag(15);

}


void INT_NFOV(void)
{

	Control_Panelvoid_Message_For_LED(NFOV);

	if(Global_u8FVState==NFV_FLAG)
	{
		delay_voidXms(100);
		Global_u8FVState=MFV_FLAG;
		//Global_u8FV_Status_Flag=1;
		USART1_VoidWriteString((u8 *)"*F#");
		Current_LED_FOV=Nfov;
	}

	MEXTI_voidClearPendingFlag(0);
}


void INT_First_Echo_LAST_ECHO(void)
{
	u8 Local_u8Pin6_value=MGPIO_u8GetPinValue(PORTA,PIN6);
	u8 Local_u8Pin7_value=MGPIO_u8GetPinValue(PORTA,PIN7);

	if(Local_u8Pin7_value==1 && Global_u8EchoState==LECHO_FLAG)
	{
		Global_u8EchoState=FECHO_FLAG;
		Global_u8Echo_Status_Flag=1;
		Control_Panelvoid_Message_For_LED(LAST_ECHO);
		Current_LED_Echo=Last_Echo;
		USART1_VoidWriteString((u8 *)"*LE#");
	}
	else if(Local_u8Pin6_value==1 && Global_u8EchoState==OFF_FLAG)
	{
		Global_u8EchoState=FECHO_FLAG;
		Global_u8Echo_Status_Flag=1;
		Control_Panelvoid_Message_For_LED(ECHO_OFF);
		Current_LED_Echo=Off;
		USART1_VoidWriteString((u8 *)"*EO#");
	}

	MEXTI_voidClearPendingFlag(6);
	MEXTI_voidClearPendingFlag(7);

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
// Switch State Check Function
void CheckSwitchState(void)
{
	if(Global_TimerEnteredOnce==0)
	{
		TIM2_voidReset();
		TIM2_voidStart();
		Global_TimerEnteredOnce=1;
		LastBullet=Current_LED_Bullet;
	}
	// static u32 last_change_time = 0;
	current_time = TIM2_u32GetTime();

	/*if(GET_BIT(Global_u16SwitchsData,0)==0  && GET_BIT(Global_u16SwitchsData,1)==0  && GET_BIT(Global_u16SwitchsData,2)==0  && GET_BIT(Global_u16SwitchsData,3)==0 && GET_BIT(Global_u16SwitchsData,4)==0 && Global_u8BulletsOn==1 )
	{
		Global_u8BulletsOn=0;
	}*/
	if(GET_BIT(Global_u16SwitchsData,0)==1  && GET_BIT(Global_u16SwitchsData,1)==0  && GET_BIT(Global_u16SwitchsData,2)==0  && GET_BIT(Global_u16SwitchsData,3)==0  && GET_BIT(Global_u16SwitchsData,4)==0 && Global_u8BulletState==HIGH_EXPO_FLAG)
	{

		Global_u8BulletState=HEAT_FLAG;
		Global_u8BulletsOn=1;
		//USART1_VoidWriteString((u8 *)"*HE#");
		Current_LED_Bullet=High_Exp;
		Control_Panelvoid_Message_For_LED(HIGH_EXP);
		Global_u8Bullets_Flag=0;
		Global_u8FirstBullet=0;
	}

	if(GET_BIT(Global_u16SwitchsData,0)==0  && GET_BIT(Global_u16SwitchsData,1)==1  && GET_BIT(Global_u16SwitchsData,2)==0  && GET_BIT(Global_u16SwitchsData,3)==0 && GET_BIT(Global_u16SwitchsData,4)==0 && Global_u8BulletState==HEAT_FLAG)
	{

		Global_u8BulletState=SABOT_FLAG;
		//USART1_VoidWriteString((u8 *)"*HT#");
		Current_LED_Bullet=Heat;
		Control_Panelvoid_Message_For_LED(HEAT);
		Global_u8Bullets_Flag=0;
		Global_u8BulletsOn=1;
		Global_u8FirstBullet=0;

	}
	if(GET_BIT(Global_u16SwitchsData,0)==0  && GET_BIT(Global_u16SwitchsData,1)==0  && GET_BIT(Global_u16SwitchsData,2)==1  && GET_BIT(Global_u16SwitchsData,3)==0  && GET_BIT(Global_u16SwitchsData,4)==0 && Global_u8BulletState==SABOT_FLAG)
	{


		Global_u8BulletState=HEAT_FLAG;
		//USART1_VoidWriteString((u8 *)"*ST#");
		Current_LED_Bullet=Sabot;
		Control_Panelvoid_Message_For_LED(SABOT);
		Global_u8Bullets_Flag=0;
		Global_u8BulletsOn=1;
		Global_u8FirstBullet=0;

	}
	if(GET_BIT(Global_u16SwitchsData,0)==0  && GET_BIT(Global_u16SwitchsData,1)==0  && GET_BIT(Global_u16SwitchsData,2)==0  && GET_BIT(Global_u16SwitchsData,3)==1  && GET_BIT(Global_u16SwitchsData,4)==0 && Global_u8BulletState==HEP_FLAG)
	{


		Global_u8BulletState=COAXIAL_GUN_FLAG;
		//USART1_VoidWriteString((u8 *)"*SC#");
		Global_u8Bullets_Flag=0;
		Current_LED_Bullet=Hep;
		Control_Panelvoid_Message_For_LED(HEP);
		Global_u8BulletsOn=1;
		Global_u8FirstBullet=0;

	}
	if(GET_BIT(Global_u16SwitchsData,0)==0  && GET_BIT(Global_u16SwitchsData,1)==0  && GET_BIT(Global_u16SwitchsData,2)==0  && GET_BIT(Global_u16SwitchsData,3)==0  && GET_BIT(Global_u16SwitchsData,4)==1 && Global_u8BulletState==COAXIAL_GUN_FLAG )
	{

		Global_u8BulletState=HEP_FLAG;
		Global_u8Bullets_Flag=1;
		//USART1_VoidWriteString((u8 *)"*CG#");
		Current_LED_Bullet=Coaxial;
		Control_Panelvoid_Message_For_LED(Coaxial_GUN);
		Global_u8BulletsOn=1;
		Global_u8FirstBullet=0;
	}
	if(LastBullet!=Current_LED_Bullet)
	{
		Global_u8BulletsEnteredOnce=0;
	}

	if(current_time>=1500 && Global_u8BulletsEnteredOnce==0 && Global_u8FirstBullet==0)
	{
		Global_TimerEnteredOnce=0;
		Global_u8BulletsEnteredOnce=1;
		TIM2_voidStop();
		TIM2_voidReset();

		switch (Current_LED_Bullet)
		{
		case Coaxial:
			USART1_VoidWriteString((u8 *)"*SC#");
			break;
		case Hep:
			USART1_VoidWriteString((u8 *)"*CG#");
			break;
		case Sabot:
			USART1_VoidWriteString((u8 *)"*ST#");
			break;
		case Heat:
			USART1_VoidWriteString((u8 *)"*HT#");
			break;
		case High_Exp:
			USART1_VoidWriteString((u8 *)"*HE#");
			break;
		}

	}



}
