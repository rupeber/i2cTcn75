
#include <avr/io.h>
#include "i2cmaster.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <i2cmaster.h>

#define Dev  0x9e      // device address, see datasheet
#define zero 0b10000001     // We will dislay a 0 if temperature is below limit
#define one 0b11010111      // We will display a 1 if temperature is above limit

int main(void) {
  uint8_t ret;
  char *string;
  char *string2;


  DDRC=0xFF; //7led segment display 
  uart_init(0);
  uart_printstrn(0, "temperature ");
  i2c_init();    // initialize I2C library


  while (1){
    ret=0;
                       
    i2c_start(Dev+1);  // set device address and read mode

    ret=i2c_readNak();
    
    if (ret>=30){ //Limit temperature set to 30 degrees
      PORTC=one;
    }

    else {
      PORTC=zero;
    }

    string = dtostrf(ret, 2, 0, string);
    string2= strcat(string,"°C");
    uart_printstrn(0, string2);
    
    _delay_ms(1000);
    
     }
    
}
