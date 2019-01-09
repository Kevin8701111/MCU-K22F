#ifndef uint8_t
	typedef __UINT8_TYPE__ uint8_t;
#endif
#ifndef uint32_t
   typedef __UINT32_TYPE__ uint32_t;
#endif

int Delay_Flag = 0;	// Flag init
int LPTMR0_IRQHandler(void) {							//IRQ
	Delay_Flag = 1;	// Flag set
    	*(volatile uint32_t*)0x40040000 |=	0x00000080;		// TCF ,Timer Counter
    }

void delay(int delay_number) {
	Delay_Flag = 0;	// Flag Clear
    	*(volatile uint32_t*)0x40040000 |=	0x00000080;  // TCF ,Timer Counter

    for(int i=1; i<=delay_number; i++) {  //�瑁�甈⊥
        while(Delay_Flag != 1) {			 //Flag����艘��(閫貊IRQ)
        	__asm("NOP");
        }
      	Delay_Flag = 0;	//Flag Clear
    }
}

int setup(void) {
	*(volatile uint32_t*)0x40048038 = 0x00003E01; //SIM_SCGC5  PortA~E LPTMR

	// --- Timer Setup (Delay)  --- //
	*(volatile uint32_t*)0x40040000 = 0x00000000; //[LPTMR0_CSR] LPTMR Reset
	*(volatile uint32_t*)0x40040004 = 0x00000000; //[LPTMR0_PSR] divides 2 ,MCGIRCLK
	*(volatile uint32_t*)0x40040008 = 99; 		//[LPTMR0_CMR] (CNR equals the CMR and increments)
	   											// 4MHz/(2*2) = 1MHz
	   											// 1MHz/100 = 10000Hz
	   											// 			  0.0001s
	*(volatile uint8_t*)0x40064000 =0x03;			//[MCG_C1] Internal Reference Clock,StopMode Enable
	*(volatile uint8_t*)0x40064001 =0x01;			//[MCG_C2] Fast internal reference clock selected 4MHz/2

	*(volatile uint32_t*)0xE000E104 =0x04000000; //NVIC LPTMR interrupt vector assignment
    *(volatile uint32_t*)0x40040000 = 0x00000041; //[LPTMR0_CSR] Interrupt,LPTMR Timer Enabl

	//LCD1602 Pin Setup
	*(volatile uint32_t*)0x400FF014 = 0x00003036; //GPIOA_PDDR PTA1、2、4、5、12、13 output
	*(volatile uint32_t*)0x400FF054 = 0x000D0004; //GPIOB_PDDR PTA2、16、18、19 output

	*(volatile uint32_t*)0x40049004 = 0x00000140; //PORTA_PCR1  /PTA1/  RS
	*(volatile uint32_t*)0x40049008 = 0x00000140; //PORTA_PCR2  /PTA2/  E
	*(volatile uint32_t*)0x40049010 = 0x00000140; //PORTA_PCR4  /PTA4/  D3
	*(volatile uint32_t*)0x40049014 = 0x00000140; //PORTA_PCR5  /PTA5/  D2
	*(volatile uint32_t*)0x40049030 = 0x00000140; //PORTA_PCR12 /PTA12/ D1
	*(volatile uint32_t*)0x40049034 = 0x00000140; //PORTA_PCR13 /PTA13/ D0

	*(volatile uint32_t*)0x4004A008 = 0x00000140; //PORTB_PCR2  /PTB2/ D7
	*(volatile uint32_t*)0x4004A040 = 0x00000140; //PORTB_PCR16 /PTB16/ D6
	*(volatile uint32_t*)0x4004A048 = 0x00000140; //PORTB_PCR18 /PTB18/ D5
	*(volatile uint32_t*)0x4004A04C = 0x00000140; //PORTB_PCR19 /PTB19/ D4

	//KeyPad Pin Setup
	//return 0;
}

//************************************************************LCD1602************************************************************
void lcd1602_writeCome() {
	*(volatile uint32_t*)0x400FF008 = 0x00000002; //RS = 0
	*(volatile uint32_t*)0x400FF004 = 0x00000004; //Enable = 1

    delay(1);
    *(volatile uint32_t*)0x400FF008 = 0x00000004; //Enable = 0
    delay(1);
}

void lcd1602_writeData() {
	*(volatile uint32_t*)0x400FF004 = 0x00000002; //RS = 1
	*(volatile uint32_t*)0x400FF004 = 0x00000004; //Enable = 1

    delay(1);
    *(volatile uint32_t*)0x400FF008 = 0x00000004; //Enable = 0
    delay(1);
}

