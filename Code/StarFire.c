/*
|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
|                                                            StarFire LED Earrings                                                              |
|                                                                  StarFire.c                                                                   |
|                                                                 Version 1.0                                                                   |
|                                                              Prajwal Bhattaram                                                                |
|                                                                  16.03.2016                                                                   |
|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void setLED(uint8_t LED);
/*
    Truth table for LED matrix
    LEDs connected to PB0 (PIN1), PB1 (PIN2), PB2 (PIN3) and PB3 (PIN4)

    Led    | Pin High | Pin Low | Pin Disc. | PORTB | DDRB |
    --------------------------------------------------------
    1      | 1        | 2       | 3, 4      | 0x01  | xF3  |
    2      | 2        | 3       | 1, 4      | 0x02  | xF6  |
    3      | 3        | 4       | 1, 2      | 0x04  | xFC  |
    4      | 2        | 1       | 3, 4      | 0x02  | xF3  |
    5      | 3        | 2       | 1, 4      | 0x04  | xF6  |
    6      | 4        | 3       | 1, 2      | 0x08  | xFC  |
    7      | 2        | 4       | 1, 3      | 0x02  | xFA  |
    8      | 4        | 2       | 1, 3      | 0x08  | xFA  |
    9      | 1        | 3       | 2, 4      | 0x01  | xF5  |
    10     | 3        | 1       | 2, 4      | 0x04  | xF5  |
    11     | 1        | 4       | 2, 3      | 0x01  | xF9  |
    12     | 4        | 1       | 2, 3      | 0x08  | xF9  |

7654 3210
---------
1111 0000
0000 1111

*/
uint8_t currentLED;
uint8_t _DDR[12] {0xF3, 0XF6, 0XFC, 0XF3, 0XF6, 0XFC, 0XFA, 0XFA, 0XF5, 0XF5, 0XF9, 0XF9};
uint8_t _PORT[12] {0X01, 0X02, 0X04, 0X02, 0X04, 0X08, 0X02, 0X08, 0X01, 0X04, 0X01, 0X08}

void main()
{
  PORTB &= 0xF0;            // Set bits 0 to 3 to 0, i.e. off
  DDRB |= 0x0F;             // Set lower 4 bits to 1, i.e. output

  // Set up timer prescalar to FCpu/256 @ 1MHz
  TCCR0B |= (_BV(CS02));

  // Set timer to CTC mode
  TCCR0A |= _BV(WGM01);
  // Enable CTC interrupt
  TIMSK |= _BV(OCIE0A);
  // Enable global interrupts
  sei();

  // Set timer to compare 1/10 second
  OCR0A = 77;

  while(1)
  {
    //Turn on an LED
    setLED(currentLED);
  }
}

// Handle timer interrupt
// this triggers the interrupt every 1/4th of a second
ISR(TIMER0_COMPA_vect)
{
  //int i;

    /*if (i != currentLED)
    {
      currentLED = i;
    }
    else
    {
      i = rand() % 7;
    }*/
  currentLED = rand() % 49;
}

void setLED(uint8_t LED)
{
  // Clear existing status, we can only drive one LED at a time
  PORTB &= 0xF0;    // Set the lower 3 bits to 0, i.e. off
  DDRB |= 0x0F;   // Set the lower 3 bits to 1, i.e. output

  // Now set the LED as determined by the interrupt code
  DDRB &= _DDR[LED];   // Disconnect the pin as set in the DDR array
  PORTB |= _PORT[LED]
}

// Switch the state of the LED from on to off or from off to on
/*void setLED(uint8_t LED)
{
  //Clear existing status, we can only drive one LED at a time
  PORTB &= 0xF0;    // Set the lower 3 bits to 0, i.e. off
  DDRB |= 0x0F;   // Set the lower 3 bits to 1, i.e. output

  switch (LED)
  {
    case 0: 
      DDRB  &= 0xEF;  // Disconnect the nr. 2 pin
      PORTB |= 0x04;  // Set pin 0 to high and pin 1 to low
      break;
    
    case 1: 
      DDRB  &= 0xEF;  // Disconnect the nr. 2 pin
      PORTB |= 0x08;  // Set pin 1 to high and pin 0 to low
      break;
    
    case 2: 
      DDRB  &= 0xFB;  // Disconnect the nr. 0 pin
      PORTB |= 0x08;  // Set pin 1 to high and pin 2 to low
      break;        

    case 3: 
      DDRB  &= 0xFB;  // Disconnect the nr. 0 pin
      PORTB |= 0x10;  // Set pin 2 to high and pin 1 to low
      break;              
            
    case 4: 
      DDRB  &= 0xF7;  // Disconnect the nr. 1 pin
      PORTB |= 0x04;  // Set pin 0 to high and pin 2 to low
      break;
    
    case 5: 
      DDRB  &= 0xF7;  // Disconnect the nr. 1 pin
      PORTB |= 0x10;  // Set pin 2 to high and pin 0 to low
      break;

    default: 
      break;
  }
}*/