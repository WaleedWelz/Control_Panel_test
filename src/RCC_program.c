/**************************************************************************************************/
/* Author  : Kareem Magdy                                                                         */
/* Version : V01                                                                                  */
/* Date    : 1 Jan 2023                                                                           */
/**************************************************************************************************/


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "RCC_private.h"
#include "RCC_config.h"




void MRCC_voidInitSysClock(void)
{   
    
	/* Enable CSSON ( Clock Secuirty System )*/
    SET_BIT(MRCC->CR,BIT_19);
     
	#if SYSTEM_CLOCK == HSI
	
	/* Choose HSI as System Clock */
    CLR_BIT(MRCC->CFGR,BIT_0);
    CLR_BIT(MRCC->CFGR,BIT_1);
	
	/* Enable HSI Clock */
    SET_BIT(MRCC->CR  ,BIT_0);
    
  	
	/*Wait HSI To Be Ready */
	while( GET_BIT(MRCC->CR,BIT_1)== HSI_NOT_READY );

    #endif

    #if  SYSTEM_CLOCK == HSE
	
	
	   #if  HSE_TYPE  ==  CRYSTAL
	   
    /* External 3-25 MHz oscillator Crystal */
	CLR_BIT(MRCC->CR ,BIT_18);
	   
	   #elif HSE_TYPE  ==  RC
	   
	   /* External 3-25 MHz oscillator RC */ 
	   SET_BIT(RCC->CR ,BIT_18);

       #endif	   
	   
	/* Choose HSE as System Clock */
    SET_BIT(MRCC->CFGR,BIT_0);
    CLR_BIT(MRCC->CFGR,BIT_1);
	
	/* Enable HSE Clock */
    SET_BIT(MRCC->CR  ,BIT_16);
	
	/*Wait HSE To Be Ready */
	while( GET_BIT(MRCC->CR ,BIT_17)== HSE_NOT_READY );
	
	#elif  SYSTEM_CLOCK == PLL
	
	    /* Choose PLL Input Clock Source */
	    #if  PLL_CLOCK_SOURCE  == PLL_HSI_DIVIDED_BY_2
		
         /* HSI Oscillator Clock / 2 Selected As PLL Input Clock */ 
		 CLR_BIT(MRCC->CFGR,BIT_16);
		 
		 
		 #elif  PLL_CLOCK_SOURCE  == PLL_HSE
		 /* HSE Selected As PLL Input Clock */ 
		 SET_BIT(MRCC->CFGR,BIT_16);
		 
		 
		 #elif  PLL_CLOCK_SOURCE  == PLL_HSE_DIVIDED_BY_2
		 /* HSE Selected As PLL Input Clock */ 
		 SET_BIT(MRCC->CFGR,BIT_16);
		 
		 /* HSE Oscillator Clock /2 As PLL Input Clock*/
		 SET_BIT(MRCC->CFGR,BIT_17);
		 
		 #endif
		 
	    #if	    PLL_MULTIPLE_VALUE  ==  PLL_CLOCK_MULTIPLE_BY_4
	    
		      /* PLL input clock x 4 */ 
		      CLR_BIT(MRCC->CFGR, BIT_18);
		      SET_BIT(MRCC->CFGR, BIT_19);
		      CLR_BIT(MRCC->CFGR, BIT_20);
		      CLR_BIT(MRCC->CFGR, BIT_21);
		
	    #elif    PLL_MULTIPLE_VALUE  ==  PLL_CLOCK_MULTIPLE_BY_5
	
	          /* PLL input clock x 5 */ 
		      SET_BIT(MRCC->CFGR, BIT_18);
		      SET_BIT(MRCC->CFGR, BIT_19);
		      CLR_BIT(MRCC->CFGR, BIT_20);
		      CLR_BIT(MRCC->CFGR, BIT_21);
			  
	    #elif    PLL_MULTIPLE_VALUE  ==  PLL_CLOCK_MULTIPLE_BY_6
	
	          /* PLL input clock x 6 */ 
		      CLR_BIT(MRCC->CFGR, BIT_18);
		      CLR_BIT(MRCC->CFGR, BIT_19);
		      SET_BIT(MRCC->CFGR, BIT_20);
		      CLR_BIT(MRCC->CFGR, BIT_21);
			  
	    #elif    PLL_MULTIPLE_VALUE  ==  PLL_CLOCK_MULTIPLE_BY_7
	
	          /* PLL input clock x 7 */ 
		      SET_BIT(MRCC->CFGR, BIT_18);
		      CLR_BIT(MRCC->CFGR, BIT_19);
		      SET_BIT(MRCC->CFGR, BIT_20);
		      CLR_BIT(MRCC->CFGR, BIT_21);
		 
	    #elif    PLL_MULTIPLE_VALUE  ==  PLL_CLOCK_MULTIPLE_BY_8
	
	          /* PLL input clock x 8 */ 
		      CLR_BIT(MRCC->CFGR, BIT_18);
		      SET_BIT(MRCC->CFGR, BIT_19);
		      SET_BIT(MRCC->CFGR, BIT_20);
		      CLR_BIT(MRCC->CFGR, BIT_21);
		 
	    #elif    PLL_MULTIPLE_VALUE  ==  PLL_CLOCK_MULTIPLE_BY_9
	 
	          /* PLL input clock x 9 */ 
		      SET_BIT(MRCC->CFGR, BIT_18);
		      SET_BIT(MRCC->CFGR, BIT_19);
		      SET_BIT(MRCC->CFGR, BIT_20);
		      CLR_BIT(MRCC->CFGR, BIT_21);
	 		  
	    #elif    PLL_MULTIPLE_VALUE  ==  PLL_CLOCK_MULTIPLE_BY_6.5
	
	          /* PLL input clock x 6.5 */ 
		      SET_BIT(MRCC->CFGR, BIT_18);
		      CLR_BIT(MRCC->CFGR, BIT_19);
		      SET_BIT(MRCC->CFGR, BIT_20);
		      SET_BIT(MRCC->CFGR, BIT_21);
			  
		#endif
		 
	/* Choose PLL as System Clock */
    CLR_BIT(MRCC->CFGR,BIT_0);
    SET_BIT(MRCC->CFGR,BIT_1);
	
	/* Enable PLL Clock */
    SET_BIT(MRCC->CR  ,BIT_24);

    /*Wait PLL To Be Ready */
	while( GET_BIT(MRCC->CR ,BIT_25)== PLL_NOT_READY );
		 
	
     	   
	
    #endif
	
}



