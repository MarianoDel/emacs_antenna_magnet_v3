//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### TEMP_SENSOR.C #########################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "temp_sensor.h"
#include "adc.h"
#include "dsp.h"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
ma8_u16_data_obj_t temp_filter;


// Module Private Functions ----------------------------------------------------
short ConvertTemp (unsigned short adc_temp);


// Module Functions ------------------------------------------------------------

//devuelve el valor promedio de la temperatura
short GetTemp (void)
{
    unsigned short new_temp_sample = 0;

    new_temp_sample = ReadADC1_SameSampleTime(ADC_Channel_TempSensor);
    new_temp_sample = MA8_U16Circular(&temp_filter, new_temp_sample);

    return ConvertTemp(new_temp_sample);
}


void GetTempInit (void)
{
    MA8_U16Circular_Reset (&temp_filter);

    unsigned short dummy = ReadADC1_SameSampleTime(ADC_Channel_TempSensor);

    for (unsigned char i = 0; i < 8; i++)
        MA8_U16Circular(&temp_filter, dummy);

}


short ConvertTemp (unsigned short adc_temp)
{
    //first convertion mode    
    // int32_t temperature; /* will contain the temperature in degree Celsius */
    // //temperature = (((int32_t) ADC1->DR * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
    // temperature = (int32_t) *TEMP30_CAL_ADDR - adc_temp;
    // temperature *= 1000;
    // temperature = temperature / 5336;	//4.3mV / °C
    // temperature = temperature + 30;

    //second convertion mode
    int32_t temperature; /* will contain the temperature in degree Celsius */
    //temperature = (((int32_t) ADC1->DR * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
    temperature = (((int32_t) adc_temp * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
    temperature = temperature * (int32_t)(110 - 30);
    temperature = temperature / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
    temperature = temperature + 30;
    

    return (short) temperature;
}


//--- end of file ---//


