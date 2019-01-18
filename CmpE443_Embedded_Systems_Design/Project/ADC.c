#include "ADC.h"

#define	ANALOG_LDRL_IOCON_ADDRESS	0x4002C064
#define ANALOG_LDRL_IOCON	*((volatile uint32_t*)(ANALOG_LDRL_IOCON_ADDRESS))

#define	ANALOG_LDRR_IOCON_ADDRESS	0x4002C068
#define ANALOG_LDRR_IOCON	*((volatile uint32_t*)(ANALOG_LDRR_IOCON_ADDRESS))

uint32_t ADC_Last = 0;
uint8_t ADC_New_Data_Available = 0;

void ADC_Init() {
	//Change the function value of pin to ADC.
	uint32_t val=ANALOG_TRIM_IOCON;
	val &= ~(1);
	val &= ~(1<<1);
	val |= 1<<2;
	ANALOG_TRIM_IOCON=val;
	val=ANALOG_LDRL_IOCON;
	val &= ~(1);
	val &= ~(1<<1);
	val |= 1<<2;
	ANALOG_LDRL_IOCON=val;
	val=ANALOG_LDRR_IOCON;
	val &= ~(1);
	val &= ~(1<<1);
	val |= 1<<2;
	ANALOG_LDRR_IOCON=val;
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	ANALOG_TRIM_IOCON &= ~(1<<3) & ~(1<<4);
	ANALOG_LDRL_IOCON &= ~(1<<3) & ~(1<<4);
	ANALOG_LDRR_IOCON &= ~(1<<3) & ~(1<<4);
	//Change Analog/Digital mode of pin to Analog.
	ANALOG_TRIM_IOCON &= ~(1<<7);
	ANALOG_LDRL_IOCON &= ~(1<<7);
	ANALOG_LDRR_IOCON &= ~(1<<7);
	//Turn on ADC.
	PCONP |= 1<<12;
	//Set the CLKDIV and make the A/D converter operational without Burst mode.
	ADC->CR|= 1<<21;
	val=ADC->CR;
	val&=~(1<<16);
	val|=59<<8;
	ADC->CR=val;
}

void ADC_Start() {
	//Write a code for starting A/D conversion
	uint32_t val = ADC->CR;
	val &= ~(1<<26) & ~(1<<25);
	val |= 1<<24;
	ADC->CR = val;
}

void ADC_Stop() {
	//Write a code for stopping A/D conversion
	ADC->CR &= ~(1<<24) & ~(1<<25) & ~(1<<26);
}

uint32_t ADC_Trim_Read() {
	uint32_t data;
	
	//Configure CR SEL bits for sampled and converting corresponding pin
	ADC->CR |= 1;
	ADC->CR &= ~(1<<2) & ~(1<<3);
	ADC_Start();
	
	//Wait until A/D conversion completed
	while((ADC->STAT&0xFFF)==0);
	//Convert the data RESULT to 0 - 1000 range and return the ADC data
	data=ADC->DR[0];
	data=data>>4 & 4095;
	data=data/40;
	ADC_Stop();
	return data;
}

uint32_t ADC_LDRL_Read() {
	
	uint32_t data;
	
	//Configure CR SEL bits for sampled and converting corresponding pin
	ADC->CR |= 1<<2;
	ADC->CR &= ~(1) & ~(1<<3);
	ADC_Start();
	
	//Wait until A/D conversion completed
	while((ADC->STAT>>2&0xFFF)==0);
	//Convert the data RESULT to 0 - 1000 range and return the ADC data
	data=ADC->DR[2];
	data=data>>4 & 4095;
	ADC_Stop();
	return data;
}

uint32_t ADC_LDRR_Read() {
	uint32_t data;
	
	//Configure CR SEL bits for sampled and converting corresponding pin
	ADC->CR |= 1<<3;
	ADC->CR &= ~(1) & ~(1<<2);
	ADC_Start();
	
	//Wait until A/D conversion completed
	while((ADC->STAT>>3&0xFFF)==0);
	//Convert the data RESULT to 0 - 1000 range and return the ADC data
	data=ADC->DR[3];
	data=data>>4 & 4095;
	ADC_Stop();
	return data;
}

