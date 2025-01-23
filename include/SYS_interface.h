/***************************************************************************************************/
/* Author  : Enjy Mohamed                                                                          */
/* Version : V01                                                                                   */
/* Date    : 18 Jan 2023                                                                            */
/* Company : AIO                                                                            */
/***************************************************************************************************/


#ifndef SYS_INTERFACE_H
#define SYS_INTERFACE_H



/* Full capacity of SysTick RELOAD 24-bit register */

#define Sys_Max   16777215



/* SysTick Initialization options, either interrupts enabled for usage of SysTick handler or disabled */

#define TimerMode     0
#define InterruptMode 1



/* SysTick different functions prototypes */

void Sys_VoidInit (u8 Copy_u8SysTickMode);  /*initialization */
void Sys_VoidReload (u32 Copy_u32CurrentFreqInMHz, u32 Copy_u32WantedSysTimeinms);
void SysTick_Handler (void);
void delay_void1ms(void);     /*  delay for 1 ms function  */
void delay_voidXms(u32 Copy_u32WantedDelayinms); /* delay for x ms function */


#endif /* SYS_INTERFACE_H_ */
