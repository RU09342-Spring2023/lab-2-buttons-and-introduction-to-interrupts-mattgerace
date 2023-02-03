/*
 * OccupancyDetector.c
 *
 *  Created on: Jan 30, 2023
 *      Author: russty
 */

#include <msp430.h>

int main(void){

#define ARMED_STATE 0
#define WARNING_STATE 1
#define ALERT_STATE 2

char state = ARMED_STATE;

    P6DIR |= BIT6;              // Configure P6.6 to an Output
    P4DIR &= ~BIT1;             // Configure P4.1 to an Input

    P4REN |= BIT1;               // Enable Resistor on P4.1
    P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup

    P4REN |= BIT1;                          // P4.1 pull-up register enable
    P4IES &= ~BIT1;                         // P4.1 Low --> High edge
    P4IE |= BIT1;                           // P4.1 interrupt enabled

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    while(1){

    switch(state){
        case ARMED_STATE:
        {
            if(P4IN & BIT1){
              __bis_SR_register(LPM3_bits | GIE); // Enter LPM3 w/interrupt
              __no_operation();                   // For debug
            }
            else
                P6OUT ^= BIT6;
            __delay_cycles(100000);
        }
    }



    }
}


#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 IFG
    P4IES ^= BIT1;                          // Transition the Edge Type (Low --> High, or High --> Low)
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}
