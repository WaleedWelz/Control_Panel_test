/**************************************************************************************************/
/* Author  : Kareem Magdy                                                                         */
/* Version : V01                                                                                  */
/* Date    : 16 Jan 2023                                                                           */
/**************************************************************************************************/
#ifndef _NVIC_PRIVATE_H
#define _NVIC_PRIVATE_H


#define NVIC_ISER0      *((volatile u32*)0xE000E100)  /*Enable  External Interrupts  0 -> 31*/
#define NVIC_ISER1      *((volatile u32*)0xE000E104)  /*Enable  External Interrupts 32 -> 63*/

#define NVIC_ICER0      *((volatile u32*)0xE000E180)  /*Disable External Interrupts  0 -> 31*/
#define NVIC_ICER1      *((volatile u32*)0xE000E184)  /*Disable External Interrupts 32 -> 63*/

#define NVIC_ISPR0      *((volatile u32*)0xE000E200)  /* Set  Pending Flag    0 -> 31*/
#define NVIC_ISPR1      *((volatile u32*)0xE000E204)  /* Set  Pending Flag    32 -> 63*/

#define NVIC_ICPR0      *((volatile u32*)0xE000E280)  /* Clear  Pending Flag    0 -> 31*/
#define NVIC_ICPR1      *((volatile u32*)0xE000E284)  /* Clear  Pending Flag    32 -> 63*/

#define NVIC_IABR0      *((volatile u32*)0xE000E300)  /* Active Flag    0 -> 31 */
#define NVIC_IABR1      *((volatile u32*)0xE000E304)  /* Active Flag    32 -> 63*/

#define NVIC_IPR         ((volatile u8*)0xE000E400)




#endif
