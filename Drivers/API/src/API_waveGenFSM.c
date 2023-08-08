/*
 * waveGenFSM.c
 *
 *  Created on: Aug 3, 2023
 *      Author: fm
 */

#include "API_waveGenFSM.h"
#include "API_uart.h"
#include "API_delay.h"
#include "main.h"
#include "string.h"

static uint8_t  sComm[10] = {0,0,0,0,0,0,0,0,0,0};
static uint8_t* pComm;
static uint8_t  i;

static const uint8_t comm_play[] = "*PLAY";
static const uint8_t comm_stop[] = "*STOP";
static const uint8_t comm_menu[] = "*MENU";
static const uint8_t comm_sCH1[] = "*CH01";
static const uint8_t comm_sCH2[] = "*CH02";
static const uint8_t comm_cfgs[] = "*L123";
static const uint8_t comm_back[] = "*BACK";


static waveGenFSMstate_t actualState;

void waveGenFSM_init()
{
	actualState = STOP;
}

void waveGenFSM_update()
{
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
				HAL_GPIO_TogglePin(LED03_GPIO_Port, LED03_Pin);
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
