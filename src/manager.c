//-------------------------------------------------------
// #### PROJECT: Magnet Antenna  G030 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MANAGER.C #######################################
//-------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "manager.h"
#include "adc.h"
#include "usart.h"
#include "tim.h"


#include <stdio.h>
#include <string.h>

// Module Private Types & Macros -----------------------------------------------
typedef enum {
    STAND_BY,
    CONNECT,
    TX_SERIE,
    TX_SERIE2,
    TX_SERIE_NC,
    TX_SERIE2_NC,
    RX_SERIE,
    TEMP_SENSE
    
} manager_states_e;


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
manager_states_e mngr_state = STAND_BY;
volatile unsigned short timer_mngr = 0;
volatile unsigned char timer_1sec_mngr = 0;
volatile unsigned short millis = 0;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Manager (char * ant_name, char * ant_params)
{
    switch (mngr_state)
    {
    case STAND_BY:
        // send antenna params on each second waiting for answers
        Led_Off();

        if (!timer_mngr)
        {
            Led_On();
            mngr_state = TX_SERIE_NC;
            Usart1RxDisable();
            USART1Send((char *)s_antena);
        }

        if (Usart1HaveData())
        {
            mngr_state = RX_SERIE;
            Usart1HaveDate_Reset();
        }
        break;

    case CONNECT:
        //cuando se agota timer_1_seg salgo a STAND_BY
        //me fijo si debo contestar algo
        Led_On();
        if (answer == KEEP_ALIVE)
        {
            Wait_ms(5);
            answer = 0;
            Led_Off();
            mngr_state = TX_SERIE;
            //apago RX
            Usart1RxDisable();
            USART1Send((char *)s_ok);
        }

        if (answer == GET_PARAMS)
        {
            Wait_ms(5);
            answer = 0;
            Led_Off();
            mngr_state = TX_SERIE;
            //apago RX
            Usart1RxDisable();
            USART1Send((char *)s_antena);
        }

        if (answer == GET_NAME)
        {
            Wait_ms(5);
            answer = 0;
            Led_Off();
            mngr_state = TX_SERIE;
            //apago RX
            Usart1RxDisable();
            USART1Send((char *)s_name);
        }

        if (answer == GET_TEMP)
        {
            Wait_ms(5);
            Led_Off();

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
                mngr_state = TX_SERIE;
            }
        }

        if ((Usart1HaveData()) && (mngr_state == CONNECT))
        {
            mngr_state = RX_SERIE;
            Usart1HaveDate_Reset();
        }

        if (!timer_1seg)	//mas de 10 segs sin comunicacion
        {
            mngr_state = STAND_BY;
        }
        break;

    case TX_SERIE:
        //espero terminar de transmitir
        if (Usart1EndOfTx())
        {
            mngr_state = TX_SERIE2;
            timer_mngr = 2;
        }
        break;

    case TX_SERIE2:
        if (!timer_mngr)
        {
            mngr_state = CONNECT;
            Led_On();
            Usart1RxEnable();
        }
        break;

    case TX_SERIE_NC:
        //espero terminar de transmitir
        if (Usart1EndOfTx())
        {
            mngr_state = TX_SERIE2_NC;
            timer_mngr = 2;
        }
        break;

    case TX_SERIE2_NC:
        if (!timer_mngr)
        {
            mngr_state = STAND_BY;
            timer_mngr = 1000;
            Led_Off();
            Usart1RxEnable();
        }
        break;

    case RX_SERIE:
        //reviso que me llego, igual paso al estado conectado
        //si entiendo el mensaje
        answer = 0;
        a = Comms_CheckMsg ((char *)pbuffrx_cpy);

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
            //mngr_state = STAND_BY;
            break;
        }
        if (timer_1seg == 0)
            mngr_state = STAND_BY;
        else
            mngr_state = CONNECT;

        break;

    case TEMP_SENSE:
        break;

    default:
        mngr_state = STAND_BY;
        break;
    }
    
//     sw_actions_t action = do_nothing;
//     resp_t resp = resp_continue;
    
