//**********************************************************************************************************
//
// Practica 5 - Programacion de Microcontroladores (CESE 2023)
// Titulo: Antirebote por Software (SOURCE)
// Autor: F.D.M.
//
//**********************************************************************************************************
#include "API_delay.h"
#include "API_debounce.h"
#include "API_uart.h"
#include "main.h"

static delay_t retardo;
static tick_t duration = DURATION_DELAY;
static debounceState_t actualState;
static bool_t estadoBoton;

static uint8_t msg1[] = "BOTON PULSADO - (flanco descendente)\n";
static uint8_t msg2[] = "BOTON LIBERADO - (flanco ascendente)\n";

//**********************************************************************************************************
// Prototipos de funcions privadas
//**********************************************************************************************************
static void buttonPressed();						// Invierte el estado del LED1 y envia Mensaje por UART
static void buttonReleased();						// Invierte el estado del LED3 y envia Mensaje por UART

//**********************************************************************************************************
// Funcion: debounceFSM_init()
// Inicializa la maquina de estados con su estado inicial
//**********************************************************************************************************
void debounceFSM_init()
{
	delayInit(&retardo, duration);
	actualState = BUTTON_UP;							// carga el estado inicial
	estadoBoton = false;
}

//**********************************************************************************************************
// Funcion: debounceFSM_update()
// Actualiza la maquina de estados: lee las entradas, resuelve la lógica de transicion de estados y
// actualiza salidas
//**********************************************************************************************************
void debounceFSM_update()
{
	switch (actualState)								// Consulta Estado FSM
	{													//
	case BUTTON_UP:										// Si BOTON Arriba ==>
		if(HAL_GPIO_ReadPin(GPIOC, B1_Pin)==0)			// Consulta si se presionò el pulsador B1
			actualState = BUTTON_FALLING;				// Actualiza FSM --> BOTON Accionado
		break;											//
	case BUTTON_FALLING:								// Si BOTON Presionado ==>
		if(delayRead(&retardo))							// Cuenta retardo de 40 ms
		{												//
			if(HAL_GPIO_ReadPin(GPIOC, B1_Pin)==0)		// Si retardo cumplido verifica si el boton sigue presionado
			{											//
				buttonPressed();						// 		Caso positivo: -Togglea Led 1
				actualState = BUTTON_DOWN;				//      	           -Actualiza FSM --> BOTON Abajo
			}											//
			else										//
				actualState = BUTTON_UP;				//		Caso negativo: -Actualiza FSM --> BOTON Arriba
		}  												//
		break;											//
	case BUTTON_DOWN:									// Si BOTON Abajo ==>
		if(HAL_GPIO_ReadPin(GPIOC, B1_Pin))				// Consulta si se liberò el pulsador B1
			actualState = BUTTON_RISING;				// Actualiza FSM ==> BOTON Liberado

		break;											//
	case BUTTON_RISING:									// Si BOTON Liberado ==>
		if(delayRead(&retardo))							// Cuenta retardo de 40 ms
		{												//
			if(HAL_GPIO_ReadPin(GPIOC, B1_Pin))			// Si retardo cumplido verifica si el boton sigue liberado
			{											//
				buttonReleased();						//		Caso positivo: -Togglea Led 3
				actualState = BUTTON_UP;				// 					   -Actualiza FSM --> BOTON Arriba
			}											//
			else										//
				actualState = BUTTON_DOWN;				//		Caso negativo: -Actualiza FSM --> BOTON Abajo
		}												//
		break;											//
	default:											//
		// CONTROL de ERRORES							//
		debounceFSM_init(); 							// Por default inicia FSM
		break;
	}
}

//**********************************************************************************************************
// Funcion: buttonPressed()
// Esta funcion invierte el estado del LED1 y envia mensaje por la UART
//**********************************************************************************************************
static void buttonPressed()
{
	HAL_GPIO_TogglePin(GPIOB, LED01_Pin);
	estadoBoton = true;
	uartSendString(msg1);
}

//**********************************************************************************************************
// Funcion: buttonReleased()
// Esta funcion invierte el estado del LED3 y envia mensaje por la UART
//**********************************************************************************************************
static void buttonReleased()
{
	HAL_GPIO_TogglePin(GPIOB, LED03_Pin);
	uartSendString(msg2);
}

//**********************************************************************************************************
// Funcion: bool_t readKey()
// Esta funcion devuelce el estado del boton (True: si el boton fue presionado / False: boton NO presionado)
//**********************************************************************************************************
bool_t readKey()
{
	bool_t flagReadKey = estadoBoton;
	estadoBoton = false;
	return flagReadKey;
}

