/***************************************************************************************************/
/* Author  : Enjy Mohamed                                                                          */
/* Version : V01                                                                                   */
/* Date    : 18 Jan 2023                                                                            */
/* Company : AIO                                                                            */
/***************************************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "SYS_interface.h"
#include "SYS_private.h"
#include "SYS_config.h"

#include "GPIO_interface.h"
#include "GPIO_private.h"
u8 counter=0;
void Sys_VoidInit (u8 Copy_u8SysTickMode) {
	Sys_CTRL    = LOW;     /* turning off the systick before configuration */
	Sys_RELOAD  = 0xFFFFFF;   /* setting the reload value as max value of 24-bit systick */
	Sys_CURRENT = 0;         /* writing any value to current register as initialization procedure required */
switch (Copy_u8SysTickMode) {

	case TimerMode:
	Sys_CTRL    = 0x5; break;
/* enabling system clock not divided by 8, disabling interrupt mode, enabling systick timer bit */

	case InterruptMode:
	Sys_CTRL    = 0x7; break;
/* enabling system clock not divided by 8, enabling interrupt mode, enabling systick timer bit */
	}
}


void Sys_VoidReload (u32 Copy_u32CurrentFreqInMHz, u32 Copy_u32WantedSysTimeinms) {
    u32 reload;
    reload = Copy_u32CurrentFreqInMHz * Copy_u32WantedSysTimeinms * 1000; /*calculating reload value */
if (reload <= Sys_Max)  /* checking if it's less than max value of reload register */
{
    Sys_RELOAD  = reload;
    Sys_CURRENT = 0;
}
else {  /* if it's more just set the max value to sys_max */
    Sys_RELOAD  = Sys_Max;
    Sys_CURRENT = 0;
}
}


/* void SysTick_Handler (){
}
 */

void delay_void1ms() {
	 Sys_VoidReload (SystemClock, 1);
	 while ((Sys_CTRL & 0x10000) ==0);     // waiting for the flag of SysTick timer to be raised during roll-over
}


void delay_voidXms(u32 Copy_u32WantedDelayinms){
     u32 i;
for (i=0; i<Copy_u32WantedDelayinms; i++) {
     delay_void1ms();
 }
}



