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
void INT_DAY_THERMAL(void);
void INT_First_Echo_LAST_ECHO(void);

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
u8 Global_u8ResetLamps=0;


// FOV State Machine Debouncing
volatile u32 Global_u32LastFOVPressTime = 0;
#define FOV_DEBOUNCE_MS 50
u8 Global_u8LastPA5_State = 0;
u8 Global_u8LastPB0_State = 0;
u8 Global_u8ExternalFOVTrigger = 0;
u8 Global_u8FirstBullet;

void main()
{
    // ------- Initialization -------
    MRCC_voidInitSysClock();
    MRCC_voidEnableClock(RCC_APB2, GPIO_PORTA_BUS);
    MRCC_voidEnableClock(RCC_APB2, GPIO_PORTB_BUS);
    MRCC_voidEnableClock(RCC_APB2, GPIO_PORTC_BUS);
    MRCC_voidEnableClock(RCC_APB2, AFIO_BUS);
    MAFIO->MAPR |= (1 << 25);

    Sys_VoidInit(TimerMode);
    USART_Init();
    TIM2_voidInit();
    TIM3_Init();

    Control_Panelvoid_Init();

    MEXTI_voidEnable(EXTI_PORTA, PIN6, RISING_EDGE);
    MEXTI_voidEnable(EXTI_PORTA, PIN7, RISING_EDGE);
    MEXTI_voidEnable(EXTI_PORTB, PIN0, RISING_EDGE);
    MEXTI_voidEnable(EXTI_PORTA, PIN15, EDGE_CHANGE);

    MEXTINT0_VidSetCallBack(&INT_NFOV);
    MEXTINT9_5_VidSetCallBack(&INT_First_Echo_LAST_ECHO);
    MEXTINT15_10_VidSetCallBack(&INT_DAY_THERMAL);

    // ------- Main Loop (Optimized) -------
    while(1)
    {
        // 1. Read all inputs ONCE at the start of the loop iteration
        Global_u8StartCommand = USART1_u8ReadChar_NonBlocking();
        Global_u16SwitchsData = ShiftRegister_u16GetData();
        current_time = TIM2_u32GetTime(); // Get current time for debouncing

        // Cache ALL GPIO pin states for this loop iteration
        u8 pin_PB14 = MGPIO_u8GetPinValue(PORTB, PIN14);
        u8 pin_PC13 = MGPIO_u8GetPinValue(PORTC, PIN13);
        u8 pin_PB12 = MGPIO_u8GetPinValue(PORTB, PIN12);
        u8 pin_PB4  = MGPIO_u8GetPinValue(PORTB, PIN4);
        u8 pin_PB8  = MGPIO_u8GetPinValue(PORTB, 8);
        u8 pin_PB11 = MGPIO_u8GetPinValue(PORTB, 11);
        u8 pin_PB7  = MGPIO_u8GetPinValue(PORTB, 7);
        u8 pin_PB13 = MGPIO_u8GetPinValue(PORTB, 13);
        u8 pin_PA8  = MGPIO_u8GetPinValue(PORTA, 8);
        u8 pin_PA11 = MGPIO_u8GetPinValue(PORTA, 11);
        u8 pin_PB6  = MGPIO_u8GetPinValue(PORTB, 6);
        u8 pin_PB1  = MGPIO_u8GetPinValue(PORTB, 1);
        u8 pin_PA6  = MGPIO_u8GetPinValue(PORTA, 6);
        u8 pin_PA7  = MGPIO_u8GetPinValue(PORTA, 7);
        u8 pin_PB0  = MGPIO_u8GetPinValue(PORTB, 0);
        u8 pin_PA5  = MGPIO_u8GetPinValue(PORTA, 5);

        // 2. Process Start Command
        if (Global_u8StartCommand != 0)
        {
            if(Global_u8StartCommand == 'R')
            {
                software_reset();
            }
            else if(Global_u8StartCommand == 'S')
            {
                USART1_VoidWriteString((u8*)"*S,");
                Control_Panel_voidStartUpLeds();
                Global_u8StartThePanel = 1;
                Global_u8StartCounter = 0;
            }
        }

        // 3. Main Panel Logic
        if(Global_u8StartThePanel == 1)
        {
            // Use the cached pin values for fast comparisons
            if(pin_PB14 == 1)
            {
                if(Global_u8Day_Thermal_Flag == 0)
                {
                    delay_voidXms(1000);
                    USART1_VoidWriteString((u8 *)"*F+#");
                }
            }

            if(pin_PC13 == 0)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8 *)"*E-#");
            }

            if(pin_PB4 == 1)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8 *)"*A#");
            }

            if(pin_PB8 == 1)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8*)"*E+#");
            }

            if(pin_PB11 == 1)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8 *)"*C+#");
            }

            if(pin_PB7 == 1)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8 *)"*C-#");
            }

            if(pin_PB13 == 1)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8 *)"*TAF#");
            }

            if(pin_PA8 == 1)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8 *)"*BTLRNG#");
            }

            if(pin_PA11 == 1)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8 *)"*B+#");
            }

            if(pin_PB6 == 1)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8 *)"*B-#");
            }

            if(pin_PB1 == 1)
            {
                delay_voidXms(1000);
                USART1_VoidWriteString((u8 *)"*CLR#");
            }

            // Lamp test logic
            if(pin_PB12 == 1)
            {
                HandleLampTestSequence();
            }

            // Echo logic using cached values
            if(pin_PA6 == 0 && pin_PA7 == 0 && Global_u8Echo_Status_Flag==1 && Global_u8EchoState==FECHO_FLAG)
            {
                Global_u8EchoState = LECHO_FLAG;
                Global_u8Echo_Status_Flag = 0;
                Control_Panelvoid_Message_For_LED(FIRST_ECHO);
                USART1_VoidWriteString((u8 *)"*FE#");
                Current_LED_Echo = First_Echo;
            }

            // --- ROBUST FOV STATE MACHINE ---
            u8 current_pa5 = pin_PA5;
            u8 current_pb0 = pin_PB0;

            // Check for RISING edge on PA5 ("Wide" button) with debounce
            if( (current_pa5 == 1) && (Global_u8LastPA5_State == 0) )
            {
                if( (current_time - Global_u32LastFOVPressTime) > FOV_DEBOUNCE_MS )
                {
                    Global_u32LastFOVPressTime = current_time;
                    // Handle Wide button press: Cycle FOV State Forward
                    switch(Global_u8FVState)
                    {
                        case NFV_FLAG:
                            Global_u8FVState = MFV_FLAG;
                            Control_Panelvoid_Message_For_LED(MFOV);
                            Current_LED_FOV = Mfov;
                            USART1_VoidWriteString((u8 *)"*F#");
                            break;
                        case MFV_FLAG:
                            Global_u8FVState = WFV_FLAG;
                            Control_Panelvoid_Message_For_LED(WFOV);
                            Current_LED_FOV = Wfov;
                            USART1_VoidWriteString((u8 *)"*W#");
                            break;
                        case WFV_FLAG:
                            Global_u8FVState = NFV_FLAG;
                            Control_Panelvoid_Message_For_LED(NFOV);
                            Current_LED_FOV = Nfov;
                            USART1_VoidWriteString((u8 *)"*N#");
                            break;
                    }
                    Global_u8FV_Status_Flag = 1;
                }
            }
            Global_u8LastPA5_State = current_pa5;

            // Check for RISING edge on PB0 ("Narrow" button) with debounce
            if( (current_pb0 == 1) && (Global_u8LastPB0_State == 0) )
            {
                if( (current_time - Global_u32LastFOVPressTime) > FOV_DEBOUNCE_MS )
                {
                    Global_u32LastFOVPressTime = current_time;
                    // Handle Narrow button press: Cycle FOV State Backward
                    switch(Global_u8FVState)
                    {
                        case NFV_FLAG:
                            Global_u8FVState = WFV_FLAG;
                            Control_Panelvoid_Message_For_LED(WFOV);
                            Current_LED_FOV = Wfov;
                            USART1_VoidWriteString((u8 *)"*W#");
                            break;
                        case MFV_FLAG:
                            Global_u8FVState = NFV_FLAG;
                            Control_Panelvoid_Message_For_LED(NFOV);
                            Current_LED_FOV = Nfov;
                            USART1_VoidWriteString((u8 *)"*N#");
                            break;
                        case WFV_FLAG:
                            Global_u8FVState = MFV_FLAG;
                            Control_Panelvoid_Message_For_LED(MFOV);
                            Current_LED_FOV = Mfov;
                            USART1_VoidWriteString((u8 *)"*F#");
                            break;
                    }
                    Global_u8FV_Status_Flag = 1;
                }
            }
            Global_u8LastPB0_State = current_pb0;

            // Handle external FOV trigger from interrupt
            if(Global_u8ExternalFOVTrigger)
            {
                Global_u8ExternalFOVTrigger = 0;
                Control_Panelvoid_Message_For_LED(NFOV);
                USART1_VoidWriteString((u8 *)"*N#");
                Current_LED_FOV = Nfov;
                Global_u8FVState = NFV_FLAG;
            }

            // Check switch state
            CheckSwitchState();

            // Check shift register bits using the pre-read value
            if(Global_u16SwitchsData & (1 << 7)) // Bit 7
            {
                if(Global_u8Day_Thermal_Flag == 0)
                {
                    delay_voidXms(1000);
                    USART1_VoidWriteString((u8 *)"*F-#");
                }
            }

            if(Global_u16SwitchsData & (1 << 6)) // Bit 6
            {
                delay_voidXms(200);
                USART1_VoidWriteString((u8 *)"*LRFF#");
            }

            if(Global_u16SwitchsData & (1 << 5)) // Bit 5
            {
                delay_voidXms(200);
                USART1_VoidWriteString((u8 *)"*LL#");
            }
        }
    }
}
void INT_DAY_THERMAL(void)
{
    u8 Local_u8Pin1_value = MGPIO_u8GetPinValue(PORTA, PIN15);

    if(Local_u8Pin1_value == 1 && Global_u8DTStateFlag == 1)
    {
        USART1_VoidWriteString((u8 *)"*T#");
        delay_voidXms(10);
        Global_u8Day_Thermal_Flag = 0;
        Global_u8DTStateFlag = 0;
    }
    else if(Local_u8Pin1_value == 0 && Global_u8DTStateFlag == 0)
    {
        USART1_VoidWriteString((u8 *)"*D#");
        delay_voidXms(10);
        Global_u8Day_Thermal_Flag = 1;
        Global_u8DTStateFlag = 1;
    }
    MEXTI_voidClearPendingFlag(15);
}