void MRCC_voidEnableClock(u8 Copy_u8BusId, u8 Copy_u8PerifId )
{ 
    /* Input Limitation 0 --> 31 */
    if ( Copy_u8PerifId <= 31 )
	{
		switch ( Copy_u8BusId ) 
		{
			case RCC_AHB :   SET_BIT( MRCC->AHBENR  ,Copy_u8PerifId);  break;
			case RCC_APB1:   SET_BIT( MRCC->APB1ENR ,Copy_u8PerifId);  break;
			case RCC_APB2:   SET_BIT( MRCC->APB2ENR ,Copy_u8PerifId);  break;
			
		}
	}
	
	else 
	{
		/* Return Error */
	}
} 

void MRCC_voidDisableClock(u8 Copy_u8BusId, u8 Copy_u8PerifId )
{ 
    /* Input Limitation 0 --> 31 */
    if ( Copy_u8PerifId <= 31 )
	{
		switch ( Copy_u8BusId ) 
		{
			case RCC_AHB :   CLR_BIT(MRCC->AHBENR  ,Copy_u8PerifId);  break;
			case RCC_APB1:   CLR_BIT(MRCC->APB1ENR ,Copy_u8PerifId);  break;
			case RCC_APB2:   CLR_BIT(MRCC->APB2ENR ,Copy_u8PerifId);  break;
			
		}
	}
	
	else 
	{
		/* Return Error */
	}
} 
