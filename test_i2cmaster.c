
#include <avr/io.h>
#include "i2cmaster.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <i2cmaster.h>
#include <util/delay.h>

#define ADDR 0x9c

void show7(int val){
	if( val == 0 ){ //SCL turns off LED 7
		PORTB |=0b01000000;
		} else { //SCL turns on led 7
		PORTB &=0b10111111;
	}
}

void show6(int val){
	if( val == 0 ){ //SCL turns off LED 6
		PORTB |=0b00100000;
		} else { //SCL turns on led 6
		PORTB &=0b11011111;
	}
}

void displayTemp(int ack, int tem) {
	
	uint64_t ledDisplay;
	
    if (ack==1){
	    ledDisplay=0xffffffffffffffffUL;

	}
	else if (tem <= 20) {
		ledDisplay=0x0000000000000000UL;
	}
	else if (tem <=25) {
		ledDisplay=0xffffffff00000000UL;
	}
	else if (tem <= 30) {
		ledDisplay=0xffff0000ffff0000UL;
	}
	else if (tem <= 32){
		ledDisplay=0xff00ff00ff00ff00UL;
	}
	else if (tem <= 34){
		ledDisplay=0xf0f0f0f0f0f0f0f0UL;
	}
	else if (tem <= 36){
		ledDisplay=0xccccccccccccccccUL;
	}
	else {
		ledDisplay=0xaaaaaaaaaaaaaaaaUL;
	}

	for (int i=0;i<64;i++){
		show6(ledDisplay & 1);
		ledDisplay= ledDisplay >>1;
		_delay_ms(25);	
	}
	
}



void monitor(void){
	  show7( bit_is_clear(PIND,PD4) ); //SCL turns on LED 7
	  show6( bit_is_clear(PIND,PD5) ); //SDA turns on LED 6
}
 
int main(void) {

	int ack;
	uint8_t tempmsb, templsb;
	uint16_t temp;
	 
	DDRB=0x60;
    i2c_init();    // initialize I2C library
	


  while (1){
#if 0	  
	  PORTD=0x00; //Set as an input

//		PORTB = 0x60;
	  DDRD=(1<<4)|(1<<5); //enable internal pull ups
	  monitor();
	  _delay_ms(500);

//		PORTB = 0x00;
	  DDRD=(0<<4)|(0<<5); //disable internal pull ups
	  monitor();
	  _delay_ms(1000);
#else
	
		
		show7(1);
 	    _delay_ms(2000);
		show7(0);
		
		i2c_start(ADDR);
		i2c_write(0x02);
		i2c_write(0xff);
		i2c_stop();
	
		
		ack = i2c_start(ADDR);
		//show6(ack);
 	    //_delay_ms(500);
		 
		 i2c_write(0x04);
		 ack |=i2c_rep_start(ADDR+1);
		 //show6(ack);
		 //_delay_ms(500);

		 tempmsb=i2c_readAck();
		 templsb=i2c_readNak();
		 i2c_stop();

		temp = ((tempmsb & 0x1f) << 8) | (templsb);
		temp >>= 4; // drop 4 LSB to get value in celsius
		 displayTemp(ack, temp);

#endif	  	  

	  }
	  
   }

