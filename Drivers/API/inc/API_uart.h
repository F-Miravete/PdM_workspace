//**********************************************************************************************************
//
// TP Final PdM y PCSE (CESE 2023)
// Titulo: Modulo API_uart (HEADER)
// Autor: F.D.M.
//
//**********************************************************************************************************

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdint.h>
#include "API_delay.h"

#define USARTx USART2				// Nombre de la USART utilizada
#define CHARACTERS_MAX 255			// Maxima cantidad de caracteres del buffer de recepcion UART
#define COMM_LENGTH_MAX 4			// Maxima cantidad de caracteres de un comando
#define VALUE_LENGTH_MAX 6			// Maxima cantidad de caracteres + 1 de un valor

typedef struct
{
	uint8_t n_digits;
	uint8_t* pointer_value;
} value_t;

bool_t uartInit();												// Inicializa la UART
void uartSendString(uint8_t * pstring);							// Envia un string por UART
void uartSendStringSize(uint8_t * pstring, uint16_t size);		// Envia "size" caracteres de un string por UART
bool_t eventCommand();											// Detecta la llegada de un comando por UART
bool_t eventValue();											// Detecta el ingreso de un valor por UART
uint8_t* readCommand();											// Lee un comando ingresado por UART
uint16_t readValue();											// Lee un valor ingresado por UART

#endif /* API_INC_API_UART_H_ */
