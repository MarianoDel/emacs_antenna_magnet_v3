//----------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARD.C #######################################
//----------------------------------------------------

// Includes --------------------------------------------------------------------
#include "hard.h"
#include "stm32g0xx.h"


// Module Private Types Constants and Macros -----------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
unsigned char red_pulse_on = 0;
unsigned char green_is_on = 0;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
#ifdef HARD_4_1
void Led_Red_On (void)
{
    red_pulse_on = 1;
    LED_OFF;
    PA12_ON;
}


void Led_Red_Off (void)
{
    red_pulse_on = 0;
    PA12_OFF;

    if (green_is_on)
    {
	LED_ON;    // on green led
    }
}


void Led_On (void)
{
    green_is_on = 1;
    
    if (red_pulse_on)
	return;    // update just the state

    LED_ON;
    PA12_OFF;
}


void Led_Off (void)
{
    green_is_on = 0;
    LED_OFF;
}


void Led_Check_Pulses (void)
{
    if (PULSE_INPUT)
	Led_Red_On();
    else
	Led_Red_Off();
}

#else    // for older versions
void Led_On (void)
{
    LED_ON;
}


void Led_Off (void)
{
    LED_OFF;
}


void Led_Check_Pulses (void)
{
}
#endif




//--- end of file ---//
