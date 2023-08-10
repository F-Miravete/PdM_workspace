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

static uint8_t  sComm[COMM_LENGHT_MAX * 2] = {0,0,0,0,0,0,0,0,0,0};
static uint8_t* pComm;
static uint8_t  i;

static const uint8_t comm_play[] = "*PLAY";
static const uint8_t comm_stop[] = "*STOP";
static const uint8_t comm_menu[] = "*MENU";
static const uint8_t comm_sCH1[] = "*CH01";
static const uint8_t comm_sCH2[] = "*CH02";
static const uint8_t comm_cfgs[] = "*L123";
static const uint8_t comm_back[] = "*BACK";

static const uint8_t msg_play[] = "\n\r** Modo PLAY **\n\r";
static uint8_t buffer[CHARACTERS_MAX];

static waveGenFSMstate_t actualState;

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
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\r**** Modo PLAY ****\n\r"
				                                        "Frequency : %ld Hz\n\r"
														"Channel 0 : %s Amplitude : %ld %\n\r"
														"Channel 1 : %s Amplitude : %ld %\n\r",
														ch0->freq,p0,ch0->amplitude,p1,ch1->amplitude);
				uartSendString(buffer);
			}
			else
			{
				if(strstr((const char*)sComm, (const char*)comm_menu))
					actualState = MENU_CONFIG;
			}
		break;
		case PLAY:
			if(strstr((const char*)sComm, (const char*)comm_stop))
			{
				actualState = STOP;
				HAL_NVIC_DisableIRQ(SPI2_IRQn);
				HAL_GPIO_TogglePin(LED03_GPIO_Port, LED03_Pin);
			}
		break;
		case MENU_CONFIG:

		break;
		case SELECT_CH1:

		break;
		case SELECT_CH2:

		break;
		case CH1_CONFIG_VALID:

		break;
		case CH2_CONFIG_VALID:

		break;
		default:

		break;
	}
}
