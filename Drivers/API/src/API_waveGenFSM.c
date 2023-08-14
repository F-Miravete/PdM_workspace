/*
 * waveGenFSM.c
 *
 *  Created on: Aug 3, 2023
 *      Author: fm
 */

#include "API_i2s.h"
#include "API_waveGenFSM.h"
#include "API_uart.h"
#include "API_delay.h"
#include "main.h"
#include "string.h"
#include "stdio.h"

static uint8_t  sComm[COMM_LENGTH_MAX];      // = {0,0,0,0};
static uint8_t* pComm;
static uint8_t  i;
static uint16_t newValue;
static uint8_t buffer[CHARACTERS_MAX];
static waveGenFSMstate_t actualState;

static const uint8_t comm_play[] = "PLAY";
static const uint8_t comm_stop[] = "STOP";
static const uint8_t comm_menu[] = "MENU";
static const uint8_t comm_sCH0[] = "CH00";
static const uint8_t comm_sCH1[] = "CH01";
static const uint8_t comm_freq[] = "FREQ";
static const uint8_t comm_back[] = "BACK";
static const uint8_t comm_amp0[] = "AMP0";
static const uint8_t comm_amp1[] = "AMP1";
static const uint8_t comm_wav0[] = "WAV0";
static const uint8_t comm_wav1[] = "WAV1";
static const uint8_t comm_sine[] = "SINE";
static const uint8_t comm_sawt[] = "SAWT";

static const uint8_t msg_menu[] = "\n\r************ Menu CONFIGURATION ************\n\r*FREQ\t*CH00\t*CH01\t*BACK\n\r";
static const uint8_t msg_stop[] = "\n\r************** Status STOP **************\n\r*PLAY\t*MENU\n\r";
static const uint8_t msg_freq[] = "\n\rInput Frequency in Hz (20 to 24000, use brackets) or *BACK : ";
static const uint8_t msg_ampl[] = "\n\rInput Amplitude in %% (0 to 100, use brackets) or *BACK : ";
static const uint8_t msg_set0[] = "\n\r************ Menu SET CHANNEL ************\n\r*AMP0\t*WAV0\t*BACK\n\r";
static const uint8_t msg_set1[] = "\n\r************ Menu SET CHANNEL ************\n\r*AMP1\t*WAV1\t*BACK\n\r";
static const uint8_t msg_wave[] = "\n\r************ SET WAVE FORM ************\n\r*SINE\t*SAWT\t*BACK\n\r";

static void printUart(const uint8_t* msg);
static void cleanComm();

//**********************************************************************************************************
// Funcion :
//
//
//**********************************************************************************************************
void waveGenFSM_init()
{
	actualState = STOP;
	HAL_NVIC_DisableIRQ(SPI2_IRQn);
	printUart(msg_stop);
}

