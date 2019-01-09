#ifndef uint32_t
	typedef __UINT32_TYPE__ uint32_t;
#endif
#ifndef uint8_t
	typedef __UINT8_TYPE__ uint8_t;
#endif

#include "pet.h"
int setup(void){
// --- Port Clock           --- //
	*(uint32_t*)0x40048038 = 	0x00003E01; //[SIM_SCGC5]  port ABCDE LPTMR
// --- GPIO Setup  --- //

	// (Test LED)
	*(uint32_t*)0x400FF014 |= 	0x00000006; //[GPIOA_PDDR] PTA 1.2 output
	*(uint32_t*)0x400FF004 |= 	0x00000006; //[GPIOA_PSOR] PTA 1.2 set

	*(uint32_t*)0x40049004 = 	0x00000124; //[PORTA_PCR1] led red
	*(uint32_t*)0x40049008 = 	0x00000124; //[PORTA_PCR2] led green

	return 0;
}
int main(void)
{
	setup();
	timerinit();
	Hx711init();
	Keypadinit();
	motorinit();


	hx711_set_scale(373.4f);
	//	hx711_tare(10);
	//	hx711_read();

//	--- shiftIn test 8b ---
/*
	uint8_t testtmp = shiftIn();
	if(testtmp == 0xFF){
		*(uint32_t*)0x400FF00C = 0x00000002;
	}
*/
//	--- shiftIn test 32b ---

	unsigned long testvalue = 0;
	uint8_t testtmp[3] = { 0 };
	uint8_t filler = 0x00;

	testtmp[2] = shiftIn();
	testtmp[1] = shiftIn();
	testtmp[0] = shiftIn();

	testvalue = ( (uint32_t)filler 	   << 24
				| (uint32_t)testtmp[2] << 16
				| (uint32_t)testtmp[1] << 8
				| (uint32_t)testtmp[0] );

	if(testvalue == 0x00FFFFFF){
		*(uint32_t*)0x400FF00C = 0x00000002;
	}



/*
while(1){
	if(hx711_get_units(10) <= 10){
		*(uint32_t*)0x400FF00C = 0x00000002;	//LED T
	}
	for(int i=0; i<500000;i++){
			__asm("NOP");
	}
}
*/
    return 0;
}
