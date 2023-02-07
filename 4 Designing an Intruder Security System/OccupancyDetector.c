/*
 * OccupancyDetector.c
 *
 *  Created on: Jan 30, 2023
 *      Author: russty
 */

#include <msp430.h>

#define ARMED_STATE 0
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


    char Button = !(P4IN & BIT1);
    char state = ARMED_STATE;
    double count = 0;


    while(1){

    switch(state)
    {
        case 0:
        {
            if(Button)
            {
                P6OUT &= ~BIT6;
                state = 1;
            }

            if(!Button)
            {
                P1OUT &= !BIT0;
                P6OUT ^= BIT6;
                __delay_cycles(6000000);
            }


        }

        case 1:
        {
            if(Button)
            {
                state = 1;

                for(count=0; count < 11; count = count+1)
                {
                    P1OUT ^= BIT0;
                 }
                __delay_cycles(500000);

                if(count == 10)
                {
                    state = 2;
                }
                }


            if(!Button)
            {
                state = 0;
            }


        }
        case 2:
        {
            if(Button)
            {
                state = 2;
                P1OUT |= BIT0;
                P6OUT ^= BIT6;
                __delay_cycles(500000);
            }
            if(!Button)
            {
                state = 2;
                P6OUT ^= BIT6;
                __delay_cycles(500000);
            }
        }
    }



    }
}