//     switch (mngr_mngr_state)
//     {
//     case INIT:
//         // hardware reset
//         DMX_Disable();

//         // channels reset
//         FiltersAndOffsets_Filters_Reset();

//         // start and clean filters
//         DisconnectChannels();
        
// #ifdef USART_DEBUG_MODE            
//         sprintf(s_to_send, "prog type: %d\n", pmem->program_type);
//         UsartDebug(s_to_send);
//         Wait_ms(100);
// #endif

//         // Init Program Screen
//         strcpy(s_to_send, "         ");
//         strcpy(s_to_send + 20, "         ");
//         switch (pmem->program_type)
//         {
//         case DMX1_MODE:
//             strcpy(s_to_send, "  DMX1 ");
//             break;
//         case DMX2_MODE:
//             strcpy(s_to_send, "  DMX2 ");
//             break;
//         case MASTER_SLAVE_MODE:
//             strcpy(s_to_send, "  Master ");
//             strcpy(s_to_send + 20, "    Slave");                
//             break;
//         case MANUAL_MODE:
//             strcpy(s_to_send, "  Manual ");
//             break;

//         case RESET_MODE:
//             strcpy(s_to_send, "   Reset ");
//             break;
                
//         }
            
//         SCREEN_ShowText2(
//             " Running ",
//             " on      ",
//             s_to_send,
//             s_to_send + 20
//             );
//         timer_mngr = 500;
    
//         while (timer_mngr)
//             display_update_int_mngr_state_machine();

//         mngr_mngr_state++;            
//         break;

//     case GET_CONF:

//         if (pmem->program_type == DMX1_MODE)
//         {
//             //reception variables
//             Packet_Detected_Flag = 0;
//             DMX_channel_selected = pmem->dmx_first_channel;
//             DMX_channel_quantity = pmem->dmx_channel_quantity;

//             //Mode Timeout enable
//             ptFTT = &DMX1Mode_UpdateTimers;

//             //packet reception enable
//             DMX_EnableRx();

//             //enable int outputs
//             FiltersAndOffsets_Enable_Outputs();
                
//             DMX1ModeReset();
//             mngr_mngr_state = MNGR_DMX1_MODE;
//         }

//         if (pmem->program_type == DMX2_MODE)
//         {
//             //reception variables
//             Packet_Detected_Flag = 0;
//             DMX_channel_selected = pmem->dmx_first_channel;
//             DMX_channel_quantity = 4 + pmem->dmx_channel_quantity;

//             //Mode Timeout enable
//             ptFTT = &DMX2Mode_UpdateTimers;

//             //packet reception enable
//             DMX_EnableRx();

//             //enable int outputs
//             FiltersAndOffsets_Enable_Outputs();            
                
//             DMX2ModeReset();
//             mngr_mngr_state = MNGR_DMX2_MODE;
//         }
            
//         if (pmem->program_type == MASTER_SLAVE_MODE)
//         {
//             //reception variables for slave mode
//             Packet_Detected_Flag = 0;
//             DMX_channel_selected = pmem->dmx_first_channel;
//             DMX_channel_quantity = pmem->dmx_channel_quantity;

//             //enable int outputs
//             FiltersAndOffsets_Enable_Outputs();            

//             //Mode Timeout enable
//             ptFTT = &MasterSlaveMode_UpdateTimers;
                
//             MasterSlaveModeReset();
                
//             mngr_mngr_state = MNGR_MASTER_SLAVE_MODE;
//         }

//         if (pmem->program_type == MANUAL_MODE)
//         {
//             //enable int outputs
//             FiltersAndOffsets_Enable_Outputs();

//             //Mode Timeout enable
//             ptFTT = &ManualMode_UpdateTimers;
                
//             ManualModeReset();
                
//             mngr_mngr_state = MNGR_MANUAL_MODE;
//         }

//         if (pmem->program_type == RESET_MODE)
//         {                
//             ResetModeReset();                
//             mngr_mngr_state = MNGR_RESET_MODE;
//         }
//         break;

