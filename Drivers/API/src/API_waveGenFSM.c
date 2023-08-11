/*
 * waveGenFSM.c
 *
 *  Created on: Aug 3, 2023
 *      Author: fm
 */

#include "API_i2s.h"
#include "API_waveGenFSM.h"
#include "API_uart.h"
#include "API_delay.h"
#include "main.h"
#include "string.h"
#include "stdio.h"

static uint8_t  sComm[COMM_LENGHT_MAX * 2] = {0,0,0,0,0,0,0,0,0,0};
static uint8_t* pComm;
static uint8_t  i;

static const uint8_t comm_play[] = "*PLAY";
static const uint8_t comm_stop[] = "*STOP";
static const uint8_t comm_menu[] = "*MENU";
static const uint8_t comm_sCH0[] = "*CH00";
static const uint8_t comm_sCH1[] = "*CH01";
static const uint8_t comm_freq[] = "*FREQ";
static const uint8_t comm_back[] = "*BACK";

static uint8_t buffer[CHARACTERS_MAX];

static waveGenFSMstate_t actualState;

static bool_t readValueFreq();
static bool_t readValueAmp();

//**********************************************************************************************************
// Funcion :
//
//
//**********************************************************************************************************
void waveGenFSM_init()
{
	actualState = STOP;
	HAL_NVIC_DisableIRQ(SPI2_IRQn);
}

//**********************************************************************************************************
// Funcion :
//
//
//**********************************************************************************************************
void waveGenFSM_update()
{
	channel *ch0;
	channel *ch1;
	uint8_t m0[] = "SINUSOIDAL";
	uint8_t m1[] = "SAWTOOTH";
	uint8_t *p0, *p1;
	if(eventCommand())
	{
		pComm = readCommand();
		for(i=0;i<COMM_LENGHT_MAX;i++)
			sComm[i] = *pComm++;
	}
	switch (actualState)
	{
		case STOP:
			if(strstr((const char*)sComm, (const char*)comm_play))
			{
				actualState = PLAY;
				startI2S();
				ch0 = readChannelProperty(0);
				ch1 = readChannelProperty(1);
				if(ch0->wave_type == SINUSOIDAL)
					p0 = m0;
				else p0 = m1;
				if(ch1->wave_type == SINUSOIDAL)
					p1 = m0;
				else p1 = m1;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\r************** Status PLAY **************\n\r"
				                                        "Frequency : %d Hz\n\r"
														"Channel 0 : %s Amplitude : %d %%\n\r"
														"Channel 1 : %s Amplitude : %d %%\n\r",
														ch0->freq,p0,ch0->amplitude,p1,ch1->amplitude);
				uartSendString(buffer);
			}
			if(strstr((const char*)sComm, (const char*)comm_menu))
			{
				actualState = MENU_CONFIG;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\r************ Menu CONFIGURATION ************\n\r"
														"*FREQ\t*CH00\t*CH01\t*BACK\n\r");
				uartSendString(buffer);
			}
		break;
		case PLAY:
			if(strstr((const char*)sComm, (const char*)comm_stop))
			{
				actualState = STOP;
				HAL_NVIC_DisableIRQ(SPI2_IRQn);
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\r************** Status STOP **************\n\r"
														"*PLAY\t*MENU\n\r");
				uartSendString(buffer);
			}
		break;
		case MENU_CONFIG:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = STOP;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\r************** Status STOP **************\n\r"
																		"*PLAY\t*MENU\n\r");
				uartSendString(buffer);
			}
			if(strstr((const char*)sComm, (const char*)comm_freq))
			{
				actualState = INPUT_FREQ;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\rInput Frequency in Hz or *BACK (20 to 24000, use brackets to enter the value): ");
				uartSendString(buffer);
			}
			if(strstr((const char*)sComm, (const char*)comm_sCH0))
			{
				actualState = INPUT_AMP_CH0;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\rInput Amplitude in %% or *BACK (0 to 100, use brackets to enter the value): ");
				uartSendString(buffer);
			}
			if(strstr((const char*)sComm, (const char*)comm_sCH1))
			{
				actualState = INPUT_AMP_CH1;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\rInput Amplitude in %% or *BACK (0 to 100, use brackets to enter the value): ");
				uartSendString(buffer);
			}
		break;
		case INPUT_FREQ:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = MENU_CONFIG;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\r************ Menu CONFIGURATION ************\n\r"
														"*FREQ\t*CH00\t*CH01\t*BACK\n\r");
				uartSendString(buffer);
			}
			if(readValueFreq())
			{
				// Procesar valor
			}
		break;
		case INPUT_AMP_CH0:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = MENU_CONFIG;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\r************ Menu CONFIGURATION ************\n\r"
														"*FREQ\t*CH00\t*CH01\t*BACK\n\r");
				uartSendString(buffer);
			}
			if(readValueAmp())
			{
				// Procesar valor para Channel 0
			}
		break;
		case INPUT_AMP_CH1:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = MENU_CONFIG;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\r************ Menu CONFIGURATION ************\n\r"
														"*FREQ\t*CH00\t*CH01\t*BACK\n\r");
				uartSendString(buffer);
			}
			if(readValueAmp())
			{
				// Procesar valor para Channel 1
			}
		break;

		default:

		break;
	}
}

static bool_t readValueFreq()
{
	// debe leer el valor de frecuencia ingresado por UART
	return true;
}

static bool_t readValueAmp()
{
	// debe leer el valor de amplitud ingresado por UART
	return true;
}
