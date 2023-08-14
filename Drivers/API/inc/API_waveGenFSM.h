/*
 * API_waveGenFSM.h
 *
 *  Created on: Aug 3, 2023
 *      Author: fm
 */

#ifndef API_INC_API_WAVEGENFSM_H_
#define API_INC_API_WAVEGENFSM_H_

#define CHANNEL_0 0
#define CHANNEL_1 1

typedef enum{										// Define estructura para FSM de Generador de tonos
STOP,
PLAY,
MENU_CONFIG,
SELECT_CH0,
SELECT_CH1,
INPUT_FREQ,
SET_AMP_CH0,
SET_AMP_CH1,
SET_WAVE_CH0,
SET_WAVE_CH1
} waveGenFSMstate_t;

void waveGenFSM_init();
void waveGenFSM_update();

#endif /* API_INC_API_WAVEGENFSM_H_ */
