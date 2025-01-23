/*
 * SYS_private.h
 *
 *  Created on: Jan 18, 2023
 *      Author: Main User
 */

#ifndef SYS_PRIVATE_H
#define SYS_PRIVATE_H

/* registers definitions needed for SysTick */

#define Sys_base   0xE000E010       /* base address for systick timer */



#define Sys_CTRL     (*(volatile u32 *)(0xE000E010+0x00))
#define Sys_RELOAD   (*(volatile u32 *)(0xE000E010+0x04))
#define Sys_CURRENT  (*(volatile u32 *)(0xE000E010+0x08))


#endif /* SYS_PRIVATE_H_ */
