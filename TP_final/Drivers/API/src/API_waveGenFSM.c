/*
 * waveGenFSM.c
 *
 *  Created on: Aug 3, 2023
 *      Author: fm
 */

#include "API_waveGenFSM.h"

static waveGenFSMstate_t actualState;

void waveGenFSM_init()
{
	actualState = STOP;
}

void waveGenFSM_update()
{
	switch (actualState)
	{
		case STOP:

		break;
		case PLAY:

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
