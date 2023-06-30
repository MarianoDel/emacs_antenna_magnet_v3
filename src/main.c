//-------------------------------------------------------
// #### PROJECT: Magnet Antenna  G030 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MAIN.C ##########################################
//-------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32g0xx.h"
#include "hard.h"
#include "gpio.h"
#include "comms.h"
#include "usart.h"
#include "tim.h"
#include "adc.h"
// #include "dsp.h"
#include "temp_sensor.h"
#include "test_functions.h"

#include <string.h>
#include <stdio.h>


//ANTENA ELEGIDA    VER EN HARD MODELO DE PLACA ANTENA!!!
// #define ANTENA0    // plana 125mm 23mm 1.3kg alambre 0.8mm dia -- Plannar 5 inches
// #define ANTENA1	//toroidal diametro mediana
// #define ANTENA1B    // plana 125mm 23mm 1.3kg -- Plannar 5 inches
// #define ANTENA2	//cilindrica chica tunel de munieca
// #define ANTENA3	//cilindrica mediana
// #define ANTENA4	//cilindrica grande
// #define ANTENA4B	//cilindrica grande, fabricada 18-06-2021
// #define ANTENA4C    //tunnel 10", fabricada 23-08-2021
// #define ANTENA4D    // tunnel 10", fabricada 22-09-2022
// #define ANTENA4E    // tunnel 10", fabricada 10-01-2023
// #define ANTENA4F    // tunnel 10" head, fabricada 10-01-2023
#define ANTENA4G    // tunnel 8", fabricada 10-01-2023
// #define ANTENA4H    // tunnel 5", fabricada 10-01-2023
//#define ANTENA5	//cilindrica muy chica OJOS
//#define ANTENA6	//cilindrica vieja de madera
// #define ANTENA7 //pencil tunel
//#define ANTENA8 //pencil plato doble
//#define ANTENAA1	//Ernesto viejas
//#define ANTENAA2	//Ernesto viejas
//#define ANTENAA3	//Ernesto viejas
// #define ANTENAA5	//Ernesto viejas
//#define ANTENAA4	//Ernesto viejas
//#define ANTENAA6 //antenas Ernesto tunel grande 350mm dia
//#define ANTENAA7 //antenas Ernesto tunel mediano 240mm dia
//#define ANTENAA1 //antenas Ernesto tunel chico 150mm dia
//#define ANTENAA9	//Nuevas Luis
//#define ANTENAB0	//antenas Ernesto tunel 9" dia
//#define ANTENAB1	//antenas Ernesto tunel 7" dia
//#define ANTENAB2	//antenas Ernesto tunel 6" dia
// #define ANTENAB3	//antenas Ernesto tunel 6" dia
// #define ANTENAB4	//antenas plato companiera pencil
// #define ANTENAB5	//antenas para ojos 2 bobinas en paralelo
// #define ANTENAB6	//eyes gogles 48G, the new ones

// Externals -------------------------------------------------------------------
// -- Externals for the timer module --------------------
volatile unsigned char timer_1seg = 0;
volatile unsigned short timer_standby = 0;
volatile unsigned short timer_led_comm = 0;

// -- Externals from or for Serial Port --------------------
volatile unsigned char buffrx_ready = 0;
volatile unsigned char *pbuffrx;
volatile unsigned char *pbuffrx_cpy;

const char s_ok [] = {"ok\r\n"};

//--- VARIABLES GLOBALES ---//
// Each name has to have less than 8 chars
//antena, R [ohms], L [mHy], Imax [A], Tmax [ºC] todos 000.00
#ifdef ANTENA0 //toroidal diametro grande
const char s_antena [] = { "ant0,012.27,087.90,001.80,065.00\r\n" };
const char s_name [] = { "name:Plannar 5 inches\r\n" };
#endif

#ifdef ANTENA1 //toroidal diametro mediana
const char s_antena [] = { "ant1,023.85,141.60,001.30,065.00\r\n" };
const char s_name [] = { "name:Plannar 5 inches\r\n" };
#endif

#ifdef ANTENA1B // plana 125mm 23mm 1.3kg -- Plannar 5 inches
const char s_antena [] = { "ant1,017.00,120.00,001.30,065.00\r\n" };
const char s_name [] = { "name:Plannar 5 inches\r\n" };
#endif