//     case MNGR_DMX1_MODE:
//         // Check encoder first
//         action = CheckActions();
            
//         resp = DMX1Mode (ch_values, action);

//         if (resp == resp_change)
//         {
//             FiltersAndOffsets_Channels_to_Backup(ch_values);
//         }

//         if (resp == resp_need_to_save)
//         {
//             need_to_save_timer = 10000;
//             need_to_save = 1;
//         }

//         if (CheckSET() > SW_MIN)
//             mngr_mngr_state = MNGR_ENTERING_MAIN_MENU;
            
//         UpdateEncoder();

            
// #ifdef USART_DEBUG_MODE
//         if (!timer_mngr)
//         {
//             timer_mngr = 1000;

//             sprintf(s_to_send, "c1: %d, c2: %d, c3: %d, c4: %d, c5: %d, c6: %d\n",
//                     *(ch_values + 0),
//                     *(ch_values + 1),
//                     *(ch_values + 2),
//                     *(ch_values + 3),
//                     *(ch_values + 4),
//                     *(ch_values + 5));
//             UsartDebug(s_to_send);

//             sprintf(s_to_send, "d1: %d, d2: %d, d3: %d, d4: %d, d5: %d, d6: %d\n",
//                     dac_chnls[0],
//                     dac_chnls[1],
//                     dac_chnls[2],
//                     dac_chnls[3],
//                     dac_chnls[4],
//                     dac_chnls[5]);
//             UsartDebug(s_to_send);

//             sprintf(s_to_send, "p1: %d, p2: %d, p3: %d, p4: %d, p5: %d, p6: %d\n",
//                     pwm_chnls[0],
//                     pwm_chnls[1],
//                     pwm_chnls[2],
//                     pwm_chnls[3],
//                     pwm_chnls[4],
//                     pwm_chnls[5]);
//             UsartDebug(s_to_send);
//         }
// #endif
            
//         break;

//     case MNGR_DMX2_MODE:
//         // Check encoder first
//         action = CheckActions();
            
//         resp = DMX2Mode (ch_values, action);

//         if (resp == resp_change)
//         {
//             FiltersAndOffsets_Channels_to_Backup(ch_values);
//         }

//         if (resp == resp_need_to_save)
//         {
//             need_to_save_timer = 10000;
//             need_to_save = 1;
//         }

//         if (CheckSET() > SW_MIN)
//             mngr_mngr_state = MNGR_ENTERING_MAIN_MENU;
            
//         UpdateEncoder();            

            
// #ifdef USART_DEBUG_MODE
//         if (!timer_mngr)
//         {
//             timer_mngr = 1000;

//             sprintf(s_to_send, "c1: %d, c2: %d, c3: %d, c4: %d, c5: %d, c6: %d\n",
//                     *(ch_values + 0),
//                     *(ch_values + 1),
//                     *(ch_values + 2),
//                     *(ch_values + 3),
//                     *(ch_values + 4),
//                     *(ch_values + 5));
//             UsartDebug(s_to_send);

//             sprintf(s_to_send, "d1: %d, d2: %d, d3: %d, d4: %d, d5: %d, d6: %d\n",
//                     dac_chnls[0],
//                     dac_chnls[1],
//                     dac_chnls[2],
//                     dac_chnls[3],
//                     dac_chnls[4],
//                     dac_chnls[5]);
//             UsartDebug(s_to_send);

//             sprintf(s_to_send, "p1: %d, p2: %d, p3: %d, p4: %d, p5: %d, p6: %d\n",
//                     pwm_chnls[0],
//                     pwm_chnls[1],
//                     pwm_chnls[2],
//                     pwm_chnls[3],
//                     pwm_chnls[4],
//                     pwm_chnls[5]);
//             UsartDebug(s_to_send);
            
//         }
// #endif
            
//         break;
//     case MNGR_MASTER_SLAVE_MODE:
//         // Check encoder first
//         action = CheckActions();

//         resp = MasterSlaveMode (pmem, action);

