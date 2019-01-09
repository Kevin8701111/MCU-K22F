#ifndef uint32_t
	typedef __UINT32_TYPE__ uint32_t;
#endif
#ifndef uint8_t
	typedef __UINT8_TYPE__ uint8_t;
#endif

#include "pet.h"
//PTB0 PD_SCK
//PTE0	DOUT
long hx711_OFFSET = 0;
float hx711_SCALE = 1;
void Hx711init(void){
	
	*(uint32_t*)0x40048038 |= 	0x00003E01; //[SIM_SCGC5 ]  port ABCDE LPTMR
	// PCR
	*(uint32_t*)0x4004A000 =	0x00000100; //[PORTB_PCR0 ] GPIO PTB0
	*(uint32_t*)0x4004D000 =	0x00000102; //[PORTE_PCR0 ] pulldown
	// PDDR
	*(uint32_t*)0x400FF054 |=	0x00000001; //[GPIOB_PDDR ] PTB0 output

	// (Test LED)
	*(uint32_t*)0x400FF014 |= 	0x00000006; //[GPIOA_PDDR] PTA 1.2 output
	*(uint32_t*)0x400FF004 |= 	0x00000006; //[GPIOA_PSOR] PTA 1.2 set

	*(uint32_t*)0x40049004 = 	0x00000124; //[PORTA_PCR1] led red
	*(uint32_t*)0x40049008 = 	0x00000124; //[PORTA_PCR2] led green

}
uint8_t shiftIn(void) {
	uint8_t value = 0;
	uint8_t i;
	uint32_t input_temp;

	for (i = 0; i < 8; ++i) {
		*(uint32_t*)0x400FF044 |= 0x00000001;				//PTB0 (PD_SCK) H
		input_temp = *(uint32_t*)0x400FF110 & 0x00000001;	//只讀0x00000001的值	PTE0(DOUT)
		if( input_temp == 0x00000001 ){						//0x00000001位址為1	PTE0(DOUT)
			value |= 1 << (7 - i);
		}
		if( input_temp == 0x00000000){						//0x00000001位址為0	PTE0(DOUT)
			value |= 0 << (7 - i);
		}
		*(uint32_t*)0x400FF048 |= 0x00000001;				//PTB0 (PD_SCK) L

	}
	return value;
}
long hx711_read(){
	//uint32_t input_temp;
	//input_temp = *(uint32_t*)0x400FF110 & 0x00000001;	//只讀0x00000001的值	PTE0(DOUT)
	
	//if DOUT = LOW, HX711 ready for retrieval
	while( *(uint32_t*)0x400FF110 == 0x00000001 ){
		__asm("NOP");
	}

	unsigned long value = 0;
	uint8_t data[3] = { 0 };
	uint8_t filler = 0x00;

	data[2] = shiftIn();	// clock 24 times, read the data
	data[1] = shiftIn();
	data[0] = shiftIn();

	// PD_SCK 	25 ; channel A, gain factor 128
	// 			26 ; channel B, gain factor 32
	// 			27 ; channel A, gain factor 64
	for(int i = 0; i<1 ; i++){		//channel A, gain factor 128
		*(uint32_t*)0x400FF044 = 0x00000001;		// PTB0 (PD_SCK), HIGH
		*(uint32_t*)0x400FF048 = 0x00000001;		// PTB0 (PD_SCK), LOW
	}

	//32bit signed (filler)
	if (data[2] & 0x80) {	//呼叫記憶體位置
		filler = 0xFF;
	} else {
		filler = 0x00;
	}

	// Construct a 32-bit signed integer
	value = (	  (uint32_t)filler  << 24
				| (uint32_t)data[2] << 16
				| (uint32_t)data[1] << 8
				| (uint32_t)data[0] 		);

	return (long)value;

}
long hx711_read_average(int times) {		// read_average
	long sum = 0;
	for (int i = 0; i < times; i++) {
		sum += hx711_read();
		__asm("NOP");
	}
	return sum / times;
}
void hx711_set_scale(float scale) {			//set_scale
	hx711_SCALE = scale;
}
//void hx711_set_offset(long offset) {		//set_offset
//	hx711_OFFSET = offset;
//}
void hx711_tare(int times) {				//歸零
//	double
	long sum = hx711_read_average(times);
	hx711_OFFSET = sum;
}
//double hx711_get_value(int times) {			//get_value ; average - OFFSET
//	return hx711_read_average(times) - hx711_OFFSET;
//}
float hx711_get_units(int times) {			//get_units	; (average - OFFSET) / SCALE
	double value = ( hx711_read_average(times) - hx711_OFFSET ) / hx711_SCALE ;
	return value ;
}
void hx711_power_down() {
	*(uint32_t*)0x400FF044 = 0x00000001;		// PTB0 (PD_SCK), HIGH
	*(uint32_t*)0x400FF048 = 0x00000001;		// PTB0 (PD_SCK),
}
void hx711_power_up() {
	*(uint32_t*)0x400FF048 = 0x00000001;		// PTB0 (PD_SCK), LOW
}


