//-----------------------------------------------------
// #### PROJECT: Magnet Antenna  F030 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### COMM.C ########################################
//-----------------------------------------------------

#include "comms.h"
#include <string.h>


// Externals -------------------------------------------------------------------
extern const char s_ok [];


// Globals ---------------------------------------------------------------------
const char s_get_gauss [] = {"get_gauss"};
const char s_get_temp [] = {"get_temp"};
const char s_get_params [] = {"get_params"};
const char s_channel [] = {"channel"};
const char s_set_display [] = {"set_display"};
const char s_cmd_display [] = {"cmd_display"};
const char s_keepalive [] = {"keepalive"};
const char s_get_name [] = {"get_name"};


// Module Functions ------------------------------------------------------------
unsigned char InterpretarMsg (char * pStr)
{

    //if (strncmp(s_get_params, pStr, sizeof(s_get_params) - 1) == 0)
    if (strncmp(pStr, s_get_params, sizeof(s_get_params) - 1) == 0)
        return GET_PARAMS;

    //if (strncmp(s_get_temp, pStr, sizeof(s_get_temp) - 1) == 0)
    if (strncmp(pStr, s_get_temp, sizeof(s_get_temp) - 1) == 0)
        return GET_TEMP;

    if (strncmp(s_set_display, pStr + 1, sizeof(s_set_display) - 1) == 0)
        return SET_DISPLAY;

    //if (strncmp(s_keepalive, pStr, sizeof(s_keepalive) - 1) == 0)
    if (strncmp(pStr, s_keepalive, sizeof(s_keepalive) - 1) == 0)
        return KEEP_ALIVE;

    if (strncmp(pStr, s_get_name, sizeof(s_get_name) - 1) == 0)
        return GET_NAME;
        
//	if (strncmp(s_cmd_display, pStr + 1, sizeof(s_cmd_display) - 1) == 0)
//	{
//		memcpy(pc, pStr + sizeof(s_cmd_display) + 4, (a - sizeof(s_cmd_display) - 4));
//		return CMD_DISPLAY;
//	}
    //ahora tengo el comando y ademas el payload en pc

    return ERROR;
}

