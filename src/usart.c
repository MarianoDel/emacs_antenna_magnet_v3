//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### UART.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "hard.h"
#include "stm32g0xx.h"
#include "usart.h"

#include <string.h>


// Usart Configs ---------------------------------------------------------------
#define USE_USART1			
//#define USE_USART2

#define BUFFRX_DIM 64
#define BUFFTX_DIM 64


// Externals -------------------------------------------------------------------
#ifdef USE_USART1
extern volatile unsigned char buffrx_ready;
extern volatile unsigned char *pbuffrx;
extern volatile unsigned char *pbuffrx_cpy;
#endif

#ifdef USE_USART2
extern volatile unsigned char usart2_pckt_ready;
extern volatile unsigned char usart2_have_data;
//extern volatile unsigned char usart2_mini_timeout;
extern volatile unsigned char tx2buff[];
extern volatile unsigned char rx2buff[];
#endif


// Globals ---------------------------------------------------------------------
#ifdef USE_USART1
volatile unsigned char * ptx1;
volatile unsigned char * ptx1_pckt_index;
volatile unsigned char * prx1;

//Reception buffer.
unsigned char buffrx[BUFFRX_DIM];
unsigned char buffrx_cpy[BUFFRX_DIM];

//Transmission buffer.
unsigned char bufftx[BUFFTX_DIM];
unsigned char * pbufftx;
unsigned char * pbufftx2;
#endif


#ifdef USE_USART2
volatile unsigned char * ptx2;
volatile unsigned char * ptx2_pckt_index;
volatile unsigned char * prx2;
#endif


// Module Functions ------------------------------------------------------------
#ifdef USE_USART1
void USART1Config(void)
{
	if (!USART1_CLK)
		USART1_CLK_ON;

	// ptx1 = tx1buff;
	// ptx1_pckt_index = tx1buff;
	// prx1 = rx1buff;

	USART1->BRR = USART_9600;
	//USART1->CR2 |= USART_CR2_STOP_1;	//2 bits stop
//	USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
//	USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//SIN TX
	// USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;	//para pruebas TX
#ifdef VER_2_0
	USART1->CR2 |= USART_CR2_TXINV;	//tx invertido
#endif

	USART1->CR1 = USART_CR1_TE | USART_CR1_UE;	//para pruebas solo TX


	//habilito el pin
	GPIOA->AFR[0] = 0x00001100;	//PA2 -> AF1 PA3 -> AF1

	//seteo punteros
	pbuffrx = buffrx;
	pbufftx = bufftx;
	pbufftx2 = bufftx;

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 5);
}


void USART1_IRQHandler(void)
{
	unsigned char dummy;

	// if (LED)
	// 	LED_OFF;
	// else
	// 	LED_ON;

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART1->ISR & USART_ISR_RXNE)
	{
		dummy = USART1->RDR & 0x0FF;


		//Saving incoming data.
		*pbuffrx = dummy;

		if (pbuffrx < &buffrx[BUFFRX_DIM])
		{
			if (*pbuffrx == '\n')
			{

				*pbuffrx = 0;

				strcpy((char*)&buffrx_cpy[0], (const char *)&buffrx[0]);

				buffrx_ready = 1;
				pbuffrx = buffrx;
				pbuffrx_cpy = buffrx_cpy;
			}
			else
				pbuffrx++;

		}
		else
			pbuffrx = buffrx;
	}
	// else
	// 	USARTx->RQR |= 0x08;


	/* USART in mode Transmitter -------------------------------------------------*/
	if (USART1->CR1 & USART_CR1_TXEIE)
	{
		if (pbufftx2 < pbufftx)
		{
			USART1->TDR = *pbufftx2 & 0x0FF;
			pbufftx2++;
		}
		else
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;

			pbufftx2 = bufftx;
			pbufftx = bufftx;
		}

