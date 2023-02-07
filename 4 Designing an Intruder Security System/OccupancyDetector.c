
/*
 * OccupancyDetector.c
 *
 *  Created on: Feb 3, 2023
 *      Author: Matthew Gerace
 *
 *
 *
 */



#include <msp430.h>
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer



    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode




    P6DIR |= BIT6;              // Configure P6.6 to an Output
    P1DIR |= BIT0;

    P4DIR &= ~BIT1;             // Configure P4.1 to an Input
    P2DIR &= ~ BIT3;            // Configure P2.3 to an Input

    P4REN |= BIT1;               // Enable Resistor on P4.1
    P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup
    P2REN |= BIT3;                  // Enable resistor on P2.3
    P2OUT |= BIT3;                  // Configure resistor on P2.3 to be a pullup

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

#define count() //defining count variable




    while(1)
    {

      if (!(P4IN & BIT1))            // if 4.1 is pressed
      {
            int count = 0;                   // creates int and makes it 0

            do
            {                           //while count <20 keep incrementing count
                P6OUT &= ~BIT6;         //set green LED to off
                P1OUT ^= BIT0;          //toggle red LED ON

                __delay_cycles(1000000);  //delay for 1 second
                count = count + 1;              //Add to counter after every cylce

                if (P4IN & BIT1)      //if button is let go of
                   {
                       count = 0; //reset count
                       break; //exit if statement
                   }

            } while (count < 10); //do this while count is less than 10

            //upper while loop^^
            //lower while loop vv


            do                           // do while/ after its been in warning for 10 seconds
            {
                P1OUT |= BIT0;          //set red LED to high

                    if (!(P2IN & BIT3))  //if P2.3 is high
                    {
                        count = 0; //reset count
                        P1OUT &= ~BIT0; //set red LED to low
                        P6OUT ^= BIT6;                       // Toggle P6.6
                         __delay_cycles(500000);             // Delay for 0.5 seconds
                         P6OUT ^= BIT6;     //toggle
                         __delay_cycles(2500000); //delay for 2.5 seconds
                    }
            }while (count == 10); //once count is == 10 execute the alert state
      }


        else //acts as default
        {
            P1OUT &= ~BIT0;
            P6OUT ^= BIT6;          // Toggle P6.6
            __delay_cycles(500000);             // delay for 0.5seconds
            P6OUT ^= BIT6; //toggle
            __delay_cycles(2500000); //delay for 2.5 seconds
        }

    }

}
