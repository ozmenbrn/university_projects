#include "Light.h"
#include "SystemStructures.h"

#include "LPC407x_8x_177x_8x.h"

void Light_init(){
	
/*	uint32_t val = PCONP;
	val |= 1<<15;
	PCONP = val;
	
	FRONT_LEFT = 0;
	FRONT_RIGHT = 0;
	BACK_LEFT = 0;
	BACK_RIGHT = 0;
	
	val = LPC_GPIO0->DIR;
	val |= 1<<23;
	val |= 1<<24;
	val |= 1<<25;
	val |= 1<<26;
	
	LPC_GPIO0->DIR = val;*/
}

void Front_Left_On(){
	LPC_GPIO0->PIN |= 1<<23;
}

void Front_Left_Off(){
	LPC_GPIO0->PIN &= ~(1<<23);
}

void Front_Right_On(){
	LPC_GPIO0->PIN |= (1<<24);
}

void Front_Right_Off(){
	LPC_GPIO0->PIN &= ~(1<<24);
}

void Back_Left_On(){
	LPC_GPIO0->PIN |= (1<<25);
}
void Back_Left_Off(){
	LPC_GPIO0->PIN &= ~(1<<25);
}

void Back_Right_On(){
	LPC_GPIO0->PIN |= (1<<26);
}
void Back_Right_Off(){
	LPC_GPIO0->PIN &= ~(1<<26);
}