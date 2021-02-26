/**************************************************************************************
* Author: Christina Kidwell
* Course: EGR 226 - 905
* Date: 02/25/2021
* Project: Lab 5 Part 1
* File: main.c
* Description: A program that cycles the External LEDs through Red, Yellow & Green colors,
* changing every time a button is pressed.
*
**************************************************************************************/
#include "msp.h"

void setupIO(void);
void SysTick_Init (void);
void SysTick_delay (uint16_t delay);
int state(void);
int waitForPress(void);

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    setupIO();
    SysTick_Init();

    while(1) {
        while(waitForPress()==0); // wait for button press
        // turn External LED to Red
        P2->OUT |= BIT0;
        P2->OUT &= ~BIT1;
        P2->OUT &= ~BIT2;
        while(waitForPress()==0); // wait for button press
        // turn External LED to Yellow
        P2->OUT &= ~BIT0;
        P2->OUT |= BIT1;
        P2->OUT &= ~BIT2;
        while(waitForPress()==0); // wait for button press
        // turn External LED to Green
        P2->OUT &= ~BIT0;
        P2->OUT &= ~BIT1;
        P2->OUT |= BIT2;
    }

}

/****| setupIO | ***********************************************
* Brief: Function to configure the input and output bits used
* for the LEDs and switches
* Param:
* none
* return:
* N/A
*************************************************************/
void setupIO(void){
    // Configure Red LED, special function bits to 0, direction to output, resistor off
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->DIR |= BIT0;
    P1->REN &= ~BIT0;

    // Configure External Red LED, special function bits to 0, direction to output, resistor off
    P2->SEL0 &= ~BIT0;
    P2->SEL1 &= ~BIT0;
    P2->DIR |= BIT0;
    P2->REN &= ~BIT0;

    // Configure External Yellow LED, special function bits to 0, direction to output, resistor off
    P2->SEL0 &= ~BIT1;
    P2->SEL1 &= ~BIT1;
    P2->DIR |= BIT1;
    P2->REN &= ~BIT1;

    // Configure External Green LED, special function bits to 0, direction to output, resistor off
    P2->SEL0 &= ~BIT2;
    P2->SEL1 &= ~BIT2;
    P2->DIR |= BIT2;
    P2->REN &= ~BIT2;

    // Configure External Switch, special function bits to 0, direction to input, resistor enabled as a pull up
    P6->SEL0 &= ~BIT4;
    P6->SEL1 &= ~BIT4;
    P6->DIR &= ~BIT4;
    P6->REN |= BIT4;
    P6->OUT |= BIT4;

    // Configure Switch 2, special function bits to 0, direction to input, resistor enabled as a pull up
    P1->SEL0 &= ~BIT4;
    P1->SEL1 &= ~BIT4;
    P1->DIR &= ~BIT4;
    P1->REN |= BIT4;
    P1->OUT |= BIT4;

    // turn off Red LED
    P1->OUT &= ~BIT0;

    // turn off External LED
    P2->OUT &= ~BIT0;
    P2->OUT &= ~BIT1;
    P2->OUT &= ~BIT2;
}
/****| SysTick_Init | *********************************************
* Brief: Function to initialize SysTick for use as a millisecond timer
* Param:
* none
* return:
* N/A
*************************************************************/
void SysTick_Init (void) {
    //initialization of systic timer
    SysTick->CTRL = 0; // disable SysTick During step
    SysTick->LOAD = 0x00FFFFFF; // max reload value
    SysTick->VAL = 0; // any write to current clears it
    SysTick->CTRL = 0x00000005; // enable systic, 3MHz, No Interrupts
}

/****| SysTick_delay | *********************************************
* Brief: Function that uses SysTick to generate a delay
* of a given number of milliseconds
* Param:
* uint16_t delay = number of milliseconds to delay
* return:
* N/A
*************************************************************/
void SysTick_delay (uint16_t delay) {
    // Systick delay function
    SysTick -> LOAD = ((delay * 3000) - 1); //delay for 1 msecond per delay value
    SysTick -> VAL = 0; // any write to CVR clears it
    while ((SysTick -> CTRL & 0x00010000) == 0); // wait for flag to be SET
}

/****| state | ***********************************************
* Brief: loop until the condition of the switch is consistent
* for two consecutive readings that are separated by 15ms
* Param:
* none
* return:
* 0 if button pressed
* 1 if button not pressed
*************************************************************/
int state(void){
    int temp,temp2;
    temp=P6->IN & BIT4; // get initial switch condition and save it
    while(1){
        SysTick_delay(15); // delay a bit
        temp2 = P6->IN & BIT4; // get another switch condition
        if (temp==temp2){ // if the new switch condition matches the saved one
            if(temp==0) return 0;
            else return 1; // then return the saved value
        }
        else {
            temp = temp2; // otherwise, update saved condition with new one
        }
    }
}


/****| waitForPress | ***********************************************
* Brief:
* if the button is not being pressed, return a 0
* if the button is being pressed, wait for release, then return a 1
* Param:
* none
* return:
* 0 if button pressed
* 1 if button not pressed
*************************************************************/
int waitForPress(void) {
    if(state()==0) return 0; //if button is pressed, return 0
    while (state()==1);      //if button is not pressed, wait for it to be pressed
    return 1;                //then return 1;
}
