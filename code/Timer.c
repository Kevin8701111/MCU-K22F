#ifndef uint32_t
	typedef __UINT32_TYPE__ uint32_t;
#endif
#ifndef uint8_t
	typedef __UINT8_TYPE__ uint8_t;
#endif

#include "pet.h"

void timerinit(void){
// --- Port Clock           --- //
	*(uint32_t*)0x40048038 |= 	0x00003E01; //[SIM_SCGC5]  portABCDE LPTMR
// --- Timer Setup (Delay)  --- //
	*(uint32_t*)0x40040000 = 	0x00000000; //[LPTMR0_CSR] LPTMR Reset
	*(uint32_t*)0x40040004 = 	0x00000000; //[LPTMR0_PSR] divides 2 ,MCGIRCLK
	*(uint32_t*)0x40040008 = 	99; 		//[LPTMR0_CMR] (CNR equals the CMR and increments Overflow)
											// 4MHz/(2*2) = 1MHz
											// 1MHz/100 = 10000Hz
											// 			  0.0001s

	*(uint8_t*)0x40064000 =	0x03;			//[MCG_C1] Internal Reference Clock,StopMode Enable
	*(uint8_t*)0x40064001 =	0x01;			//[MCG_C2] Fast internal reference clock selected 4MHz/2 [MCG_SC](FCRDIV)001

	*(uint32_t*)0xE000E104 =	0x04000000; //NVIC LPTMR interrupt vector assignment

	*(uint32_t*)0x40040000 |= 	0x00000041; //[LPTMR0_CSR] Interrupt, timer Enable

}

// --- Delay Function --- //
// delay();
// delay(1) = 100us
// delay(10000) = 1s

unsigned long timer_overflow_us = 0 ;	
int LPTMR0_IRQHandler(void){					//IRQ
	timer_overflow_us += 1 ;					// overflow count
	*(uint32_t*)0x40040000 |=	0x00000080;		// TCF w1c
}
void delay(int delay_number)
{
	unsigned long start = timer_overflow_us;
	while (delay_number > 0) {
		if ((timer_overflow_us - start) >= 1){
			delay_number--;
			start += 1;
		}
	}
}
void delayNOP(int delay_number){
	for(int i=0; i<delay_number;i++){
		__asm("NOP");
	}
}
unsigned long current_time(void){
	unsigned long t;
	t = timer_overflow_us;
	return t;
}
