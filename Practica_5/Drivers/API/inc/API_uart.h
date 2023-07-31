/*
 * API_uart.h
 *
 *  Created on: Jul 27, 2023
 *      Author: fm
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#define USARTx USART2

bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);
static uint16_t sizeArray(uint8_t *pstring);

#endif /* API_INC_API_UART_H_ */
