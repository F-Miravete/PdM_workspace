/*
 * API_debounce.h
 *
 *  Created on: Jul 23, 2023
 *      Author: FDM
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#define DURATION_DELAY 40							// Define duracion del retardo anti-rebote en 40 ms

typedef enum{										// Define estructura para FSM de anti-rebote
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RISING,
} debounceState_t;

//**********************************************************************************************************
// Prototipos del modulo API_debounce
//**********************************************************************************************************
void debounceFSM_init();							// Carga el estado inicial en FSM anti-rebote
void debounceFSM_update();							// Lee las entradas, resuelve la lógica de
													// transición de estados y actualizar las salidas de la FSM de anti-rebote
bool_t readKey();									// Devuelve estado de tecla presionada

#endif /* API_INC_API_DEBOUNCE_H_ */