void INT_NFOV(void)
{
    // Set flag for main loop to process
    Global_u8ExternalFOVTrigger = 1;
    MEXTI_voidClearPendingFlag(0);
}

void INT_First_Echo_LAST_ECHO(void)
{
    u8 Local_u8Pin6_value = MGPIO_u8GetPinValue(PORTA, PIN6);
    u8 Local_u8Pin7_value = MGPIO_u8GetPinValue(PORTA, PIN7);

    if(Local_u8Pin7_value == 1 && Global_u8EchoState == LECHO_FLAG)
    {
        Global_u8EchoState = FECHO_FLAG;
        Global_u8Echo_Status_Flag = 1;
        Control_Panelvoid_Message_For_LED(LAST_ECHO);
        Current_LED_Echo = Last_Echo;
        USART1_VoidWriteString((u8 *)"*LE#");
    }
    else if(Local_u8Pin6_value == 1 && Global_u8EchoState == OFF_FLAG)
    {
        Global_u8EchoState = FECHO_FLAG;
        Global_u8Echo_Status_Flag = 1;
        Control_Panelvoid_Message_For_LED(ECHO_OFF);
        Current_LED_Echo = Off;
        USART1_VoidWriteString((u8 *)"*EO#");
    }
    MEXTI_voidClearPendingFlag(6);
    MEXTI_voidClearPendingFlag(7);
}

