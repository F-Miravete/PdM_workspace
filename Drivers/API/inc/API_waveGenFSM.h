//**********************************************************************************************************
//
// TP Final PdM y PCSE (CESE 2023)
// Titulo: Modulo API_waveGenFSM (HEADER)
// Autor: F.D.M.
//
//**********************************************************************************************************

#ifndef API_INC_API_WAVEGENFSM_H_
#define API_INC_API_WAVEGENFSM_H_

#define CHANNEL_0 0
#define CHANNEL_1 1

typedef enum{					// Define estructura para FSM de Generador de tonos
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

void waveGenFSM_init();			// Inicializa FSM
void waveGenFSM_update();		// Actualiza FSM

#endif /* API_INC_API_WAVEGENFSM_H_ */