//		USARTx->RQR |= 0x08;
	}

	if ((USART1->ISR & USART_ISR_ORE) || (USART1->ISR & USART_ISR_NE) || (USART1->ISR & USART_ISR_FE))
	{
		USART1->ICR |= 0x0e;
		dummy = USART1->RDR;
	}
}


unsigned char USART1Send(char * send)
{
	unsigned char i;

	i = strlen(send);

	if ((pbufftx + i) < &bufftx[BUFFTX_DIM])
	{

		strcpy((char*)pbufftx, (const char *)send);
		pbufftx += i;

		USART1->CR1 |= USART_CR1_TXEIE;

	}
	else
		return END_ERROR;


	return WORKING;
}

void Usart1RxDisable (void)
{
	USART1->CR1 &= ~USART_CR1_RXNEIE;
	USART1->CR1 &= ~USART_CR1_RE;
}

void Usart1RxEnable (void)
{
	USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_RE;
}

#endif	//USE_USART1

#ifdef USE_USART2
void USART2Config(void)
{
	if (!USART2_CLK)
		USART2_CLK_ON;

	GPIOA->AFR[0] |= 0x0001100;	//PA2 -> AF1 PA3 -> AF1

	ptx2 = tx2buff;
	ptx2_pckt_index = tx2buff;
	prx2 = rx2buff;

	USART2->BRR = USART_115200;
	USART2->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;

	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 7);
}

void USART2_IRQHandler(void)
{
	unsigned char dummy;

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART2->ISR & USART_ISR_RXNE)
	{
		//RX WIFI
		dummy = USART2->RDR & 0x0FF;

#ifdef USE_GSM_GATEWAY
		if (prx2 < &rx2buff[SIZEOF_DATA])
		{
			*prx2 = dummy;
			prx2++;
			usart2_have_data = 1;
		}
			usart2_mini_timeout = TT_GPS_MINI;
#endif

	}

	/* USART in mode Transmitter -------------------------------------------------*/

	if (USART2->CR1 & USART_CR1_TXEIE)
	{
		if (USART2->ISR & USART_ISR_TXE)
		{
			if ((ptx2 < &tx2buff[SIZEOF_DATA]) && (ptx2 < ptx2_pckt_index))
			{
				USART2->TDR = *ptx2;
				ptx2++;
			}
			else
			{
				ptx2 = tx2buff;
				ptx2_pckt_index = tx2buff;
				USART2->CR1 &= ~USART_CR1_TXEIE;
			}
		}
	}

	if ((USART2->ISR & USART_ISR_ORE) || (USART2->ISR & USART_ISR_NE) || (USART2->ISR & USART_ISR_FE))
	{
		USART2->ICR |= 0x0e;
		dummy = USART2->RDR;
	}
}

void Usart2Send (char * send)
{
	unsigned char i;

	i = strlen(send);
	Usart2SendUnsigned((unsigned char *) send, i);
}

void Usart2SendUnsigned(unsigned char * send, unsigned char size)
{
	if ((ptx2_pckt_index + size) < &tx2buff[SIZEOF_DATA])
	{
		memcpy((unsigned char *)ptx2_pckt_index, send, size);
		ptx2_pckt_index += size;
		USART2->CR1 |= USART_CR1_TXEIE;
	}
}

void Usart2SendSingle(unsigned char tosend)
{
	Usart2SendUnsigned(&tosend, 1);
}

unsigned char ReadUsart2Buffer (unsigned char * bout, unsigned short max_len)
{
	unsigned int len;

	len = prx2 - rx2buff;

	if (len < max_len)
		memcpy(bout, (unsigned char *) rx2buff, len);
	else
	{
		len = max_len;
		memcpy(bout, (unsigned char *) rx2buff, len);
	}

	//ajusto punteros de rx luego de la copia
	prx2 = rx2buff;

	return (unsigned char) len;
}

#endif	///USE_USART2

//--- end of file ---//
