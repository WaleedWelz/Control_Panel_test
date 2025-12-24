#ifndef TIMER3_STOPWATCH_H
#define TIMER3_STOPWATCH_H

void TIM3_Init(void);          // Initialize TIM3 for 1ms ticks
void TIM3_Start(void);         // Start the timer
void TIM3_Stop(void);          // Stop and capture time
void TIM3_Reset(void);         // Reset counter and overflow
u32  TIM3_GetTime(void);   // Get current time in ms
void TIM3_Delay(u32 ms);
#endif
