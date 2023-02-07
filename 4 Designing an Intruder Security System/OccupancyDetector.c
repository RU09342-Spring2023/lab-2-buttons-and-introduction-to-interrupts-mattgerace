/*
 * OccupancyDetector.c
 *
 *  Created on: Feb 3, 2023
 *      Author: Matthew Gerace
 */

#include <msp430.h>

#define ARMED_STATE 0 //defining states that msp could possibly be in
#define WARNING_STATE 1
#define ALERT_STATE 2


int main(void){




    P6DIR |= BIT6;              // Configure P6.6 to an Output
    P1DIR |= BIT0;

    P4DIR &= ~BIT1;             // Configure P4.1 to an Input

    P4REN |= BIT1;               // Enable Resistor on P4.1
    P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup

    P4REN |= BIT1;                          // P4.1 pull-up register enable
    P4IES &= ~BIT1;                         // P4.1 Low --> High edge
    P4IE |= BIT1;                           // P4.1 interrupt enabled

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings


    char Button = !(P4IN & BIT1); //assigning char button to bitmask
    char state = ARMED_STATE; //assigning designated state and default state to armed
    int count = 0; //creating and int called count to track seconds


    while(1){

    switch(state)
    {
        case 0:
        {
            if(Button)
            {
                P6OUT &= ~BIT6; //turn P6.6 off
                state = 1; //state change when button/movement is detected
            }

            if(!Button)
            {
                P1OUT &= !BIT0; //standard to turn off P1.0 LED
                P6OUT ^= BIT6; //toggles P6.6 LED
                __delay_cycles(500000); //waits 0.5 seconds
                P6OUT ^= BIT6; //toggles P6.6 LED off
                __delay_cycles(2500000); //waits 2.5 seconds
            }


        }

        case 1:
        {
            if(Button)
            {
                state = 1; //keeps it in state 1
                P1OUT ^= BIT0;  //toggles P1OUT
                count = count + 1; //count up
                if (count == 10) //if count reaches 10
                    {
                        state = 2; //state change to alert
                    }
                __delay_cycles(500000); //wait 0.5s
                }


            if(!Button)
            {
                state = 0;  //if movement stops being detected state 0
            }


        }
        case 2:         //case 0 and 1 work as expected, even count variable works as expected, had trouble transitioning into state 2 and keeping LED on
        {
            if(Button)
            {
                state = 2; //stay in alert
                P1OUT |= BIT0; //turn P1 on constantly
                __delay_cycles(500000);
            }
            if(!Button) //if button is released go back to armed
            {
                state = 0;
            }
        }
    }



    }
}
