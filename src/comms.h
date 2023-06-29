//-----------------------------------------------------
// #### PROJECT: Magnet Antenna  F030 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### COMM.H ########################################
//-----------------------------------------------------

#ifndef COMM_H_
#define COMM_H_

// Exported Types Constants and Macros -----------------------------------------
#define GET_PARAMS	10
#define GET_TEMP 11
#define GET_GAUSS 12
#define CHANNEL	13
#define SET_DISPLAY	14
#define CMD_DISPLAY	15
#define KEEP_ALIVE	16
#define GET_NAME        17

#define ERROR	50

// Exported Module Functions ---------------------------------------------------
unsigned char InterpretarMsg (char *);


#endif /* COMM_H_ */