#ifdef ANTENA2 //cilindrica chica
const char s_antena [] = { "ant2,005.70,011.10,002.80,065.00\r\n" };
const char s_name [] = { "name:Wrist Tunnel\r\n" };
#endif

#ifdef ANTENA3 //cilindrica mediana
const char s_antena [] = { "ant3,003.50,019.00,003.50,065.00\r\n" };
const char s_name [] = { "name:Tunnel 6 inches\r\n" };
#endif

#ifdef ANTENA4 //cilindrica grande
const char s_antena [] = { "ant4,004.00,022.60,003.50,065.00\r\n" };
const char s_name [] = { "name:Tunnel 8 inches\r\n" };
#endif

#ifdef ANTENA4B //cilindrica grande
const char s_antena [] = { "ant4,002.50,021.00,003.50,065.00\r\n" };
const char s_name [] = { "name:Tunnel 10 inches\r\n" };
#endif

#ifdef ANTENA4C //cilindrica grande 3 capas 0.9, 4 capas 0.7
const char s_antena [] = { "ant4,006.60,056.50,003.50,065.00\r\n" };
const char s_name [] = { "name:Tunnel 10 inches\r\n" };
#endif

#ifdef ANTENA4D    //tunnel 10" 6 capas 1.2mm
const char s_antena [] = { "ant4,010.10,128.10,003.50,065.00\r\n" };
const char s_name [] = { "name:Tunnel 10 inches\r\n" };
#endif

#ifdef ANTENA4E    //tunnel 10" 4 capas 1.2mm 750T, 10-01-2023
const char s_antena [] = { "ant4,008.50,100.00,003.50,065.00\r\n" };
const char s_name [] = { "name:Tunnel 10 inches\r\n" };
#endif

#ifdef ANTENA4F    //tunnel 10" 4 capas 1.2mm 750T, 10-01-2023
const char s_antena [] = { "ant4,008.50,100.00,003.50,065.00\r\n" };
const char s_name [] = { "name:Head Tunnel 10\"\r\n" };
#endif

#ifdef ANTENA4G    //tunnel 8" 4 capas 1mm 1000T, 10-01-2023
const char s_antena [] = { "ant4,015.30,155.00,003.50,065.00\r\n" };
const char s_name [] = { "name:Tunnel 8 inches\r\n" };
#endif

#ifdef ANTENA4H    //tunnel 5" 4 capas 0.75mm 1000T, 10-01-2023
const char s_antena [] = { "ant4,014.30,083.00,002.00,065.00\r\n" };
const char s_name [] = { "name:Tunnel 5 inches\r\n" };
#endif

#ifdef ANTENA5 //cilindrica muy chica OJOS
const char s_antena [] = { "ant5,052.00,130.00,000.60,065.00\r\n" };
#endif

#ifdef ANTENA6 //cilindrica vieja de madera
const char s_antena [] = { "ant6,010.60,078.20,002.00,065.00\r\n" };
#endif

#ifdef ANTENA7A //pencil tunel
const char s_antena [] = { "ant7,012.70,064.80,002.00,065.00\r\n" };
#endif

#ifdef ANTENA8 //pencil plato doble
const char s_antena [] = { "ant8,022.60,157.20,001.10,065.00\r\n" };
#endif

//antena, R [ohms], L [mHy], Imax [A], Tmax [Â°C] todos 000.00
#ifdef ANTENAA9 //toroidal diametro mediana antena nueva DE=110 DI=45
const char s_antena [] = { "anta,023.80,133.00,001.10,065.00\r\n" };
#endif

#ifdef ANTENAA1 //antenas Ernesto viejas CHICOS
const char s_antena [] = { "anta,008.60,045.00,002.50,065.00\r\n" };
#endif

#ifdef ANTENAA2 //antenas Ernesto viejas CHICOS
const char s_antena [] = { "anta,009.00,051.40,003.20,065.00\r\n" };
#endif

#ifdef ANTENAA3 //antenas Ernesto T02
const char s_antena [] = { "anta,007.20,041.40,004.00,065.00\r\n" };
#endif

#ifdef ANTENAA4 //antenas Ernesto viejas CHICOS
const char s_antena [] = { "anta,022.60,157.20,001.10,065.00\r\n" };
#endif

