#include "LPC407x_8x_177x_8x.h"
#include "Library/GPIO.h"
#include "PWM.h"
#include "Library/LED.h"
#include "Library/SystemStructures.h"
#include "Library/Joystick.h"
#include "Parameters.h"
#include "Library/Timer.h"
#include "Ultrasonic.h"
#include "ADC.h"
#include "Serial.h"
#include "External.h"
#include "ESP8266.h"

uint32_t v;
uint32_t speed;
int32_t LDR_left;
int32_t LDR_right;
int32_t LDR_diff;
int8_t threshold=100;

void init() {
	LED_Init();
	in1_in2_init();
	in3_in4_init();
	ADC_Init();
	
	PWM_Init();
	//PWM_Write(ROBOT_SPEED);
	PWM_Cycle_Rate(20);
	
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	Ulrasonic_Start_Trigger();
	
	Serial_Init();
	External_Init();
	
	/*ESP8266_Init();
	ESP8266_sendCommand("AT+CWJAP=\"HWLAB\",\"12345678\"\r\n");
	ESP8266_waitResponseEnd();
	ESP8266_sendCommand("AT+CIFSR\r\n");
	ESP8266_waitResponseEnd();
	char *response=ESP8266_readResponse();
	Serial_WriteData(*response++);
	while(!serialTransmitCompleted){};*/
}