//**********************************************************************************************************
// Funcion :
//
//
//**********************************************************************************************************
void waveGenFSM_update()
{
	channel *ch0;
	channel *ch1;
	uint8_t m0[] = "SINUSOIDAL";
	uint8_t m1[] = "SAWTOOTH";
	uint8_t *p0, *p1;

	// Verificacion si arribó un comando
	if(eventCommand())
	{
		pComm = readCommand();
		for(i=0;i<COMM_LENGTH_MAX;i++)
			sComm[i] = *pComm++;
	}


	switch (actualState)
	{
		case STOP:
			if(strstr((const char*)sComm, (const char*)comm_play))
			{
				actualState = PLAY;
				startI2S();
				ch0 = readChannelProperty(CHANNEL_0);
				ch1 = readChannelProperty(CHANNEL_1);
				if(ch0->wave_type == SINUSOIDAL)
					p0 = m0;
				else p0 = m1;
				if(ch1->wave_type == SINUSOIDAL)
					p1 = m0;
				else p1 = m1;
				snprintf((char*)buffer, CHARACTERS_MAX, "\n\r************** Status PLAY **************\n\r"
				                                        "Frequency : %d Hz\n\r"
														"Channel 0 : %s Amplitude : %d %%\n\r"
														"Channel 1 : %s Amplitude : %d %%\n\r",
														ch0->freq,p0,ch0->amplitude,p1,ch1->amplitude);
				uartSendString(buffer);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_menu))
			{
				actualState = MENU_CONFIG;
				printUart(msg_menu);
				cleanComm();
			}
		break;
		case PLAY:
			if(strstr((const char*)sComm, (const char*)comm_stop))
			{
				actualState = STOP;
				HAL_NVIC_DisableIRQ(SPI2_IRQn);
				printUart(msg_stop);
				cleanComm();
			}
		break;
		case MENU_CONFIG:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = STOP;
				printUart(msg_stop);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_freq))
			{
				actualState = INPUT_FREQ;
				printUart(msg_freq);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_sCH0))
			{
				actualState = SELECT_CH0;
				printUart(msg_set0);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_sCH1))
			{
				actualState = SELECT_CH1;
				printUart(msg_set1);
				cleanComm();
			}
		break;
		case INPUT_FREQ:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = MENU_CONFIG;
				printUart(msg_menu);
				cleanComm();
			}
			if(eventValue())
			{
				newValue = readValue();
				if(newValue < 20 || newValue > 24000)
				{
					actualState = MENU_CONFIG;
					printUart((const uint8_t*)"\n-\rINVALID VALUE\n\r");
					printUart(msg_menu);
				}
				else
				{
					setFreqChannels(newValue);
					snprintf((char*)buffer, CHARACTERS_MAX, "\n\rValue in Hz = %d\n\r", newValue);
					uartSendString(buffer);
					actualState = MENU_CONFIG;
					printUart(msg_menu);
				}
			}
		break;
		case SELECT_CH0:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = MENU_CONFIG;
				printUart(msg_menu);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_amp0))
			{
				actualState = SET_AMP_CH0;
				printUart(msg_ampl);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_wav0))
			{
				actualState = SET_WAVE_CH0;
				printUart(msg_wave);
				cleanComm();
			}

		break;
		case SELECT_CH1:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = MENU_CONFIG;
				printUart(msg_menu);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_amp1))
			{
				actualState = SET_AMP_CH1;
				printUart(msg_ampl);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_wav1))
			{
				actualState = SET_WAVE_CH1;
				printUart(msg_wave);
				cleanComm();
			}
		break;
		case SET_AMP_CH0:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = SELECT_CH0;
				printUart(msg_set0);
				cleanComm();
			}
			if(eventValue())
			{
				newValue = readValue();
				if(newValue < 0 || newValue > 100)
				{
					actualState = SELECT_CH0;
					printUart((const uint8_t*)"\n-\rINVALID VALUE\n\r");
					printUart(msg_set0);
				}
				else
				{
					setAmpChannel(CHANNEL_0, (uint8_t)newValue);
					snprintf((char*)buffer, CHARACTERS_MAX, "\n\rValue in %% = %d\n\r", newValue);
					uartSendString(buffer);
					actualState = MENU_CONFIG;
					printUart(msg_menu);
				}
			}
		break;
		case SET_AMP_CH1:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = SELECT_CH1;
				printUart(msg_set1);
				cleanComm();
			}
			if(eventValue())
			{
				newValue = readValue();
				if(newValue < 0 || newValue > 100)
				{
					actualState = SELECT_CH1;
					printUart((const uint8_t*)"\n\rINVALID VALUE\n\r");
					printUart(msg_set1);
				}
				else
				{
					setAmpChannel(CHANNEL_1, (uint8_t)newValue);
					snprintf((char*)buffer, CHARACTERS_MAX, "\n\rValue in %% = %d\n\r", newValue);
					uartSendString(buffer);
					actualState = MENU_CONFIG;
					printUart(msg_menu);
				}
			}
		break;

		case SET_WAVE_CH0:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = SELECT_CH0;
				printUart(msg_set0);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_sine))
			{
				actualState = MENU_CONFIG;
				setWaveChannel(CHANNEL_0, SINUSOIDAL);
				printUart(msg_menu);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_sawt))
			{
				actualState = MENU_CONFIG;
				setWaveChannel(CHANNEL_0, SAWTOOTH);
				printUart(msg_menu);
				cleanComm();
			}
		break;
		case SET_WAVE_CH1:
			if(strstr((const char*)sComm, (const char*)comm_back))
			{
				actualState = SELECT_CH1;
				printUart(msg_set1);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_sine))
			{
				actualState = MENU_CONFIG;
				setWaveChannel(CHANNEL_1, SINUSOIDAL);
				printUart(msg_menu);
				cleanComm();
			}
			if(strstr((const char*)sComm, (const char*)comm_sawt))
			{
				actualState = MENU_CONFIG;
				setWaveChannel(CHANNEL_1, SAWTOOTH);
				printUart(msg_menu);
				cleanComm();
			}
		break;

		default:

		break;
	}
}

static void printUart(const uint8_t* msg)
{
	uint8_t buffer[CHARACTERS_MAX];
	snprintf((char*)buffer, CHARACTERS_MAX, (const char*)msg);
	uartSendString(buffer);
}


static void cleanComm()
{
	uint8_t i;
	for(i=0;i<COMM_LENGTH_MAX;i++)
		sComm[i] = 0;
}



