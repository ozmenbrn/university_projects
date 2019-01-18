#include "External.h"
#include "Parameters.h"
#include "PWM.h"

void External_Init() {
	//Change the functionality of the push button as EINT0
	uint32_t val = IOCON_PUSH_BUTTON;
	val |= 1;
	val &= ~(1<<1);
	val &= ~(1<<2);
	IOCON_PUSH_BUTTON = val;
	//Change the External interrupt mode as Edge Sensitive
	EXT->EXTMODE |= 1;
	//Enable interrupt for EINT0_IRQn.
	NVIC_EnableIRQ(EINT0_IRQn);
}

void EINT0_IRQHandler() {
	//Clear interrupt for EINT0
	// if in auto mode
	if(START_MODE){
		PWM_Write(80);
		FORWARD_FLAG=1;
		BACKWARD_FLAG=0;
		TURN_RIGHT_FLAG=0;
		TURN_LEFT_FLAG=0;
	}// if in manuel mode, switch to auto
	else if(!START_MODE){
		START_MODE=1;
	}
	EXT->EXTINT |= 1;
	NVIC_ClearPendingIRQ(EINT0_IRQn);
}
