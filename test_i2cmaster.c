
#include <avr/io.h>
#include "i2cmaster.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <i2cmaster.h>
#include <util/delay.h>


#define Dev  0x9a      // device address, see datasheet

int main(void) {
  uint8_t ret;

  DDRB=0x60; //7led segment display 
  i2c_init();    // initialize I2C library


  while (1){
    ret=0;
                       
    i2c_start(Dev+1);  // set device address and read mode

    ret=i2c_readNak();
    
    if (ret>0 & ret<200){ //Limit temperature set to 30 degrees
      PORTB=0b10111111;
      _delay_ms(500);

      PORTB=0b11111111;
      _delay_ms(500);      
    }

    else {
      PORTB=0b10111111;
      _delay_ms(500);    
    }

    
     }
    
}
