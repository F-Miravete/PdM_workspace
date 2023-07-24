//**********************************************************************************************************
//
// Practica 4 Punto 2 - Programacion de Microcontroladores (CESE 2023)
// Titulo: Antirebote por Software (SOURCE)
// Autor: F.D.M.
//
//**********************************************************************************************************
#include "API_delay.h"
#include "API_debounce.h"
#include "main.h"

static delay_t retardo;
static tick_t duration = DURATION_DELAY;
extern debounceState_t actualState;
static bool_t estadoBoton;

//**********************************************************************************************************
// Prototipos de funcions privadas
//**********************************************************************************************************
static void buttonPressed();								// Invierte el estado del LED1
static void buttonReleased();								// Invierte el estado del LED3

//**********************************************************************************************************
// Funcion: debounceFSM_init()
// Inicializa la maquina de estados con su estado inicial
//**********************************************************************************************************
void debounceFSM_init(debounceState_t fsm)
{
	delayInit(&retardo, duration);
	actualState = BUTTON_UP;	// carga el estado inicial
	estadoBoton = false;
}

//**********************************************************************************************************
// Funcion: debounceFSM_update()
// Actualiza la maquina de estados: lee las entradas, resuelve la lógica de transicion de estados y
// actualiza salidas
//**********************************************************************************************************
void debounceFSM_update(debounceState_t fsm)
{
	switch (fsm)										// Consulta Estado FSM
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
		estadoBoton = true;
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
		debounceFSM_init(fsm);							// Por default inicia FSM
		break;
	}
}

//**********************************************************************************************************
// Funcion: buttonPressed()
// Esta funcion invierte el estado del LED1
//**********************************************************************************************************
static void buttonPressed()
{
	HAL_GPIO_TogglePin(GPIOB, LED01_Pin);
}

//**********************************************************************************************************
// Funcion: buttonReleased()
// Esta funcion invierte el estado del LED3
//**********************************************************************************************************
static void buttonReleased()
{
	HAL_GPIO_TogglePin(GPIOB, LED03_Pin);
}

//**********************************************************************************************************
// Funcion: bool_t readKey()
// Esta funcion devuelce el estado del boton
//**********************************************************************************************************
bool_t readKey()
{
	bool_t aux = false;
	if(estadoBoton)
	{
		estadoBoton = false;
		aux = true;
	}
	return aux;
}

