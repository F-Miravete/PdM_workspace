//**********************************************************************************************************
//
// TP Final PdM y PCSE (CESE 2023)
// Titulo: Modulo API_uart (SOURCE)
// Autor: F.D.M.
//
//**********************************************************************************************************

#include "API_delay.h"
#include "API_uart.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>


UART_HandleTypeDef UartHandle;
static bool_t flagNewCommand = false;
static bool_t flagStartValue = false;
static bool_t flagEndValue = false;
static uint8_t rxBuff[CHARACTERS_MAX];
static uint8_t rxData[CHARACTERS_MAX];
static uint8_t index;
static uint8_t sValue[VALUE_LENGTH_MAX];

static uint16_t sizeArray(uint8_t *pstring);

//**********************************************************************************************************
// Funcion : bool_t uartInit()
//			 Inicializa el USART2 de la placa NUCLEO-F401RE, devuelve true si la inicializacion fue exitosa
//
//	         - Word Length = 8 Bits data (BE CAREFUL : word lenght incluye al bit de paridad si este es seteado)
//	         - Stop Bit    = One Stop bit
//	         - Parity      = NONE parity
//	         - BaudRate    = 9600 baud
//	         - Hardware flow control disabled (RTS and CTS signals)
//**********************************************************************************************************
bool_t uartInit()
{
	  HAL_StatusTypeDef statusInitUart;
	  bool_t initUartRET;
	  uint8_t stringInit[] = "*** CONFIG Serial PORT 9600 8N1 ***\n\r";
	  UartHandle.Instance        = USARTx;
	  UartHandle.Init.BaudRate   = 9600;
	  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	  UartHandle.Init.StopBits   = UART_STOPBITS_1;
	  UartHandle.Init.Parity     = UART_PARITY_NONE;
	  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  UartHandle.Init.Mode       = UART_MODE_TX_RX;
	  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	  statusInitUart = HAL_UART_Init(&UartHandle);
	  if (statusInitUart != HAL_OK)
	      initUartRET = false;
	  else
	  {
		  initUartRET = true;
		  if (HAL_UART_Transmit(&UartHandle, stringInit, (uint16_t)sizeof(stringInit), 0xFFFF) != HAL_OK)
		  	  initUartRET = false;
		  if (HAL_UART_Receive_IT(&UartHandle, rxBuff, 1) != HAL_OK)
			  initUartRET = false;
	  }
	  return initUartRET;
}

//**********************************************************************************************************
// Funcion : void uartSendString(uint8_t * pstring)
//			 Envia un string por la UART en forma completa.
//			 Recibe como parametro: - un puntero al string que se desea enviar.
//**********************************************************************************************************
void uartSendString(uint8_t *pstring)
{
	if (pstring != NULL)
	{
		if (HAL_UART_Transmit(&UartHandle, pstring, sizeArray(pstring), 0xFFFF) != HAL_OK)
			Error_Handler();
	}
	else Error_Handler();
}

//**********************************************************************************************************
// Funcion : void uartSendStringSize(uint8_t * pstring, uint16_t size)
//			 Envia una cantidad de caracteres de un string por la UART.
//			 Recibe como parametro: - un puntero al string que se desea enviar.
//			 						- una variable size que especifica la cantidad de caracteres a enviar
//**********************************************************************************************************
void uartSendStringSize(uint8_t *pstring, uint16_t size)
{
	if (pstring != NULL)
	{
		uint16_t aux = sizeArray(pstring);
		if (aux < size)
			size = aux;
		if (HAL_UART_Transmit(&UartHandle, pstring, size, 0xFFFF) != HAL_OK)
			Error_Handler();
	}
	else Error_Handler();
}


//**********************************************************************************************************
// Funcion : uint16_t sizeArray(uint8_t *pstring).
//			 Devuelve la cantidad de elementos de un array que contiene un string
//			 Recibe como parametro: - un puntero al array
//**********************************************************************************************************
static uint16_t sizeArray(uint8_t *pstring)
{
	uint16_t size = 0;
	if (pstring != NULL)
	{
		while ((pstring[size] != '\0') && (size < CHARACTERS_MAX))
			size++;
	}
	else Error_Handler();
	return size;
}

//**********************************************************************************************************
// Funcion : bool_t eventCommand()
//			 Devuelve verdadero si la USART recibio un comando
//			 Recibe como parametro: -
//**********************************************************************************************************
bool_t eventCommand()
{
	bool_t flag = flagNewCommand;
	flagNewCommand = false;
	return flag;
}

//**********************************************************************************************************
// Funcion : bool_t eventValue()
//			 Devuelve verdadero si la USART recibio un valor
//			 Recibe como parametro: -
//**********************************************************************************************************
bool_t eventValue()
{
	bool_t flag = flagEndValue;
	flagEndValue = false;
	return flag;
}

//**********************************************************************************************************
// Funcion : uint8_t * readCommand()
//			 Devuelve puntero al buffer donde se guardan los datos ingresados por UART
//			 Recibe como parametro: -
//**********************************************************************************************************
uint8_t* readCommand()
{
	return rxData;
}

//**********************************************************************************************************
// Funcion : uint16_t readValue()
//			 Devuelve el valor ingresado
//			 Recibe como parametro: -
//**********************************************************************************************************
uint16_t readValue()
{
	uint8_t i;
	uint16_t valor;
	for(i=0;i<(index-1);i++)
		sValue[i] = rxData[i];
	sValue[i] = 0;
	valor = (uint16_t)atoi((const char*)sValue);
	return valor;
}

//**********************************************************************************************************
// Funcion : void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//			 Rutina de atencion cuando ingresa interrupcion
//			 Recibe como parametro: - un puntero a la instancia del puerto USART
//**********************************************************************************************************
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USARTx)
	{
		uint8_t charData = rxBuff[0];
		rxData[index++] = charData;
		if(index >= CHARACTERS_MAX)
			index = 0;
		// -------------------------------------------------------------------------------------------------
		// Verifica si se envia un comando
		// -------------------------------------------------------------------------------------------------
		if(charData == '*')
			index = 0;
		if(index >= COMM_LENGTH_MAX)
		{
			flagNewCommand = true;
		}
		// -------------------------------------------------------------------------------------------------
		// Verifica si se envia un valor
		// -------------------------------------------------------------------------------------------------
		if(flagStartValue)
			if(charData == ')')
			{
				flagEndValue = true;
				flagStartValue = false;
			}
		if(charData == '(')
		{
			index = 0;
			flagStartValue = true;
		}

		HAL_UART_Receive_IT(&UartHandle, rxBuff, 1);
	}
}


