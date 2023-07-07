// ********************************************************************************************************
  // Defino vector de 3 elementos con las direcciones de los pines 7, 8 y 9 del port GPIOB
  // pin 7 -> Led01  /  pin 8 -> Led02  /  pin 9 -> Led03
  // Las direcciones se encuentran definidas en Drivers/STM32F4xx_HAL_Driver/Inc/STM32F4xx_hal_gpio.h
  // Inicializo las 3 salidas del port GPIOB en 0
  // ********************************************************************************************************
#include "API_delay.h"

static void API_Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}



//*******************************************************************************************************************
// delayInit: Inicializa retardo cargando su valor en [ms] y su flag running en 0
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
// delayRead: Implementa el retardo.
// 			  Si el retardo no esta corriendo lo pone a correr
//			  Si el retardo esta corriendo compara el tiempo trancurrido y devuelve true si el tiempo se ha cumplido
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
// delayWrite: Cambia el tiempo de duracion de un retardo existente
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



