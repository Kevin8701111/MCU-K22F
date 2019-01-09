#ifndef uint32_t
	typedef __UINT32_TYPE__ uint32_t;
#endif
#ifndef uint8_t
	typedef __UINT8_TYPE__ uint8_t;
#endif

#include "pet.h"

void Keypadinit(void){
/*
	rowPins	row1 - PTD4
			row2 - PTD5
			row3 - PTD6
			row4 - PTD7
			
	colPins	col1 - PTC11
			col2 - PTC8
			col3 - PTC4					
*/
	*(uint32_t*)0x40048038 |= 	0x00003E01; //[SIM_SCGC5 ]  port ABCDE LPTMR
//rowPins (input)
	*(uint32_t*)0x4004C010 =	0x00000103; //[PORTD_PCR4 ] GPIO Pull Enable,Pullup
	*(uint32_t*)0x4004C014 =	0x00000103; //[PORTD_PCR5 ]
	*(uint32_t*)0x4004C018 =	0x00000103; //[PORTD_PCR6 ]
	*(uint32_t*)0x4004C01C =	0x00000103; //[PORTD_PCR7 ]
//colPins (output)
	*(uint32_t*)0x4004B02C =	0x00000100; //[PORTC_PCR11 ]	GPIO
	*(uint32_t*)0x4004B020 =	0x00000100; //[PORTC_PCR8 ]
	*(uint32_t*)0x4004B010 =	0x00000100; //[PORTC_PCR4 ]
//GPIO PPDR
	*(uint32_t*)0x400FF094 |=	0x00000910; //[GPIOC_PDDR ] PTC 11,8,4 output

}

unsigned long Keypad_scan_time = 0; //掃描頻率
unsigned long Keypad_debounce = 0;	//防彈跳
char KeypadGetkey(void){
	char keychar = 'N';
	unsigned long now = timer_overflow_us;			//取得目前 timer_overflow

	if(( now - Keypad_debounce ) >= 3500){
		keychar = Keypadscan();						//得到字符
		if (keychar != 'N'){						//如果有按下按鍵
			Keypad_debounce = timer_overflow_us;	//取得按下時間
		}
	}
	return keychar;
}
char Keypadscan(void){
	char keychar = 'N';

	unsigned long now = timer_overflow_us;		//取得目前 timer_overflow

	if(( now - Keypad_scan_time ) >= 1000 ){	//掃描頻率
		Keypad_scan_time = now ;

	*(uint32_t*)0x400FF084	|= 	0x00000910; // PTC 11,8,4 set	//	0x400FF084	GPIOC_PSOR
																//	0x400FF088	GPIOC_PCOR
// col - 1
	*(uint32_t*)0x400FF088	|= 	0x00000800; // PTC 11 c
	*(uint32_t*)0x400FF084	|= 	0x00000100; // PTC  8 s
	*(uint32_t*)0x400FF084	|= 	0x00000010; // PTC  4 s
		//[GPIOD_PDIR ] Input Register
	if	   (*(uint32_t*)0x400FF0D0 == 0x000000E0){	keychar = '1';	}	//row-1 1110
	else if(*(uint32_t*)0x400FF0D0 == 0x000000D0){	keychar = '4';	}	//row-2 1101
	else if(*(uint32_t*)0x400FF0D0 == 0x000000B0){	keychar = '7';	}	//row-3 1011
	else if(*(uint32_t*)0x400FF0D0 == 0x00000070){	keychar = '*';	}	//row-4 0111
// col - 2
	*(uint32_t*)0x400FF084	|= 	0x00000800; // PTC 11 s
	*(uint32_t*)0x400FF088	|= 	0x00000100; // PTC  8 c
	*(uint32_t*)0x400FF084	|= 	0x00000010; // PTC  4 s
		//[GPIOD_PDIR ] Input Register
	if	   (*(uint32_t*)0x400FF0D0 == 0x000000E0){	keychar = '2';	}	//row-1 1110
	else if(*(uint32_t*)0x400FF0D0 == 0x000000D0){	keychar = '5';	}	//row-2 1101
	else if(*(uint32_t*)0x400FF0D0 == 0x000000B0){	keychar = '8';	}	//row-3 1011
	else if(*(uint32_t*)0x400FF0D0 == 0x00000070){	keychar = '0';	}	//row-4 0111
// col - 3
	*(uint32_t*)0x400FF084	|= 	0x00000800; // PTC 11 s
	*(uint32_t*)0x400FF084	|= 	0x00000100; // PTC  8 s
	*(uint32_t*)0x400FF088	|= 	0x00000010; // PTC  4 c
		//[GPIOD_PDIR ] Input Register
	if	   (*(uint32_t*)0x400FF0D0 == 0x000000E0){	keychar = '3';	}	//row-1 1110
	else if(*(uint32_t*)0x400FF0D0 == 0x000000D0){	keychar = '6';	}	//row-2 1101
	else if(*(uint32_t*)0x400FF0D0 == 0x000000B0){	keychar = '9';	}	//row-3 1011
	else if(*(uint32_t*)0x400FF0D0 == 0x00000070){	keychar = '#';	}	//row-4 0111

	}

	return keychar;
}

