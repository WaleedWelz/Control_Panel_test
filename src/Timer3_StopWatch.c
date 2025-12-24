#include "stm32f10x.h"  // or your device-specific header

volatile u32 timer3_overflow_count = 0;  // Overflow counter

// Initialize TIM3 for 1ms resolution (72MHz / 7200 / 10)
void TIM3_Init(void) {
    // 1. Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // 2. Configure for 1ms ticks (72MHz / 7200 / 10)
    TIM3->PSC = 7200 - 1;    // 72MHz / 7200 = 10kHz
    TIM3->ARR = 10 - 1;      // 10kHz / 10 = 1ms
    TIM3->CNT = 0;           // Reset counter

    // 3. Enable update interrupt (for overflow tracking)
    TIM3->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM3_IRQn);  // Enable TIM3 IRQ in NVIC

    // 4. Start timer (optional)
 //   TIM3->CR1 |= TIM_CR1_CEN;
}

// Start TIM3
void TIM3_Start(void) {
    TIM3->CNT = 0;                   // Reset counter
    timer3_overflow_count = 0;       // Reset overflow
    TIM3->CR1 |= TIM_CR1_CEN;        // Enable timer
}

// Stop TIM3 and return total time (ms)
void TIM3_Stop(void) {
    TIM3->CR1 &= ~TIM_CR1_CEN;       // Disable timer
}

// Reset TIM3 counter and overflow
void TIM3_Reset(void) {
    TIM3->CNT = 0;
    timer3_overflow_count = 0;
}

// Get current time in milliseconds
u32 TIM3_GetTime(void) {
    return timer3_overflow_count;     // Each overflow = 1ms
}

// TIM3 overflow interrupt handler
void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {     // Check overflow flag
        timer3_overflow_count++;      // Increment overflow count
        TIM3->SR &= ~TIM_SR_UIF;      // Clear flag
    }
}

void TIM3_Delay(u32 ms) {
    u32 start_time = TIM3_GetTime();  // Get current time
    while ((TIM3_GetTime() - start_time) < ms) {
        // Block until delay completes
    }
}
