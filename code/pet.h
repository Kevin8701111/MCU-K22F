#ifndef __pet_h__
#define __pet_h__
// --- timer --- ( Timer.c )  

void timerinit(void);				//temer register init
unsigned long timer_overflow_us;	//timer overflow count
unsigned long current_time(void);	//current_time(); get timer_overflow_us
void delay(int delay_number);		//delay(1) = 100us
									//delay(10000) = 1s
void delayNOP(int delay_number);	//__asm("NOP")

// --- Keypad --- ( FT_Keypad.c )
void Keypadinit(void);				//Keypad register init 
char KeypadGetkey(void);			//Keypad_Getkey  return char
char Keypadscan(void);				//keypad scan

// --- Hx711 --- ( FT_Hx711.c )

void Hx711init(void);					//Hx711 register init
uint8_t shiftIn(void);				
long hx711_read();
long hx711_read_average(int times);		// read_average	
void hx711_set_scale(float scale);
void hx711_tare(int times);
float hx711_get_units(int times);		//get_units	; (average - OFFSET) / SCALE		 
void hx711_power_down();
void hx711_power_up();
								
// --- StepperMotor --- ( FT_Motor.c )
void motorinit(void);				//StepperMotor register init

struct StepMotor{
	int step_number ;	//
	int direction 	;	//
	int motor_steps ;	//
						//(28BYJ-48 5V) 5.625 /64, Speed Variation Ratio:1/64, 64*64=4096
	unsigned long last_step_time; //
	uint32_t motorpin1;
	uint32_t motorpin2;
	uint32_t motorpin3;
	uint32_t motorpin4;
};
struct StepMotor Motor1 ;	//motor pins
struct StepMotor Motor2 ;	
		/* Motor_1	(28BYJ-48 5V) K22F
			PTC5	B'	blue			0x00000020
			PTC10	A'	pink			0x00000400
			PTC7	B	yellow			0x00000080
			PTC9	A	orange			0x00000200	*/
		/* Motor_2	(28BYJ-48 5V) K22F
			PTC6	B'	blue			0x00000040
			PTC3	A'	pink			0x00000008
			PTC1	B	yellow			0x00000002
			PTC2	A	orange			0x00000004  */

void MotorStep(struct StepMotor *motor, int steps );
									//MotorStep(motor,steps)
									//motor : Motor1 or Motor2
									//steps : direction+ / -
									//ex. MotorStep(&Motor1,2048);   motor1 step +2048
									//	  MotorStep(&motor2,-2048);  motor2 strp -2048
#endif
