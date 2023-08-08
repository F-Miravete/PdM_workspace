/*
 * API_uart.h
 *
 *  Created on: Jul 27, 2023
 *      Author: fm
 */

#include <stdint.h>
#include "API_delay.h"

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#define USARTx USART2
#define CARACTERES_MAX 255
#define COMM_LENGHT_MAX 5

bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveCommand(uint8_t * pstring, uint16_t size);
static uint16_t sizeArray(uint8_t *pstring);
bool_t eventCommand();
uint8_t* readCommand();

#endif /* API_INC_API_UART_H_ */