#ifdef ANTENAA5 //antenas Ernesto viejas CHICOS
const char s_antena [] = { "anta,022.60,157.20,001.10,065.00\r\n" };
#endif

#ifdef ANTENAA6 //antenas Ernesto tunel grande 350mm dia
const char s_antena [] = { "anta,009.00,051.00,003.50,065.00\r\n" };
#endif

#ifdef ANTENAA7 //antenas Ernesto tunel mediano 240mm dia
const char s_antena [] = { "anta,009.00,045.00,003.50,065.00\r\n" };
#endif

#ifdef ANTENAA8 //antenas Ernesto tunel chico 150mm dia
const char s_antena [] = { "anta,010.60,007.82,002.00,065.00\r\n" };
#endif

#ifdef ANTENAB0 //antenas Ernesto tunel 5,6ohm 15,4mHy 08-08-17
const char s_antena [] = { "anta,005.60,015.40,003.00,065.00\r\n" };
#endif

#ifdef ANTENAB1 //antenas Ernesto tunel 08-08-17
const char s_antena [] = { "anta,002.70,019.40,003.00,065.00\r\n" };
#endif

#ifdef ANTENAB2 //antenas Ernesto tunel 08-08-17
const char s_antena [] = { "anta,003.50,023.25,003.00,065.00\r\n" };
#endif

#ifdef ANTENAB3 //antenas Ernesto tunel 08-08-17
const char s_antena [] = { "anta,002.20,021.71,003.00,065.00\r\n" };
#endif

#ifdef ANTENAB4 //antenas plana companiera de pencil 26-04-18
// const char s_antena [] = { "anta,031.10,220.00,001.10,065.00\r\n" };
const char s_antena [] = { "anta,031.10,150.00,001.10,065.00\r\n" };    //lo baje para que me deje salr cuadrada en 60Hz
#endif

#ifdef ANTENAB5
const char s_antena [] = { "anta,147.00,180.00,000.32,055.00\r\n" };
const char s_name [] = { "name:GT Googles 1\r\n" };
#endif

#ifdef ANTENAB6
const char s_antena [] = { "anta,061.00,063.00,000.64,055.00\r\n" };
const char s_name [] = { "name:GT Googles\r\n" };
#endif


// Module Private Functions ----------------------------------------------------
void SysTickError (void);
void TimingDelay_Decrement (void);


