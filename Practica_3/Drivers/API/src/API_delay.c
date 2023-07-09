//**********************************************************************************************************
//
// Practica 3 - Programacion de Microcontroladores (CESE 2023)
// Titulo: Modulo API_delay (SOURCE)
// Autor: F.D.M.
//
//**********************************************************************************************************

#include "API_delay.h"

//*******************************************************************************************************************
// Funcion: delayInit()
// Inicializa retardo cargando su valor en [ms] y su flag running en 0
//*******************************************************************************************************************
void delayInit( delay_t * delay, tick_t duration )
{
	if(delay != NULL )
	{
		delay->running = false;
		if(duration > DURATION_MAX)
			duration = DURATION_MAX;
		if(duration < DURATION_MIN)
			duration = DURATION_MIN;
		delay->duration = duration;
	}
	else API_Error_Handler();
}

//*******************************************************************************************************************
// Funcion: delayRead()
// Implementa un retardo NO bloqueante.
// 		Si el retardo no  esta corriendo lo pone a correr
//		Si el retardo esta corriendo compara el tiempo trancurrido y devuelve true si el tiempo se ha cumplido
//*******************************************************************************************************************
bool_t delayRead( delay_t * delay )
{
	bool_t time_end = false;
	if(delay != NULL)
	{
		if(delay->running == false)
		{
			delay->running = true;
			delay->startTime = HAL_GetTick();
		}
		else
		{
			if((HAL_GetTick() - delay->startTime) >= delay->duration)
			{
				delay->running = false;
				time_end = true;
			}
		}
	}
	else API_Error_Handler();
	return time_end;
}

//*******************************************************************************************************************
// Funcion: delayWrite()
// Cambia el tiempo de duracion de un retardo existente
//*******************************************************************************************************************
void delayWrite( delay_t * delay, tick_t duration )
{
	if(delay != NULL)
	{
		if(duration > DURATION_MAX)
			duration = DURATION_MAX;
		if(duration < DURATION_MIN)
			duration = DURATION_MIN;
		delay->duration = duration;
	}
	else API_Error_Handler();
}

//**********************************************************************************************************
// Funcion: API_Error_Handler()
// Manejo de errores por pasaje de parametros en modulo API_delay, si existe un error enciende led de
// usuario LD2
//**********************************************************************************************************
void API_Error_Handler(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);

	while (1)
	{
	}
}