int millisecond = 250;
//When the time between rising - falling or falling - rising edge is less than 100 millisecond and greater than 5 millisecond, turn on the LED on the Quick Start Board.
//Otherwise turn off the LED on the Quick Start Board.
void update() {
	
	ultrasonicSensorDuration=ultrasonicSensorFallingTime-ultrasonicSensorRisingTime;
	ultrasonicSensorDistance=ultrasonicSensorDuration/58;
	
	speed=ADC_Trim_Read();
	PWM_Write(speed*5);
	LDR_left=ADC_LDRL_Read();
	LDR_right=ADC_LDRR_Read();
	 
	//LDR_diff=LDR_left-LDR_right;
	
	if(PRINT_AUTO){
		serialTransmitData = "AUTO"; 
		Serial_WriteData(*serialTransmitData++);
		while(!serialTransmitCompleted){};
		PRINT_AUTO=0;
	}
  else if(PRINT_MANUEL){
		serialTransmitData = "MANUEL"; 
		Serial_WriteData(*serialTransmitData++);
		while(!serialTransmitCompleted){};
		PRINT_MANUEL=0;
	}
	
	// manuel mode
	if(!START_MODE){
		if(Joystick_Left_Pressed()){
			TURN_LEFT_FLAG=1;
			TURN_RIGHT_FLAG=0;
			FORWARD_FLAG=0;
			BACKWARD_FLAG=0;
		}
		if(Joystick_Right_Pressed()){
			TURN_LEFT_FLAG=0;
			TURN_RIGHT_FLAG=1;
			FORWARD_FLAG=0;
			BACKWARD_FLAG=0;
		}
		if(Joystick_Up_Pressed()){
			TURN_LEFT_FLAG=0;
			TURN_RIGHT_FLAG=0;
			FORWARD_FLAG=1;
			BACKWARD_FLAG=0;
		}
		if(Joystick_Down_Pressed()){
			TURN_LEFT_FLAG=0;
			TURN_RIGHT_FLAG=0;
			FORWARD_FLAG=0;
			BACKWARD_FLAG=1;
		}
		if(Joystick_Center_Pressed()){
			TURN_LEFT_FLAG=0;
			TURN_RIGHT_FLAG=0;
			FORWARD_FLAG=0;
			BACKWARD_FLAG=0;
		}
		
		if(FORWARD_FLAG){
				if(LDR_left-LDR_right>-threshold && LDR_left-LDR_right<threshold){
				
				Front_Left_On();
				Front_Right_On();
				Back_Right_Off();
				Back_Left_Off();
				forward_left();
				forward_right();
				if(ultrasonicSensorDistance<=OBSTACLE_DISTANCE){
					Front_Left_Off();
					Front_Right_Off();
					Back_Right_On();
					Back_Left_On();	
					while(ultrasonicSensorDistance<OBSTACLE_ESCAPE_DISTANCE || ultrasonicSensorDistance>200){	

						backward_left();
						backward_right();
						
						ultrasonicSensorDuration=ultrasonicSensorFallingTime-ultrasonicSensorRisingTime;
						ultrasonicSensorDistance=ultrasonicSensorDuration/58;
					}
					Front_Left_On();
					Front_Right_On();
					Back_Right_Off();
					Back_Left_Off();
					forward_right();
					forward_left();
				}
		}
		if(LDR_left-LDR_right<-threshold){
				Front_Left_On();
				Front_Right_Off();
				Back_Right_Off();
				Back_Left_On();		
				
					forward_right();
					backward_left();
					for(int i=0;i<millisecond*12000;i++);
					Front_Left_Off();
					Back_Left_Off();
					for(int i=0;i<millisecond*12000;i++);
					Front_Left_On();
					Back_Left_On();
				
				Front_Left_Off();
				Back_Left_Off();
				
		}
		if(LDR_left-LDR_right>threshold){
			Front_Left_Off();
			Front_Right_On();
			Back_Right_On();
			Back_Left_Off();	
			
				forward_left();
			  backward_right();
				for(int i=0;i<millisecond*12000;i++);
				Front_Right_Off();
				Back_Right_Off();
				for(int i=0;i<millisecond*12000;i++);
				Front_Right_On();
				Back_Right_On();
			
			Front_Right_Off();
			Back_Right_Off();
			
		}
		}
		else if(TURN_LEFT_FLAG){
			Front_Left_On();
			Front_Right_Off();
			Back_Right_Off();
			Back_Left_On();		
			
				forward_right();
				backward_left();
				for(int i=0;i<millisecond*12000;i++);
				Front_Left_Off();
				Back_Left_Off();
				for(int i=0;i<millisecond*12000;i++);
				Front_Left_On();
				Back_Left_On();
			
			Front_Left_Off();
			Back_Left_Off();
			TURN_LEFT_FLAG=1;
			TURN_RIGHT_FLAG=0;
			FORWARD_FLAG=0;
			BACKWARD_FLAG=0;
		}
		else if(TURN_RIGHT_FLAG){
			Front_Left_Off();
			Front_Right_On();
			Back_Right_On();
			Back_Left_Off();	
			
				forward_left();
			  backward_right();
				for(int i=0;i<millisecond*12000;i++);
				Front_Right_Off();
				Back_Right_Off();
				for(int i=0;i<millisecond*12000;i++);
				Front_Right_On();
				Back_Right_On();
			
			Front_Right_Off();
			Back_Right_Off();
			TURN_LEFT_FLAG=0;
			TURN_RIGHT_FLAG=1;
			FORWARD_FLAG=0;
			BACKWARD_FLAG=0;
			
		}
		else if(BACKWARD_FLAG){
			Front_Left_Off();
			Front_Right_Off();
			Back_Right_On();
			Back_Left_On();	
			backward_left();
			backward_right();
			TURN_LEFT_FLAG=0;
			TURN_RIGHT_FLAG=0;
			FORWARD_FLAG=0;
			BACKWARD_FLAG=1;
			
		}
		else{
			Front_Left_Off();
			Front_Right_Off();
			Back_Left_Off();
			Back_Right_Off();
			
			stop_left();
			stop_right();
		}
	}
	// autonomous mode
	else{
		//PWM_Write(80);
		if(Joystick_Up_Pressed()){
			FORWARD_FLAG=1;
			BACKWARD_FLAG=0;
			TURN_LEFT_FLAG=0;
			TURN_RIGHT_FLAG=0;
		}
		if(LDR_left-LDR_right>-threshold && LDR_left-LDR_right<threshold){
				FORWARD_FLAG=1;
				BACKWARD_FLAG=0;
				TURN_LEFT_FLAG=0;
				TURN_RIGHT_FLAG=0;
				Front_Left_On();
				Front_Right_On();
				Back_Right_Off();
				Back_Left_Off();
				forward_left();
				forward_right();
				if(ultrasonicSensorDistance<=OBSTACLE_DISTANCE){
					Front_Left_Off();
					Front_Right_Off();
					Back_Right_On();
					Back_Left_On();	
					while(ultrasonicSensorDistance<OBSTACLE_ESCAPE_DISTANCE || ultrasonicSensorDistance>200){	

						backward_left();
						backward_right();
						
						ultrasonicSensorDuration=ultrasonicSensorFallingTime-ultrasonicSensorRisingTime;
						ultrasonicSensorDistance=ultrasonicSensorDuration/58;
					}
					Front_Left_On();
					Front_Right_On();
					Back_Right_Off();
					Back_Left_Off();
					forward_right();
					forward_left();
				}
		}
		if(LDR_left-LDR_right<-threshold){
				Front_Left_On();
				Front_Right_Off();
				Back_Right_Off();
				Back_Left_On();		
				
					forward_right();
					backward_left();
					for(int i=0;i<millisecond*12000;i++);
					Front_Left_Off();
					Back_Left_Off();
					for(int i=0;i<millisecond*12000;i++);
					Front_Left_On();
					Back_Left_On();
				
				Front_Left_Off();
				Back_Left_Off();
				TURN_LEFT_FLAG=1;
				TURN_RIGHT_FLAG=0;
				FORWARD_FLAG=0;
				BACKWARD_FLAG=0;
		}
		if(LDR_left-LDR_right>threshold){
			Front_Left_Off();
			Front_Right_On();
			Back_Right_On();
			Back_Left_Off();	
			
				forward_left();
			  backward_right();
				for(int i=0;i<millisecond*12000;i++);
				Front_Right_Off();
				Back_Right_Off();
				for(int i=0;i<millisecond*12000;i++);
				Front_Right_On();
				Back_Right_On();
			
			Front_Right_Off();
			Back_Right_Off();
			TURN_LEFT_FLAG=0;
			TURN_RIGHT_FLAG=1;
			FORWARD_FLAG=0;
			BACKWARD_FLAG=0;
		}
				
	}
	
}

int main() {
    init();
		
		TURN_LEFT_FLAG=0;
		TURN_RIGHT_FLAG=0;
		FORWARD_FLAG=0;
		BACKWARD_FLAG=0;
		START_MODE=0;
    while(1) {
			ADC_Start();
			
        update();
    }
}
