#include <msp430.h>
#include <GPIO_Driver.h>

/**
 * main.c
 *
 *
 * author: Matt Gerace
 * edited: 2/9/2023
 */


void delay(int t) //delay module
{
    int i; //variable i
    for (i=0;i<t;i++) //for loop that runs
    {
        __delay_cycles(100000); //delays for 0.1 sec
    }

}


int main(void)
{
    gpioinit(); //init func

    int count = 0; //counter for switching states

    while(1) //infinite loop
    {
        char detect = gpioRead(4,1); //make detect sensitive to P4.1
        char rst = gpioRead(2, 3); //make rst sensitive to P2.3
        if(!detect) //checks detect
        {
            if(count<10)
            {
            gpioWrite(6, 6, 0); //turns off green led
            gpioWrite(1, 0, 1); //turns on red
            delay(5); //delays for 0.5s
            gpioWrite(1,0,0); //turns off red
            delay(5); //delays for 0.5s
            count++; //increments count
            }
            if(count>=10) //once count hits 10
            {
                gpioWrite(1, 0, 1); //perma locks red led
            }

        }
        if(detect) //checks detect
        {
            if(count < 10) //only executes while count is under 10 to avoid switching back into this state during permalock
            {
            gpioWrite(1, 0, 0); //turns red led off
            gpioWrite(6, 6, 1); //turns green led on
            delay(5); //delays for 0.5s
            gpioWrite(6, 6, 0); //green led off
            delay(30); //delay for 3s
            }
        }

        if(!rst) //checks for rst
        {
            count = 0; //resets count variable
        }


    }

}


void gpioinit()
{
    gpioInit(4, 1, 0); //init P4.1 to input
    gpioInit(2, 3, 0); //int P2.3 to input
    gpioInit(1, 0, 1); //init P1.0 to output
    gpioInit(6, 6, 1); //init P6.6 to output


    P4REN |= BIT1;               // Enable Resistor on P4.1
    P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup
    P2REN |= BIT3;                  // Enable resistor on P2.3
    P2OUT |= BIT3;                  // Configure resistor on P2.3 to be a pullup

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

}
