#include "Serial.h"
#include "Parameters.h"
#include "Library/GPIO.h"

char* serialTransmitData = 0;
uint8_t serialTransmitCompleted = 0;
char serialReceivedCharacter = 0;
//uint32_t val;
uint8_t six_flag = 0;

void Serial_Init() {
	//Change the function of TX and RX pins for UART.
	uint32_t val = Serial_UART_RX_PIN;
	val |= (1);
	val &= ~(1<<1);
	val &= ~(1<<2);
	Serial_UART_RX_PIN = val;
	val = Serial_UART_TX_PIN;
	val |= (1);
	val &= ~(1<<1);
	val &= ~(1<<2);
	Serial_UART_TX_PIN = val;
	//Turn on UART0.
	PCONP |= 1<<3;
	//Enable FIFO for UART0.
	Serial_UART->FCR |= 1;
	//In order to change the DLM, DLL and FDR values, Write correct code for enabling the access to Divisor Latches.
	Serial_UART->LCR |= 1<<7;
	//Write correct DLM, DLL and FDR values for 9600 baudrate
	Serial_UART->DLM = 0x01;
	Serial_UART->DLL = 0x25;
	Serial_UART->FDR = 0x01 << 0 | 0x03 << 4;
	//Write correct code for disabling the access to Divisor Latches.
	Serial_UART->LCR &= ~(1<<7);
	//Change LCR register value for 8-bit character transfer, 1 stop bits and Even Parity.
	val = Serial_UART->LCR;
	val |= 1 | 1<<1;
	val &= ~(1<<2);
	val |= 1<<3 | 1<<4;
	val &= ~(1<<5);
	Serial_UART->LCR = val;
	//Enable the Receive Data Available and THRE Interrupt.
	Serial_UART->IER |= 1 | 1<<1;
	//Enable UART0_IRQn Interrupt.
	NVIC_EnableIRQ(UART0_IRQn);
	//Set UART0_IRQn Priority to 5.
	NVIC_SetPriority(UART0_IRQn, 6);
}

void UART0_IRQHandler() {	
	uint32_t currentInterrupt = ((Serial_UART->IIR & (0x7 << 1)) >> 1);
	
	//First if statement is for Receive Data Available interrupt.
	if(currentInterrupt == 0x2) {
		serialReceivedCharacter = Serial_ReadData();
		if(serialReceivedCharacter=='*'){
			START_MODE=0;
			stop_right();
			stop_left();
			FORWARD_FLAG=0;
			BACKWARD_FLAG=0;
			TURN_RIGHT_FLAG=0;
			TURN_LEFT_FLAG=0;
			six_flag=0;
			PRINT_MANUEL=1;
		}else if(serialReceivedCharacter=='#'){
			START_MODE=1;
			stop_left();
			stop_right();
			FORWARD_FLAG=0;
			BACKWARD_FLAG=0;
			TURN_RIGHT_FLAG=0;
			TURN_LEFT_FLAG=0;
			six_flag=0;
			PRINT_AUTO=1;
		}else if(serialReceivedCharacter=='6'){
			if(six_flag==1){
				FORWARD_FLAG=1;
				BACKWARD_FLAG=0;
				TURN_RIGHT_FLAG=0;
				TURN_LEFT_FLAG=0;
				six_flag=0;
			}
			six_flag=1;
		}else{
			six_flag=0;
		}
	}
	//Second if statement is for THRE interrupt
	else if(currentInterrupt == 0x1) {
		if(*serialTransmitData > 0) {
			Serial_WriteData(*serialTransmitData++);
		}
		else {
			serialTransmitCompleted = 1;
		}
	}
}

char Serial_ReadData() {
	return Serial_UART->RBR;
}

void Serial_WriteData(char data) {
	serialTransmitCompleted = 0;
	Serial_UART->THR = data;
}

void Serial_Write(char* data) {
	while(*data > 0)  {
		Serial_WriteData(*data++);
	}
}
