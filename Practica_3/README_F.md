# Practica 3 - Programacion de Microcontroladores (CESE 2023)

## RESUMEN
- [Plataforma de desarrollo] 
- [Objetivos]
- [Ubicacion de archivos]
- [Variables y macros]
- [Funciones implementadas]

## Plataforma de desarrollo
Compilado y probado para la placa de desarrollo **ST NUCLEO-F401RE**

**NOTA:** Este ejercicio utiliza las salidas GPIOB pin 7,8 y 9 cableadas a 3 leds externos.

## Objetivos
Implementar un módulo de software para trabajar con retardos no bloqueantes. 
El programa principal utiliza retardos no bloqueantes y hace titilar en forma periódica e independiente tres leds.

## Ubicacion y archivos
```bash
├───Drivers
│    ├───API
│    │   ├───Inc     <--- API_delay.h
│    │   └───Scr     <--- API_delay.c
│    ├───Core
│    ├───CMSIS
│    └───STM32F4xx_HAL_Driver
├───Inc      <--- main.h
├───Src      <--- main.c
```

## Variables y macros
```C
#define CANT 3						// Define macro cantidad de leds
#define DURATION_MAX 86400000		// Define macro retardo maximo en [ms], equivale a un dia
#define DURATION_MIN 0				// Define macro retardo minimo en [ms], cero
#define DURATION_1 250				// Define macro retardo Led 1 en [ms]
#define DURATION_2 500				// Define macro retardo Led 2 en [ms]
#define DURATION_3 1000				// Define macro retardo Led 3 en [ms]

typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{				
	tick_t startTime;			
	tick_t duration;				
	bool_t running;				
} delay_t;
```

## Funciones implementadas
```C
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );
void API_Error_Handler(void);
```
