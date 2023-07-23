//**********************************************************************************************************
//
// Practica 3 - Programacion de Microcontroladores (CESE 2023)
// Titulo: Modulo API_delay (HEADER)
// Autor: F.D.M.
//
//**********************************************************************************************************

#ifndef API_DELAY_H

#define CANT 3						// Define macro cantidad de leds
#define DURATION_MAX 86400000		// Define macro retardo maximo en [ms], equivale a un dia
#define DURATION_MIN 0				// Define macro retardo minimo en [ms], cero
#define DURATION_1 250				// Define macro retardo Led 1 en [ms]
#define DURATION_2 500				// Define macro retardo Led 2 en [ms]
#define DURATION_3 1000				// Define macro retardo Led 3 en [ms]

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{						// Define estructura para el manejo de retardos
	tick_t startTime;				//
	tick_t duration;				//
	bool_t running;					//
} delay_t;							//

//**********************************************************************************************************
// Prototipos del modulo API_delay
//**********************************************************************************************************
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );
void API_Error_Handler(void);

#endif
