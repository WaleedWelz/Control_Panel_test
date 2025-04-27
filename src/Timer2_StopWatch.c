#include "stm32f10x.h"  // or your device-specific header

volatile u32 stopwatch_time = 0;  // Stopwatch time in milliseconds
volatile u32 overflow_count = 0;  // Keeps track of how many times the timer has overflowed

void TIM2_voidStart(void)
{
	TIM2->CNT = 0;
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_voidStop(void)
{
	stopwatch_time = overflow_count * 65536 + TIM2->CNT; // Add the number of overflows
	TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
}


// Timer Functions
void TIM2_voidInit(void)
{
	// Enable clock for TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Configure for 1ms resolution (72MHz / 7200 / 10)
	TIM2->PSC = 7200 - 1;     // 10kHz
	TIM2->ARR = 10 - 1;       // 1ms ticks
	TIM2->CNT = 0;
	TIM2->CR1 |= TIM_CR1_CEN;  // Enable timer

	TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt
	NVIC_EnableIRQ(TIM2_IRQn);  // Enable TIM2 interrupt in NVIC

}


u32 TIM2_u32GetTime(void)
{
	// Return current timer value in milliseconds
	 return overflow_count;  // Each overflow = 1ms
}

void TIM2_voidReset(void)
{
	TIM2->CNT = 0;
	overflow_count = 0;
}

// Optional: Timer overflow interrupt handler to keep track of overflows
void TIM2_IRQHandler(void)
{
	if (TIM2->SR & TIM_SR_UIF)  // Check for update interrupt flag
	{
		overflow_count++;  // Increment the overflow count
		TIM2->SR &= ~TIM_SR_UIF;  // Clear the interrupt flag
	}
}

