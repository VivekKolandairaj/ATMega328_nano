/*
 * nanoBlink.c
 *
 * Created: 12-07-2021 01:28:56
 * Author : Vivek
 * Description: A register level code for blinking LED connected to PB4 in the nano board. 
 *              Timer 0 will be used to generate the delay of approximately 1 milli second.
 */ 

#include <stdint.h>


int main(void)
{
	//pointers to the timer 0 registers
	//pointer to the timer/counter control register A
	uint8_t volatile *const pTCCR0A = (uint8_t*) 0x44;
	//pointer to the timer/counter control register B
	uint8_t volatile *const pTCCR0B = (uint8_t*) 0x45;
	//pointer to the timer/counter register
	uint8_t volatile *const pTCNT0 = (uint8_t*) 0x46;
	//pointer to the timer/counter 0 interrupt flag register
	uint8_t volatile *const pTIFR0 = (uint8_t*) 0x35;
	
	//pointers to the port B registers
	//pointer to the port B data direction register
	uint8_t volatile *const pDDRB = (uint8_t*) 0x24;
	//pointer to the port B data register
	//uint8_t volatile *const pPortB = (uint8_t*) 0x25;
	//pointer to the port B pin register
	uint8_t volatile *const pPinB = (uint8_t*) 0x23;
	
	//a variable for counting overflow
	uint8_t volatile ovflo_count = 0;
	
    /* Replace with your application code */
	//setup PB4 as output
	*pDDRB |= (1 << 4);
	
	//setup timer 0 in normal mode operation
	*pTCCR0A = 0x00;
	//setup prescaler for timer 0 as clk/1024
	*pTCCR0B = 0x05;
	//load the timer/counter register
	*pTCNT0 = 0x00;
	
    while (1) 
    {
		//toggle PB4
		*pPinB |= (1 << 4);
		
		while(ovflo_count < 61){
			if((*pTIFR0 & 0x01) == 1){
				ovflo_count++; //increment the overflow counter
				*pTIFR0 = 0x01; //clear the overflow flag
				*pTCNT0 = 0x00; //reload the counter
			}
			else continue;
		}
		
		ovflo_count = 0;
		
		
		
    }
}

/* Timer calculations:
*  The nano board uses a 16MHz crystal
*
*  clock for timer = 16MHz/1024 = 15625 = 15.625 KHz
*  1 cycle = 1/15625 = 64 micro seconds
*  Timer overflows when count equals 0xFF. i.e after 256 cycles.
*  overflow time = 256 X 64 micro seconds = 16.384 milli seconds
*  For 1 second = 1000 milli seconds, number of overflows = 1000 milli seconds/ 16.384 milli seconds = 61.035
*  I will approximate this as 61. 
*  For counting 1 second precisely, the counter should overflow 61 times. 
*/