#ifndef uint32_t
	typedef __UINT32_TYPE__ uint32_t;
#endif
#ifndef uint8_t
	typedef __UINT8_TYPE__ uint8_t;
#endif

#include "pet.h"

struct StepMotor Motor1 = {
	.step_number = 0,				//當前步數
	.direction   = 0,				//轉動方向
	.motor_steps = 4096,			//一圈步數
	.last_step_time = 0,			//最後執行時間
	.motorpin1	 = 0x00000020,		//PTC5	B'	blue
	.motorpin2	 = 0x00000400,		//PTC10	A'	pink
	.motorpin3	 = 0x00000080,		//PTC7	B	yellow
	.motorpin4	 = 0x00000200		//PTC9	A	orange
};
struct StepMotor Motor2 = {
	.step_number = 0,
	.direction   = 0,
	.motor_steps = 4096,
	.last_step_time = 0,			
	.motorpin1	 = 0x00000040,		//PTC6	B'	blue
	.motorpin2	 = 0x00000008,		//PTC3	A'	pink
	.motorpin3	 = 0x00000002,		//PTC1	B	yellow
	.motorpin4	 = 0x00000004		//PTC2	A	orange
};


void motorinit(void){
// --- Port Clock           --- //
	*(uint32_t*)0x40048038 |= 	0x00003E01; //[SIM_SCGC5]  port ABCDE LPTMR

// --- Stepper GPIO Setup  --- //

	*(uint32_t*)0x400FF094 |=	0x000006EE; //[GPIOC_PDDR ] PTC 1,2,3,5,6,7,9,10 output

	*(uint32_t*)0x4004B004 =	0x00000140;	//[PORTC_PCR1 ]	Pin Mux Control GPIO, hihg drive strength
	*(uint32_t*)0x4004B008 =	0x00000140;	//[PORTC_PCR2 ]
	*(uint32_t*)0x4004B00C =	0x00000140;	//[PORTC_PCR3 ]
	*(uint32_t*)0x4004B018 =	0x00000140;	//[PORTC_PCR6 ]

	*(uint32_t*)0x4004B014 =	0x00000140;	//[PORTC_PCR5 ]
	*(uint32_t*)0x4004B01C =	0x00000140; //[PORTC_PCR7 ]
	*(uint32_t*)0x4004B024 =	0x00000140;	//[PORTC_PCR9 ]
	*(uint32_t*)0x4004B028 =	0x00000140;	//[PORTC_PCR10]

}

void MotorStep(struct StepMotor *motor, int steps )
{

	int steps_left = abs(steps);				//取絕對值
	if (steps > 0) { motor->direction = 1; }	//正轉或反轉
	if (steps < 0) { motor->direction = 0; }
/*
// 固定時間執行
int steps_left = 2048
void MotorStep(struct StepMotor *motor, int direction){
	unsigned long now = timer_overflow_us;
	if((steps_left > 0) && (( now - motor->last_step_time ) >= 20)){
		motor->last_step_time = now ;
//  	...
	}
void MotorReset(void){	//重置步數
	steps_left = 2048;
}	
*/
	while (steps_left > 0)
	{
		unsigned long now = timer_overflow_us;	//timer_overflow_us ; timer overflow count
		if(( now - motor->last_step_time ) >= 20 ){		// 1 : 100us
			motor->last_step_time = now ;		//取得執行時間

			if (motor->direction == 1)				//正轉
			{
				motor->step_number++;
				if (motor->step_number == motor->motor_steps) {	//轉完一圈
					motor->step_number = 0;
				}
			}
			else //(direction == 0)					//反轉
			{
				if (motor->step_number == 0) {				//轉完一圈
					motor->step_number = motor->motor_steps;
				}
				motor->step_number--;
			}
			steps_left--;									//減少執行次數
			int step_motor = (motor->step_number % 4);		//目前步數取餘數
			switch (step_motor) 							//相位激磁
			{
			case 0:  // Color:OYPB 1000
				*(uint32_t*)0x400FF084 |=	motor->motorpin4;	//GPIOC_PSOR
				*(uint32_t*)0x400FF088 |=	motor->motorpin3;	//GPIOC_PCOR
				*(uint32_t*)0x400FF088 |=	motor->motorpin2;
				*(uint32_t*)0x400FF088 |=	motor->motorpin1;
			break;/*
			case 0:  // Color:OYPB 1100
				*(uint32_t*)0x400FF084 |=	motor->motorpin4;
				*(uint32_t*)0x400FF084 |=	motor->motorpin3;
				*(uint32_t*)0x400FF088 |=	motor->motorpin2;
				*(uint32_t*)0x400FF088 |=	motor->motorpin1;
			break;*/
			case 1:  // Color:OYPB 0100
				*(uint32_t*)0x400FF088 |=	motor->motorpin4;
				*(uint32_t*)0x400FF084 |=	motor->motorpin3;
				*(uint32_t*)0x400FF088 |=	motor->motorpin2;
				*(uint32_t*)0x400FF088 |=	motor->motorpin1;
			break;/*
			case 1:  // Color:OYPB 0110
				*(uint32_t*)0x400FF088 |=	motor->motorpin4;
				*(uint32_t*)0x400FF084 |=	motor->motorpin3;
				*(uint32_t*)0x400FF084 |=	motor->motorpin2;
				*(uint32_t*)0x400FF088 |=	motor->motorpin1;
			break;*/
			case 2: // Color:OYPB 0010
				*(uint32_t*)0x400FF088 |=	motor->motorpin4;
				*(uint32_t*)0x400FF088 |=	motor->motorpin3;
				*(uint32_t*)0x400FF084 |=	motor->motorpin2;
				*(uint32_t*)0x400FF088 |=	motor->motorpin1;
			break;/*
			case 2: // Color:OYPB 0011
				*(uint32_t*)0x400FF088 |=	motor->motorpin4;
				*(uint32_t*)0x400FF088 |=	motor->motorpin3;
				*(uint32_t*)0x400FF084 |=	motor->motorpin2;
				*(uint32_t*)0x400FF084 |=	motor->motorpin1;
			break;*/
			case 3: // Color:OYPB 0001
				*(uint32_t*)0x400FF088 |=	motor->motorpin4;
				*(uint32_t*)0x400FF088 |=	motor->motorpin3;
				*(uint32_t*)0x400FF088 |=	motor->motorpin2;
				*(uint32_t*)0x400FF084 |=	motor->motorpin1;
			break;/*
			case 3: // Color:OYPB 1001
				*(uint32_t*)0x400FF084 |=	motor->motorpin4;
				*(uint32_t*)0x400FF088 |=	motor->motorpin3;
				*(uint32_t*)0x400FF088 |=	motor->motorpin2;
				*(uint32_t*)0x400FF084 |=	motor->motorpin1;
			break;*/
			}
		}
	}
}
