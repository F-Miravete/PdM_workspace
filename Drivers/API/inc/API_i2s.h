/*
 * API_i2s.h
 *
 *  Created on: Aug 9, 2023
 *      Author: z002m07j
 */


#ifndef API_INC_API_I2S_H_
#define API_INC_API_I2S_H_

#include <stdint.h>
#include "API_delay.h"

// Freq. sampling 96000 Hz
// Freq. Min. 10 Hz - Freq. Max. 24000 Hz
// Buffer size min. -> 96000/24000 = 4
// Buffer size max. -> 96000/20 = 4800
#define BUFFER_SIZE_MAX 4800
#define BUFFER_SIZE_MIN 4
#define SCALE_SIN_WAVE 16383
#define SCALE_SAW_WAVE 32767

typedef enum{
SINUSOIDAL,
SAWTOOTH
} wave_t;

typedef struct
{
	uint8_t n_ch;			// 0 o 1
	wave_t wave_type;		// SINUSOIDAL o SAWTOOTH
	uint8_t amplitude;		// 0 to 100 [%]
	uint16_t freq;			// 20 to 24000 [Hz]
} channel;

bool_t i2sInit();											// Inicializa el modulo I2S
void startI2S(void);										// Arranca la transmision I2S
void stopI2S(void);											// Detiene la transmision I2S
channel* readChannelProperty(uint8_t ch);					// Devuelve un puntero a la estrucutura channel para cada canal
void setFreqChannels(uint16_t freq);						// Usada cuando se ingresa un nuevo valor de frecuencia
void setAmpChannel(uint8_t n_channel, uint8_t amplitude);	// Usada cuando se ingresa un nuevo valor de amplitud
void setWaveChannel(uint8_t n_channel, wave_t wave_type);	// Usada cuando se ingresa una nueva forma de onda

#endif /* API_INC_API_I2S_H_ */