void lcd1602_init(void) {
	delay(1);
	*(volatile uint32_t*)0x400FF008 = 0x00000002; //RS = 0

	lcd1602_writeCome(0x01);
	lcd1602_writeCome(0x06);
	lcd1602_writeCome(0x0C);
	lcd1602_writeCome(0x38);


	/*(volatile uint32_t*)0x400FF004 = 0x00000004; //Enable = 1
	*(volatile uint32_t*)0x400FF048 = 0x00000004; //0x01
    *(volatile uint32_t*)0x400FF048 = 0x00010000;
    *(volatile uint32_t*)0x400FF048 = 0x00040000;
    *(volatile uint32_t*)0x400FF048 = 0x00080000;
    *(volatile uint32_t*)0x400FF008 = 0x00000010;
    *(volatile uint32_t*)0x400FF008 = 0x00000020;
    *(volatile uint32_t*)0x400FF008 = 0x00001000;
    *(volatile uint32_t*)0x400FF004 = 0x00002000;
    delay(1);
    *(volatile uint32_t*)0x400FF008 = 0x00000004; //Enable = 0
    delay(1);*/

	*(volatile uint32_t*)0x400FF004 = 0x00000004; //Enable = 1
	*(volatile uint32_t*)0x400FF048 = 0x00000004; //0x06
    *(volatile uint32_t*)0x400FF048 = 0x00010000;
    *(volatile uint32_t*)0x400FF048 = 0x00040000;
    *(volatile uint32_t*)0x400FF048 = 0x00080000;
    *(volatile uint32_t*)0x400FF008 = 0x00000010;
    *(volatile uint32_t*)0x400FF004 = 0x00000020;
    *(volatile uint32_t*)0x400FF004 = 0x00001000;
    *(volatile uint32_t*)0x400FF008 = 0x00002000;
    delay(1);
    *(volatile uint32_t*)0x400FF008 = 0x00000004; //Enable = 0
    delay(1);

	*(volatile uint32_t*)0x400FF004 = 0x00000004; //Enable = 1
	*(volatile uint32_t*)0x400FF048 = 0x00000004; //0x0C
    *(volatile uint32_t*)0x400FF048 = 0x00010000;
    *(volatile uint32_t*)0x400FF048 = 0x00040000;
    *(volatile uint32_t*)0x400FF048 = 0x00080000;
    *(volatile uint32_t*)0x400FF004 = 0x00000010;
    *(volatile uint32_t*)0x400FF004 = 0x00000020;
    *(volatile uint32_t*)0x400FF008 = 0x00001000;
    *(volatile uint32_t*)0x400FF008 = 0x00002000;
    delay(1);
    *(volatile uint32_t*)0x400FF008 = 0x00000004; //Enable = 0
    delay(1);

	*(volatile uint32_t*)0x400FF004 = 0x00000004; //Enable = 1
	*(volatile uint32_t*)0x400FF048 = 0x00000004; //0x38
    *(volatile uint32_t*)0x400FF048 = 0x00010000;
    *(volatile uint32_t*)0x400FF044 = 0x00040000;
    *(volatile uint32_t*)0x400FF044 = 0x00080000;
    *(volatile uint32_t*)0x400FF004 = 0x00000010;
    *(volatile uint32_t*)0x400FF008 = 0x00000020;
    *(volatile uint32_t*)0x400FF008 = 0x00001000;
    *(volatile uint32_t*)0x400FF008 = 0x00002000;
    delay(1);
    *(volatile uint32_t*)0x400FF008 = 0x00000004; //Enable = 0
    delay(1);

	*(volatile uint32_t*)0x400FF004 = 0x00000004; //Enable = 1
	*(volatile uint32_t*)0x400FF044 = 0x00000004; //0x85
    *(volatile uint32_t*)0x400FF048 = 0x00010000;
    *(volatile uint32_t*)0x400FF048 = 0x00040000;
    *(volatile uint32_t*)0x400FF048 = 0x00080000;
    *(volatile uint32_t*)0x400FF008 = 0x00000010;
    *(volatile uint32_t*)0x400FF004 = 0x00000020;
    *(volatile uint32_t*)0x400FF008 = 0x00001000;
    *(volatile uint32_t*)0x400FF004 = 0x00002000;
    delay(1);
    *(volatile uint32_t*)0x400FF008 = 0x00000004; //Enable = 0
    delay(1);

    //寫入資料至LCD
    *(volatile uint32_t*)0x400FF004 = 0x00000002; //RS = 1

	*(volatile uint32_t*)0x400FF004 = 0x00000004; //Enable = 1
	*(volatile uint32_t*)0x400FF048 = 0x00000004; //0x41    寫入"A"
    *(volatile uint32_t*)0x400FF044 = 0x00010000;
    *(volatile uint32_t*)0x400FF048 = 0x00040000;
    *(volatile uint32_t*)0x400FF048 = 0x00080000;
    *(volatile uint32_t*)0x400FF008 = 0x00000010;
    *(volatile uint32_t*)0x400FF008 = 0x00000020;
    *(volatile uint32_t*)0x400FF008 = 0x00001000;
    *(volatile uint32_t*)0x400FF004 = 0x00002000;
    delay(1);
    *(volatile uint32_t*)0x400FF008 = 0x00000004; //Enable = 0
    delay(1);
}

//************************************************************LCD1602***********************************************************

void main() {
	char c;
	setup();
	lcd1602_init();

	scanf("%s", &c);
	lcd1602_writeData(c);

	return 0;
}
