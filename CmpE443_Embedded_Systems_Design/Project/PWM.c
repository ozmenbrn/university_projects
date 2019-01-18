#include "PWM.h"

void PWM_Init() {
	PCONP |= 1 << 5;
	//Change the function of the pin in here:
	IOCON_MOTOR_SPEED |= 3;
	
	//Enable PWM output for corresponding pin.
	PWM0->PCR |= 1<<10;
	//Reset The PWM Timer Counter and The PWM Prescale Counter on the Next Positive Edge of PCLK
	PWM0->TCR |= 1<<1;
	
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * 20 * 1000;
	
	//Reset TC, when MR0 matches TC.
	PWM0->MCR |= 1<<1;
	
	//Enable PWM Match 0 Latch.
	PWM0->LER |= 1;
	
	//Enable Counter and PWM and Clear Reset on the PWM
	uint32_t val = PWM0->TCR;
	val |= 1;
	val &= ~(1<<1);
	val |= 1<<3;
	PWM0->TCR = val;
	
	PWM_Write(0);
}

void PWM_Cycle_Rate(uint32_t period_In_Cycles) {
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * period_In_Cycles * 1000;
	
	//Enable PWM Match 0 Latch.
	PWM0->LER |= 1;
}

void PWM_Write(uint32_t T_ON) {	
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	T_ON = (uint32_t)(((PWM0->MR0) * T_ON) / 100);
	
	if (T_ON == PWM0->MR0) {
		T_ON++;
	}
	PWM0->MR2 = T_ON;
	
	//Enable PWM Match Register Latch.
	
	PWM0->LER |= (1<<2);
}