// Switch State Check Function
void CheckSwitchState(void)
{
    if(Global_TimerEnteredOnce == 0)
    {
        TIM2_voidReset();
        TIM2_voidStart();
        Global_TimerEnteredOnce = 1;
        LastBullet = Current_LED_Bullet;
    }

    current_time = TIM2_u32GetTime();

    // OPTIMIZATION: Cache globals for consistent reads
    const u16 switchData = Global_u16SwitchsData;
    const u8 bulletState = Global_u8BulletState;

    // OPTIMIZATION: Check if exactly one switch is active (bits 0-4)
    if((switchData & 0x001F) != 0) // Check if any of bits 0-4 are set
    {
        if((switchData & (switchData - 1)) == 0) // Check if only one bit is set in the whole variable
        {
            // Determine which switch is active
            u8 activeSwitch = 0;
            u16 temp = switchData;
            while(temp >>= 1) activeSwitch++;

            // OPTIMIZATION: Replace if-else chain with switch
            switch(activeSwitch)
            {
                case 0: // Switch 0
                    if(bulletState == HIGH_EXPO_FLAG)
                    {
                        Global_u8BulletState = HEAT_FLAG;
                        Global_u8BulletsOn = 1;
                        Current_LED_Bullet = High_Exp;
                        Control_Panelvoid_Message_For_LED(HIGH_EXP);
                        Global_u8Bullets_Flag = 0;
                        Global_u8FirstBullet = 0;
                    }
                    break;

                case 1: // Switch 1
                    if(bulletState == HEAT_FLAG)
                    {
                        Global_u8BulletState = SABOT_FLAG;
                        Current_LED_Bullet = Heat;
                        Control_Panelvoid_Message_For_LED(HEAT);
                        Global_u8Bullets_Flag = 0;
                        Global_u8BulletsOn = 1;
                        Global_u8FirstBullet = 0;
                    }
                    break;

                case 2: // Switch 2
                    if(bulletState == SABOT_FLAG)
                    {
                        Global_u8BulletState = HEAT_FLAG;
                        Current_LED_Bullet = Sabot;
                        Control_Panelvoid_Message_For_LED(SABOT);
                        Global_u8Bullets_Flag = 0;
                        Global_u8BulletsOn = 1;
                        Global_u8FirstBullet = 0;
                    }
                    break;

                case 3: // Switch 3
                    if(bulletState == HEP_FLAG)
                    {
                        Global_u8BulletState = COAXIAL_GUN_FLAG;
                        Global_u8Bullets_Flag = 0;
                        Current_LED_Bullet = Hep;
                        Control_Panelvoid_Message_For_LED(HEP);
                        Global_u8BulletsOn = 1;
                        Global_u8FirstBullet = 0;
                    }
                    break;

                case 4: // Switch 4
                    if(bulletState == COAXIAL_GUN_FLAG)
                    {
                        Global_u8BulletState = HEP_FLAG;
                        Global_u8Bullets_Flag = 1;
                        Current_LED_Bullet = Coaxial;
                        Control_Panelvoid_Message_For_LED(Coaxial_GUN);
                        Global_u8BulletsOn = 1;
                        Global_u8FirstBullet = 0;
                    }
                    break;
            }
        }
    }

    if(LastBullet != Current_LED_Bullet)
    {
        Global_u8BulletsEnteredOnce = 0;
    }

    if(current_time >= 1500 && Global_u8BulletsEnteredOnce == 0 && Global_u8FirstBullet == 0)
    {
        Global_TimerEnteredOnce = 0;
        Global_u8BulletsEnteredOnce = 1;
        TIM2_voidStop();
        TIM2_voidReset();

        // REVERTED TO ORIGINAL: Safest option without knowing enum values
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
void HandleLampTestSequence(void)
{
    Global_u8ResetLamps++;
    static u8 LampTestState = 0;

    if(Global_u8ResetLamps == 1)
    {
        handleLampTest();
        LampTestState = 1;
    }
    else if(Global_u8ResetLamps >= 2)
    {
        resetLamps();
        // Restore previous states
        switch(Current_LED_FOV)
        {
            case Nfov: Control_Panelvoid_Message_For_LED(NFOV); break;
            case Mfov: Control_Panelvoid_Message_For_LED(MFOV); break;
            case Wfov: Control_Panelvoid_Message_For_LED(WFOV); break;
        }
        switch(Current_LED_Echo)
        {
            case First_Echo: Control_Panelvoid_Message_For_LED(FIRST_ECHO); break;
            case Last_Echo:  Control_Panelvoid_Message_For_LED(LAST_ECHO); break;
            case Off:        Control_Panelvoid_Message_For_LED(ECHO_OFF); break;
        }
        switch(Current_LED_Bullet)
        {
            case Coaxial: Control_Panelvoid_Message_For_LED(Coaxial_GUN); break;
            case Hep:     Control_Panelvoid_Message_For_LED(HEP); break;
            case Sabot:   Control_Panelvoid_Message_For_LED(SABOT); break;
            case Heat:    Control_Panelvoid_Message_For_LED(HEAT); break;
            case High_Exp:Control_Panelvoid_Message_For_LED(HIGH_EXP); break;
        }
        Global_u8ResetLamps = 0;
        LampTestState = 0;
    }
}
