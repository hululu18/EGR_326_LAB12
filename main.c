/*------------------------------------------------
*   Authors = Dixit Gurung, Jacob VanderVeen
*   EGR 226 908
*   Date =
*   Lab10
*   PART I, II, III and Extra credit
*---------------------------------------------------*/

#include "msp.h"
#include <stdio.h>

void ADC14_init(void); //initialize the ADC
void Led_pwm(void); //initialize  timerA for controlling PWM

int PWMperiod = 5000;

main(void)

{
    int  result;



    ADC14_init();
    Led_pwm();

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;



    while (1)
    {

        ADC14->CTL0 |= 1;
        while (!ADC14->IFGR0);
        result = ADC14->MEM[0];
        //making the value of result comparable with CCR[0] value
        if (result >20000){
            result = 20000;
        }

        //scaling the result value with 6000
        //for example if result = 18000, then it will be converted to 18000*0.3 = 5400
        //So that It could be directly fed to CCR[2], to control PWM
        result = result * 0.3;



        printf("result = %d\n\n",result);
        TIMER_A1->CCR[2]   = 5000-result;



    }

}

void ADC14_init(void)

{

    P5SEL0 |= 0X20;  //P5.5 pin is selected

    P5SEL1 |= 0X20;

    ADC14->CTL0 &= ~0x00000002;

    ADC14->CTL0 |= 0x04400110;

    ADC14->CTL1 |= 0x00000030;

    ADC14->CTL1 |= 0x00000000;

    ADC14->MCTL[0] = 0x00000000;

    //ADC14->MCTL[0] = ADC14_MCTLN_INCH_0;

    ADC14->CTL0 |= 0x00000002;

}




//This function is setup for generating PWM for led
void Led_pwm(void)

{

    P7->SEL0 |= BIT6;                             //sets up TimerA2.1 on P5.6

    P7->SEL1 &= ~ BIT6;

    P7->DIR |= BIT6;                              //outputs PWM signal

    TIMER_A1->CCR[0] = PWMperiod; //70ms period CLK/8
    TIMER_A1->CCR[2] = .99*PWMperiod; //10us ON time CLK/8

    TIMER_A1->CCTL[2] = TIMER_A_CCTLN_OUTMOD_3;    // Reset/set

    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 |         // SMCLK
                    TIMER_A_CTL_ID_3 |              //divide CLK by 8
                    TIMER_A_CTL_MC_1 |             // Up Mode ... Count up
                    TIMER_A_CTL_CLR;               // clear TA0R Register
}

