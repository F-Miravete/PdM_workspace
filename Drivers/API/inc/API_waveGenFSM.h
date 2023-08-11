/*
 * API_waveGenFSM.h
 *
 *  Created on: Aug 3, 2023
 *      Author: fm
 */

#ifndef API_INC_API_WAVEGENFSM_H_
#define API_INC_API_WAVEGENFSM_H_



typedef enum{										// Define estructura para FSM de Generador de tonos
STOP,
PLAY,
MENU_CONFIG,
INPUT_AMP_CH0,
INPUT_AMP_CH1,
INPUT_FREQ
} waveGenFSMstate_t;

void waveGenFSM_init();
void waveGenFSM_update();

#endif /* API_INC_API_WAVEGENFSM_H_ */
