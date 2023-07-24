/*
 * API_debounce.h
 *
 *  Created on: Jul 23, 2023
 *      Author: fm
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#define DURATION_DELAY 40		// Define duracion del retardo en 40 ms

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RISING,
} debounceState_t;

void debounceFSM_init(debounceState_t fsm);			// debe cargar el estado inicial
void debounceFSM_update(debounceState_t fsm);		// debe leer las entradas, resolver la lógica de
													// transición de estados y actualizar las salidas
bool_t readKey();									// Devuelve estado de tecla presionada

#endif /* API_INC_API_DEBOUNCE_H_ */
