/* Includes ------------------------------------------------------------------*/


#ifndef API_DELAY_H

#define CANT 3
#define DURATION_MAX 86400000
#define DURATION_MIN 0

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{
tick_t startTime;
tick_t duration;
bool_t running;
} delay_t;

// void API_Error_Handler(void);
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );

#endif
