#include "GPIO.h"

uint32_t val;

void GPIO_DIR_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->DIR &= ~MASK;
	}
	else {
		PORT->DIR |= MASK;
	}
}

void GPIO_PIN_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->PIN &= ~MASK;
	}
	else {
		PORT->PIN |= MASK;
	}
}

uint32_t GPIO_PIN_Read(GPIO_TypeDef* PORT,uint32_t MASK) {
	return PORT->PIN &= MASK;
}

void in1_in2_init(){
	LPC_GPIO0->DIR |= (1<<8) | (1<<7);
	
	//in2
	LPC_IOCON->P0_8 &= ~(0x3 << 3);
	LPC_IOCON->P0_8 |= (0 & 0x3) << 3;
	//in1
	LPC_IOCON->P0_7 &= ~(0x3 << 3);
	LPC_IOCON->P0_7 |= (0 & 0x3) << 3;
}

void in3_in4_init(){
	LPC_GPIO1->DIR |= (1<<24) | (1<<23);
	
	//in4 1.23
	LPC_IOCON->P1_23 &= ~(0x3 << 3);
	LPC_IOCON->P1_23 |= (0 & 0x3) << 3;
	//in3 1.24
	LPC_IOCON->P1_24 &= ~(0x3 << 3);
	LPC_IOCON->P1_24 |= (0 & 0x3) << 3;
}

void stop_left(){
	val=LPC_GPIO0->PIN;
	val |= 1<<8;
	val |= (1<<7);
	LPC_GPIO0->PIN = val;
}

void stop_right(){
	val=LPC_GPIO1->PIN;
	val |= 1<<23;
	val |= (1<<24);
	LPC_GPIO1->PIN = val;
}
	
void forward_right(){
	val=LPC_GPIO1->PIN;
	val |= 1<<23;
	val &= ~(1<<24);
	LPC_GPIO1->PIN = val;
}

void backward_right(){
	val=LPC_GPIO1->PIN;
	val |= 1<<24;
	val &= ~(1<<23);
	LPC_GPIO1->PIN = val;
}

void forward_left(){
	val=LPC_GPIO0->PIN;
	val |= 1<<8;
	val &= ~(1<<7);
	LPC_GPIO0->PIN = val;
}
	
void backward_left(){
	val=LPC_GPIO0->PIN;
	val |= 1<<7;
	val &= ~(1<<8);
	LPC_GPIO0->PIN = val;
}
	