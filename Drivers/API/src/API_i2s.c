/*
 * API_i2s.c
 *
 *  Created on: Aug 9, 2023
 *      Author: z002m07j
 */

#include "API_i2s.h"
#include "math.h"
#include "main.h"

I2S_HandleTypeDef hi2s2;
int32_t dataRaw[RAW_SIZE * 2];
int16_t channel1[RAW_SIZE];
int16_t channel2[RAW_SIZE];

bool_t i2sInit()
{
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s2.Init.Standard = I2S_STANDARD_MSB;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B_EXTENDED;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_96K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }

  iniciarData();
  HAL_NVIC_EnableIRQ(SPI2_IRQn);
  HAL_I2S_Transmit_IT(&hi2s2, (uint16_t*)dataRaw, RAW_SIZE*2);
  return true;
}

void iniciarData(void)
{
	uint16_t i;
	int32_t aux;
	for(i=0;i<RAW_SIZE;i++)
	{
		//dataRaw[i] = ((RAW_SIZE-1) - i) * 65;
		//if(dataRaw[i] > 32767)
		//	dataRaw[i] = 0;
		channel1[i] = 16383*sinf(i*2*M_PI/RAW_SIZE);
		channel2[i] = i*(32767/RAW_SIZE);
		//if(seno[i]<0)
		//	dataRaw[i] = ~seno[i] + 1;
		//else
			dataRaw[i] = channel1[i];
			aux = dataRaw[i]<<16;
			dataRaw[i] = aux;
			dataRaw[i] = dataRaw[i] + (int32_t)channel2[i];
		//dataRaw[i] = ~(dataRaw[i]) + 1;
	}
}

//**********************************************************************************************************
// Funcion : void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//			 Rutina de atencion cuando ingresa interrupcion
//			 Recibe como parametro: - un puntero a la instancia del puerto USART
//**********************************************************************************************************
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	if(hi2s->Instance == SPI2)
	{
		HAL_I2S_Transmit_IT(&hi2s2, (uint16_t*)dataRaw, RAW_SIZE*2);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}
}
