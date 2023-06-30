//-------------------------------------------------------
// #### PROJECT: Magnet Antenna  G030 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS.H #########################################
//-------------------------------------------------------

#ifndef COMMS_H_
#define COMMS_H_


// Exported Types Constants and Macros -----------------------------------------
typedef enum {
    GET_PARAMS,
    GET_TEMP,
    KEEPALIVE,
    GET_NAME,
    ERROR
    
} comms_answers_e;


// Exported Module Functions ---------------------------------------------------
comms_answers_e Comms_CheckMsg (char * pStr);


#endif /* COMMS_H_ */
