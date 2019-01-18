#include "Timer.h"

uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint32_t capturedTime = 0;

void Timer_Init() {
	//Give the Correct Function Values to IOCON_OUT
	IOCON_OUT |= 3;
	//Enable Timer3
	PCONP |= 1<<23;
	//Change the mode of Timer3 to Timer Mode.
	uint32_t val = TIMER3->CTCR;
	val |= (1);
	val &= ~(1<<1);
	val |= 1<<2;
	val &= ~(1<<3);
	TIMER3->CTCR = val;
	//Disable Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR &= ~(1);
	//Reset Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR |= 1<<1;
	//Change PR Register value for 1 microsecond incrementing
	TIMER3->PR = 59;
	//Capture Rising and Falling Edge on CAP1
	val = TIMER3->CCR;
	val &= ~(1<<3);
	val &= ~(1<<4);
	val &= ~(1<<5);
	TIMER3->CCR |= val;
	//Remove the reset on counters of Timer3.
	TIMER3->TCR &= ~(1<<1);
	//Enable Timer3 Counter and Prescale Counter for counting.	
	TIMER3->TCR |= 1;
}
