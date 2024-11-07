//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include <stdio.h>
#include <string.h>


// Types Constants and Macros --------------------------------------------------
#define HARD_VER    "4.0"
#define SOFT_VER    "1.0"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Functions to Test ----------------------------------------------------
void USART1Send (char * out);


// Module Auxiliary Functions --------------------------------------------------
void Test_Functions (void);


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    Test_Functions ();

    return 0;
}


void Test_Functions (void)
{
    char str1 [100];
    
    printf("Testing hard soft on init...\n");
    sprintf(str1, "Hard: %s Soft: %s\r\n",
            HARD_VER,
            SOFT_VER);
    USART1Send(str1);
}


void USART1Send (char * out)
{
    printf("usart tx -> %s", out);
}



//--- end of file ---//


