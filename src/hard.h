//----------------------------------------------------
// #### PROJECT: Magnet Antennas - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARD.H #######################################
//----------------------------------------------------

// Define to prevent recursive inclusion ---------------------------------------
#ifndef HARD_H_
#define HARD_H_

// #include "stm32g0xx.h"

//-- Defines For Configuration -------------------
//---- Configuration for Hardware Versions -------
#define HARD_4_1    // G030J6 uC small board one led bitonal comm & pulse
// #define HARD_4_0    // G030J6 uC small board only one comm led
// #define HARD_3_0    // G030J6 uC with cable connectors and two leds

#define SOFT_4_0    // send hard & soft on init

//-------- Clock Frequency ------------------------------------
#define CLOCK_FREQ_64_MHZ    // Modify this also on system_stm32g0xx.c SystemInit
// #define CLOCK_FREQ_16_MHZ    // Modify this also on system_stm32g0xx.c SystemInit


//-- End Of Defines For Configuration ---------------


// Exported Types Constants & Macros  ------------------------------------------

#ifdef HARD_4_1
// GPIOA pin12
#define PA12    ((GPIOA->ODR & 0x0001) != 0)
#define PA12_ON    (GPIOA->BSRR = 0x00001000)
#define PA12_OFF    (GPIOA->BSRR = 0x10000000)

// GPIOB pin0
#define LED    ((GPIOB->ODR & 0x0001) != 0)
#define LED_ON    (GPIOB->BSRR = 0x00000001)
#define LED_OFF    (GPIOB->BSRR = 0x00010000)

// GPIOB pin6    Alternative Usart1 tx/rx

// GPIOB pin7    Input Pullup
#define PULSE_INPUT    ((GPIOB->IDR & 0x0080) == 0)

#else    // older hard versions
// GPIOB pin0
#define LED    ((GPIOB->ODR & 0x0001) != 0)
#define LED_ON    (GPIOB->BSRR = 0x00000001)
#define LED_OFF    (GPIOB->BSRR = 0x00010000)
#endif

#define Led_Green_On()    Led_On()
#define Led_Green_Off()    Led_Off()
// Exported Module Functions ---------------------------------------------------
void Led_On (void);
void Led_Off (void);

void Led_Red_On (void);
void Led_Red_Off (void);

void Led_Check_Pulses (void);


#ifdef HARD_4_1
#define HARD_VER    "4.1"
#pragma message "Hardware version 4.1 -- rectangular small brd LED bitonal comms & pulse"
#endif
#ifdef HARD_4_0
#define HARD_VER    "4.0"
#pragma message "Hardware version 4.0 -- rectangular small brd only comms LED"
#endif
#ifdef HARD_3_0
#define HARD_VER    "3.0"
#pragma message "Hardware version 3.0 -- rectangular long brd with connectors and 2 LEDs"
#endif

#ifdef SOFT_4_0
#define SOFT_VER    "4.0"
#endif

#endif /* HARD_H_ */
