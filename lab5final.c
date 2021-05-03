/*********************************************************************
 * ECE 344L - Microprocessors - Spring 2021
 * Russell Teodoro
 * Lab 5 - Using Serial Ports
 * April 9, 2021
 ********************************************************************/
#include <plib.h>

// Configure MX7 board for debugging
#pragma config ICESEL = ICS_PGx1

#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = EC
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_8
#pragma config FSOSCEN = OFF            // Secondary oscillator enable
#define SYS_FREQ (80000000L)

#define PB_DIV		8			// 10 MHz peripheral clock
#define PRESCALE          256
#define TOGGLES_PER_SEC        2
#define T2_TICK             (SYS_FREQ/PB_DIV/PRESCALE/TOGGLES_PER_SEC)
#define	cntMsDelay	10000

void ChangeLEDs();
void ChangeState();
void DeviceInit();
void DelayInit();
void DelayMs(int cms);

unsigned long int *u2txregp;
unsigned long int *u2rxregp;

int count = 0;
unsigned int c;


void __ISR(_TIMER_2_VECTOR, ipl2) Timer2Handler(void)
{
	mT2ClearIntFlag(); // clear the interrupt flag
		
}

void __ISR(_UART2_VECTOR, ipl3) Uart2Handler(void)
{
	extern unsigned int c;
	extern int count;

		c = ReadUART2();			// read char from RXREG
		mU2RXClearIntFlag(); // clear int flag
		count++;
		count = (count)%256;
		if (c >=65 && c <= 90){
			c = c + 32;
		}
		
		
									
     
        // echo the character
  	WriteUART2(c);    
}

void ChangeLEDs(){
	switch(count){
	case 0: PORTClearBits(IOPORT_E, BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7); break;
		case 1: PORTWrite (IOPORT_E, BIT_0);break;
		case 2: PORTWrite (IOPORT_E, BIT_1);break;
		case 3: PORTWrite (IOPORT_E, BIT_0 |BIT_1);break;
		case 4: PORTWrite (IOPORT_E, BIT_2);break;
		case 5: PORTWrite (IOPORT_E, BIT_0 |BIT_2);break;
		case 6: PORTWrite (IOPORT_E, BIT_1 |BIT_2);break;
		case 7: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2);break;
		case 8: PORTWrite (IOPORT_E, BIT_3 );break;
		case 9: PORTWrite (IOPORT_E, BIT_0|BIT_3);break;
		case 10: PORTWrite (IOPORT_E, BIT_1 |BIT_3);break;
		case 11: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_3);break;
		case 12: PORTWrite (IOPORT_E, BIT_2 |BIT_3 );break;
		case 13: PORTWrite (IOPORT_E, BIT_0 |BIT_2 |BIT_3);break;
		case 14: PORTWrite (IOPORT_E, BIT_1 |BIT_2 |BIT_3 );break;
		case 16: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 17: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 18: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 19: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 20: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 21: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 22: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 23: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 24: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 25: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 26: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		case 27: PORTWrite (IOPORT_E, BIT_0 |BIT_1 |BIT_2 |BIT_3);break;
		default : PORTClearBits(IOPORT_E, BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);break;
	}
}

	
int main(void)
{
	extern unsigned long int *u2txregp;
	extern unsigned long int *u2rxregp;
	extern int count;
	u2txregp = 0xbf806220L;
	u2rxregp = 0xbf806230L;

    int char_in_flag;
	unsigned long int u2mode;
	unsigned long int u2status;
	
	DeviceInit();
	DelayInit();
	

    
    u2mode = UART_EN | UART_IDLE_CON | UART_RX_TX |
                UART_DIS_WAKE | UART_DIS_LOOPBACK |
                UART_DIS_ABAUD | UART_NO_PAR_8BIT | UART_EN_BCLK |
                UART_1STOPBIT | UART_IRDA_DIS | UART_MODE_SIMPLEX |
                UART_NORMAL_RX | UART_BRGH_SIXTEEN;
u2status = UART_TX_PIN_LOW | UART_RX_ENABLE | UART_TX_ENABLE|UART_INT_RX_CHAR;
    
//    u2mode = 0xb800;          // equivalent instructions
//    u2status = 0x1400;      
    
	// configured for 9600 baud
    OpenUART2(u2mode, u2status, 64);
	ConfigIntUART2(UART_RX_INT_EN|UART_TX_INT_DIS|UART_INT_PR3);
	mU2RXClearIntFlag();
    WriteUART2(62);        // prompt
   
    while(1)
	{
		mPORTEWrite(count);
	}
	return 0;
}
 void DeviceInit()
{
	//On MX7 LED1 is on RG12
	//		   LED2 is on RG13
	//		   LED3 is on RG14
	//		   LED4 is on RG15
	//Set ports for onboard LEDs to outputs & claer them
PORTSetPinsDigitalOut (IOPORT_E, BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);
	PORTClearBits(IOPORT_E, BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);	//On MX7 PMODLED1 is on E0

}

void DelayInit()	
{
	unsigned int tcfg;

	/* Config Timer 2. This sets it to count 312500 Hz with a period of T2_TICK	*/
	tcfg = T2_ON|T2_SOURCE_INT|T2_PS_1_256;
	OpenTimer2(tcfg, T2_TICK);
	
	/* Now enable system-wide multi-vector interrrupt handling	*/
	INTEnableSystemMultiVectoredInt();
	
	/* configure timer 2 interrupt with priority of 2	*/
	ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
	
	/* Clear interrupt flag	*/
	mT2ClearIntFlag();

}
// *****************************************************************************