//         if ((resp == resp_change) ||
//             (resp == resp_change_all_up))    //fixed mode save and change
//         {
//             //TODO: check how to do this in the new version data512?
//             // data512[0] = 0;
//             // for (unsigned char n = 0; n < sizeof(ch_values); n++)
//             // {
//             //     ch_values[n] = pmem->fixed_channels[n];
//             //     data512[n + 1] = ch_values[n];
//             // }

//             FiltersAndOffsets_Channels_to_Backup (ch_values);
            
//             if (resp == resp_change_all_up)    //fixed mode changes will be saved
//                 resp = resp_need_to_save;                
//         }

//         if (!timer_mngr)
//         {
//             if ((pmem->program_inner_type == MASTER_INNER_FIXED_MODE) ||
//                 (pmem->program_inner_type == MASTER_INNER_SKIPPING_MODE) ||
//                 (pmem->program_inner_type == MASTER_INNER_GRADUAL_MODE) ||
//                 (pmem->program_inner_type == MASTER_INNER_STROBE_MODE))
//             {
//                 timer_mngr = 40;
//                 SendDMXPacket (PCKT_INIT);
//             }
//         }

//         if (resp == resp_need_to_save)
//         {
//             need_to_save_timer = 10000;
//             need_to_save = 1;
//         }

//         if (CheckSET() > SW_MIN)
//             mngr_mngr_state = MNGR_ENTERING_MAIN_MENU;

//         UpdateEncoder();
            
//         break;

//     case MNGR_MANUAL_MODE:
//         // Check encoder first
//         action = CheckActions();

//         resp = ManualMode (pmem, action);

//         if ((resp == resp_change) ||
//             (resp == resp_change_all_up))    //fixed mode save and change
//         {
//             for (unsigned char n = 0; n < sizeof(ch_values); n++)
//                 ch_values[n] = pmem->fixed_channels[n];

//             FiltersAndOffsets_Channels_to_Backup (ch_values);
            
//             if (resp == resp_change_all_up)
//                 resp = resp_need_to_save;
//         }

//         if (resp == resp_need_to_save)
//         {
//             need_to_save_timer = 10000;
//             need_to_save = 1;
//         }

//         if (CheckSET() > SW_MIN)
//             mngr_mngr_state = MNGR_ENTERING_MAIN_MENU;

//         UpdateEncoder();
            
//         break;


//     case MNGR_RESET_MODE:
//         // Check encoder first
//         action = CheckActions();

//         resp = ResetMode (pmem, action);

//         if (resp == resp_finish)
//         {
//             // if (pmem->program_type == RESET_MODE)    //not save, go to main menu again
//             mngr_mngr_state = MNGR_ENTERING_MAIN_MENU;
//         }

//         if (resp == resp_need_to_save)
//         {
//             need_to_save_timer = 100;    //save almost instantly
//             need_to_save = 1;
//             mngr_mngr_state = INIT;
//         }

//         if (CheckSET() > SW_MIN)
//             mngr_mngr_state = MNGR_ENTERING_MAIN_MENU;

//         UpdateEncoder();
            
//         break;

//     case MNGR_ENTERING_MAIN_MENU:
//         //deshabilitar salidas hardware
//         DMX_Disable();
            
//         //reseteo canales
//         DisconnectChannels();

//         MainMenuReset();

//         SCREEN_ShowText2(
//             "Entering ",
//             " Main    ",
//             "  Menu   ",
//             "         "
//             );
            
//         mngr_mngr_state++;
//         break;

//     case MNGR_ENTERING_MAIN_MENU_WAIT_FREE:
//         if (CheckSET() == SW_NO)
//         {
//             mngr_mngr_state++;
//         }
//         break;
            
//     case MNGR_IN_MAIN_MENU:
//         // Check encoder first
//         action = CheckActions();

//         resp = MainMenu(pmem, action);

//         if (resp == resp_need_to_save)
//         {
// #ifdef SAVE_FLASH_IMMEDIATE
//             need_to_save_timer = 0;
// #endif
// #ifdef SAVE_FLASH_WITH_TIMEOUT
//             need_to_save_timer = 10000;
// #endif
//             need_to_save = 1;
//             mngr_mngr_state = INIT;
//         }
            
