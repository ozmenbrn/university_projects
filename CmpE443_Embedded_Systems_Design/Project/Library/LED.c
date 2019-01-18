#include "LED.h"

void LED_Init() {
	
	LPC_SC->PCONP |= 1<<15 ;
	
	//LPC_GPIO2->DIR |= 1 << 10;
	LPC_GPIO1->DIR |= (1 << 12) | (1 <<11) | (1<<7) | (1<<6);
	//LPC_GPIO1->DIR |= 1 << 11;
	//LPC_GPIO1->DIR |= 1 << 7;
	
	LPC_IOCON->P1_6 &= ~(0x3 << 3);
	LPC_IOCON->P1_6 |= (0 & 0x3) << 3;
	
	LPC_IOCON->P1_12 &= ~(0x3 << 3);
	LPC_IOCON->P1_12 |= (0 & 0x3) << 3;
	
	LPC_IOCON->P1_11 &= ~(0x3 << 3);
	LPC_IOCON->P1_11 |= (0 & 0x3) << 3;
	
	LPC_IOCON->P1_7 &= ~(0x3 << 3);
	LPC_IOCON->P1_7 |= (0 & 0x3) << 3;
}


void LED_ON() {
	LPC_GPIO1->CLR = 1 << 18;
	LPC_GPIO0->CLR = 1 << 13;
	LPC_GPIO1->SET = 1 << 13;
	LPC_GPIO2->SET = 1 << 19;
}

void LED_OFF() {
	LPC_GPIO1->SET = 1 << 18;
	LPC_GPIO0->SET = 1 << 13;
	LPC_GPIO1->CLR = 1 << 13;
	LPC_GPIO2->CLR = 1 << 19;
}

void Front_Left_On(){
	LPC_GPIO1->PIN |= 1<<6;
}

void Front_Left_Off(){
	LPC_GPIO1->PIN &= ~(1<<6);
}

void Front_Right_On(){
	LPC_GPIO1->PIN |= (1<<12);
}

void Front_Right_Off(){
	LPC_GPIO1->PIN &= ~(1<<12);
}

void Back_Left_On(){
	LPC_GPIO1->PIN |= (1<<11);
}
void Back_Left_Off(){
	LPC_GPIO1->PIN &= ~(1<<11);
}

void Back_Right_On(){
	LPC_GPIO1->PIN |= (1<<7);
}
void Back_Right_Off(){
	LPC_GPIO1->PIN &= ~(1<<7);
}
