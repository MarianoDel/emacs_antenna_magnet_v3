//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32G030
// ##
// #### ADC.H #################################
//---------------------------------------------

#ifndef _ADC_H_
#define _ADC_H_

//----------- Defines For Configuration --------------//
//----------- Some ADC Configurations ----------------//
// #define ADC_WITH_INT
#define ADC_WITH_DMA

#ifdef WITH_TEMP_CONTROL
#define ADC_WITH_TEMP_SENSE
#endif

#ifdef ADC_WITH_TEMP_SENSE
#define SIZEOF_BOARD_TEMP		8
#endif
//----------- End of ADC Configurations --------------//

#define Temp_Channel    adc_ch[0]
#define ADC_CHANNEL_QUANTITY         1
#define ADC_LAST_CHANNEL_QUANTITY    (ADC_CHANNEL_QUANTITY - 1)

#define ADC_All_Orer_Channels    (ADC_Channel_0)


// Module Exported Functions ---------------------------------------------------
void AdcConfig (void);
unsigned short ReadADC1_SameSampleTime (unsigned int);
unsigned short ReadADC1Check (unsigned char);
unsigned int ADCGetCalibrationFactor (void);

#ifdef ADC_WITH_TEMP_SENSE
void UpdateTemp(void);
unsigned short GetTemp (void);
void FillTempBuffer (void);
short ConvertTemp (unsigned short);
#endif
#endif /* _ADC_H_ */