//         if (resp == resp_finish)
//             mngr_mngr_state = INIT;

//         UpdateEncoder();

//         if (CheckSET() > SW_HALF)
//             mngr_mngr_state = MNGR_ENTERING_HARDWARE_MENU;
            
//         break;

//     case MNGR_ENTERING_HARDWARE_MENU:
//         HardwareModeReset();

//         //Mode Timeout enable
//         ptFTT = &HardwareMode_UpdateTimers;
            

//         SCREEN_ShowText2(
//             "Entering ",
//             " Hardware",
//             "  Menu   ",
//             "         "
//             );
            
//         mngr_mngr_state++;
//         break;

//     case MNGR_ENTERING_HARDWARE_MENU_WAIT_FREE:
//         if (CheckSET() == SW_NO)
//         {
//             mngr_mngr_state++;
//         }
//         break;
            
//     case MNGR_IN_HARDWARE_MENU:
//         // Check encoder first
//         action = CheckActions();

//         resp = HardwareMode(pmem, action);

//         if ((resp == resp_need_to_save) ||
//             (resp == resp_finish))
//         {
//             //hardware config its saved instantly
//             need_to_save = 1;
//             mngr_mngr_state = INIT;
//         }

//         UpdateEncoder();
//         break;
            
//     default:
//         mngr_mngr_state = INIT;
//         break;
//     }

//     //cuestiones generales        
//     UpdateSwitches();

//     // update de LCD
//     display_update_int_mngr_state_machine();

//     // colors commands update from comms
//     UpdateCommunications();

//     // now call it by tim6 on pwm_handler int
//     // DAC_MUX_Update(dac_chnls);


// #ifdef USE_OVERTEMP_PROT
//     if (check_ntc)    //NTC NOT SHORTED
//     {
//         if ((mngr_mngr_state < MNGR_ENTERING_MAIN_MENU) &&
//             (!protections_sample_timer))
//         {
//             unsigned short temp_filtered = 0;
//             temp_filtered = MA16_U16Circular(&temp_filter, Temp_Channel);

//             if (CheckTempGreater (temp_filtered, pmem->temp_prot))
//             {
//                 //stop LEDs outputs
//                 DisconnectByVoltage();
//                 CTRL_FAN_ON;

//                 SCREEN_ShowText2(
//                     "LEDs     ",
//                     "Overtemp ",
//                     "         ",
//                     "         "
//                     );

// #ifdef USART2_DEBUG_MODE
//                 sprintf(s_to_send, "overtemp: %d\n", temp_filtered);
//                 Usart2Send(s_to_send);
// #endif

//                 do {
//                     display_update_int_mngr_state_machine();
//                 } while (CheckTempGreater (Temp_Channel, TEMP_RECONNECT));
                    
//                 //reconnect
//                 mngr_mngr_state = INIT;
//             }
//             else if (CheckTempGreater (temp_filtered, TEMP_IN_35))
//             {
//                 CTRL_FAN_ON;
//             }
//             else if (CheckTempGreater (TEMP_IN_30, temp_filtered))
//             {
//                 CTRL_FAN_OFF;
//             }
                

        }
    }    // check_ntc
#endif    // USE_OVERTEMP_PROT


    // save flash after configs
//     if ((need_to_save) && (!need_to_save_timer))
//     {
//         need_to_save = Flash_WriteConfigurations();

// #ifdef USART_DEBUG_MODE
//         if (need_to_save)
//             UsartDebug((char *) "Memory Saved OK!\n");
//         else
//             UsartDebug((char *) "Memory problems\n");
// #endif

//         need_to_save = 0;
//     }
        
}


void Manager_Timeouts (void)
{
    // if (need_to_save_timer)
    //     need_to_save_timer--;

    if (timer_mngr)
        timer_mngr--;    

    if (millis < 1000)
        millis++;
    else
    {
        millis = 0;
        if (timer_1sec_mngr)
            timer_1sec_mngr--;
    }
}


//--- end of file ---//