//-------------------------------------------//
// @brief  Main program.
// @param  None
// @retval None
//------------------------------------------//
int main(void)
{
    char str1 [40];
    unsigned short ts_cal1, ts_cal2;
    int temp = 0;
    unsigned char state = 0;
    unsigned char a = 0;
    unsigned char answer = 0;

    //GPIO Configuration.
    GPIO_Config();

    //Start the SysTick Timer
#if (defined CLOCK_FREQ_64_MHZ)
    if (SysTick_Config(64000))
        SysTickError();
#elif (defined CLOCK_FREQ_16_MHZ)
    if (SysTick_Config(16000))
        SysTickError();
#else
#error "No Clock Frequency defined on hard.h"
#endif
    
    //--- Funciones de Test Hardware ---
    TF_Hardware_Tests ();    
    //--- Fin Funciones de Test Hardware ---    
    
    //TIM Configuration.
    TIM_3_Init();

    //UART configuration.
    USART1Config();

    //ADC configuration.
    AdcConfig();

    // factory calibration of temp sensor
    ts_cal1 = *((uint16_t*)0x1FFFF7B8);
    // ts_cal1 = *((uint16_t*)0x1FFFF7B8);
    // ts_cal2 = *((uint16_t*)0x1FFFF7C2);

    Usart1RxDisable();

    USART1Send("\r\nts_cal1: ");
    memset(str1, 0, sizeof(str1));
    sprintf(str1, "%d", ts_cal1);
    USART1Send(str1);
        
    Wait_ms(100);

    GetTempInit();
    sprintf(str1, "\r\ncurrent temp: %d\r\n", GetTemp());
    USART1Send(str1);
    Wait_ms(100);
        
    Usart1RxEnable();
    Wait_ms(1900);

    //--- Main loop ---//
    while(1)
    {
        //PROGRAMA DE PRODUCCION
        switch (state)
        {
        case STAND_BY:
            //envio la info de la antena cada 1 seg hasta que me contesten
            LED_COMM_OFF;
            if (!timer_standby)
            {
                LED_COMM_ON;
                state = TX_SERIE_NC;
                Usart1RxDisable();
                USART1Send((char *)s_antena);
            }

            if (buffrx_ready)
            {
                state = RX_SERIE;
                buffrx_ready = 0;
            }
            break;

        case CONNECT:
            //cuando se agota timer_1_seg salgo a STAND_BY
            //me fijo si debo contestar algo
            LED_COMM_ON;
            if (answer == KEEP_ALIVE)
            {
                Wait_ms(5);
                answer = 0;
                LED_COMM_OFF;
                state = TX_SERIE;
                //apago RX
                Usart1RxDisable();
                USART1Send((char *)s_ok);
            }

            if (answer == GET_PARAMS)
            {
                Wait_ms(5);
                answer = 0;
                LED_COMM_OFF;
                state = TX_SERIE;
                //apago RX
                Usart1RxDisable();
                USART1Send((char *)s_antena);
            }

            if (answer == GET_NAME)
            {
                Wait_ms(5);
                answer = 0;
                LED_COMM_OFF;
                state = TX_SERIE;
                //apago RX
                Usart1RxDisable();
                USART1Send((char *)s_name);
            }

            if (answer == GET_TEMP)
            {
                Wait_ms(5);
                LED_COMM_OFF;

                temp = GetTemp();

                //reviso errores de conversion
                if ((temp >= 0) && (temp <= 85))
                {
                    memset(str1, 0, sizeof(str1));
                    sprintf(str1, "temp,%03d.00\r\n", temp);
                    //apago RX
                    Usart1RxDisable();
                    USART1Send(str1);

                    answer = 0;
                    state = TX_SERIE;
                }
            }

            if ((buffrx_ready) && (state == CONNECT))
            {
                state = RX_SERIE;
                buffrx_ready = 0;
            }

            if (!timer_1seg)	//mas de 10 segs sin comunicacion
            {
                state = STAND_BY;
            }
            break;

        case TX_SERIE:
            //espero terminar de transmitir
            if (!(USART1->CR1 & 0x80))
            {
                state = TX_SERIE2;
                timer_standby = 2;
            }
            break;

        case TX_SERIE2:
            if (!timer_standby)
            {
                state = CONNECT;
                LED_COMM_ON;
                Usart1RxEnable();
            }
            break;

        case TX_SERIE_NC:
            //espero terminar de transmitir
            if (!(USART1->CR1 & 0x80))
            {
                state = TX_SERIE2_NC;
                timer_standby = 2;
            }
            break;

        case TX_SERIE2_NC:
            if (!timer_standby)
            {
                state = STAND_BY;
                timer_standby = 1000;
                LED_COMM_OFF;
                Usart1RxEnable();
            }
            break;

        case RX_SERIE:
            //reviso que me llego, igual paso al estado conectado
            //si entiendo el mensaje
            answer = 0;
            a = InterpretarMsg ((char *)pbuffrx_cpy);
            //a = InterpretarMsg ((char *)pbuffrx);

            switch (a)
            {
            case GET_PARAMS:
                answer = GET_PARAMS;
                timer_1seg = 10;
                break;

            case GET_TEMP:
                answer = GET_TEMP;
                timer_1seg = 10;
                break;

            case GET_NAME:
                answer = GET_NAME;
                timer_1seg = 10;
                break;
                
            case SET_DISPLAY:
                break;

            case CMD_DISPLAY:
                break;

            case KEEP_ALIVE:
                answer = KEEP_ALIVE;
                timer_1seg = 10;
                break;

            case ERROR:
            default:
                //rx_error++;
                //state = STAND_BY;
                break;
            }
            if (timer_1seg == 0)
                state = STAND_BY;
            else
                state = CONNECT;

            break;

        case TEMP_SENSE:
            break;

        default:
            state = STAND_BY;
            break;
        }
    }

    return 0;
}

    
void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}


void TimingDelay_Decrement(void)
{
    TIM_Timeouts ();
    
    if (timer_standby)
        timer_standby--;

    // if (timer_led)
    //     timer_led--;

}


//--- end of file ---//
