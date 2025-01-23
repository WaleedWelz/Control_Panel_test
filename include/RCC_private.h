/***************************************************************************************************/
/* Author  : Kareem Magdy                                                                          */
/* Version : V01                                                                                   */
/* Date    : 1 Jan 2023                                                                            */
/* Company : AIO                                                                            */
/***************************************************************************************************/


#ifndef _RCC_PRIVATE_H
#define _RCC_PRIVATE_H



#define BIT_0            0
#define BIT_1            1
#define BIT_2            2
#define BIT_3            3
#define BIT_4            4
#define BIT_5            5
#define BIT_6            6
#define BIT_7            7
#define BIT_8            8
#define BIT_9            9
#define BIT_10           10
#define BIT_11           11
#define BIT_12           12
#define BIT_13           13
#define BIT_14           14
#define BIT_15           15
#define BIT_16           16
#define BIT_17           17
#define BIT_18           18
#define BIT_19           19
#define BIT_20           20
#define BIT_21           21
#define BIT_22           22
#define BIT_23           23
#define BIT_24           24
#define BIT_25           25
#define BIT_26           26
#define BIT_27           27
#define BIT_28           28
#define BIT_29           29
#define BIT_30           30
#define BIT_31           31


#define HSI_NOT_READY    0
#define HSE_NOT_READY    0
#define PLL_NOT_READY    0



/* Register Definition   */
typedef struct{
	volatile u32 CR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 APB2RSTR;
	volatile u32 APB1RSTR;
	volatile u32 AHBENR;
	volatile u32 APB2ENR;
	volatile u32 APB1ENR;
	volatile u32 BDCR;
	volatile u32 CSR;
}MRCC;


#define MRCC ((volatile MRCC *)0x40021000)



#endif
