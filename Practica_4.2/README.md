# Practica 4 - Programacion de Microcontroladores (CESE 2023)

## RESUMEN
- Plataforma de desarrollo 
- Objetivos
- Ubicacion de archivos
- Variables y macros
- Funciones implementadas

## Plataforma de desarrollo
![alt text](PlacaF401RE_Pract4.jpeg "Placa")

Compilado y probado para la placa de desarrollo **ST NUCLEO-F401RE**

**NOTA:** Este ejercicio utiliza las salidas GPIOB pin 7,8 y 9 cableadas a 3 leds externos.

## Objetivos

Implementar un módulo de software en un archivo fuente API_debounce.c con su correspondiente archivo de cabecera API_debounce.h y ubicarlos en el proyecto dentro de  las carpetas /drivers/API/src y /drivers/API/inc, respectivamente.
En API_debounce.h se deben ubicar los prototipos de las funciones públicas y las declaraciones:
void debounceFSM_init();
void debounceFSM_update();

/* La función readKey debe leer una variable interna del módulo y devolver true o false si la tecla fue presionada.  Si devuelve true, debe resetear (poner en false) el estado de la variable.*/
bool_t readKey();

En API_debounce.c se deben ubicar las declaraciones privadas, los prototipos de las funciones privadas y la implementación de todas las funciones del módulo, privadas y públicas:

La declaración de debounceState_t debe ser privada en el archivo .c y la variable de estado de tipo debounceState_t debe ser global privada (con static).

Declarar una variable tipo bool_t global privada que se ponga en true cuando ocurre un flanco descendente y se ponga en false cuando se llame a la función readKey();

Implementar un programa que cambie la frecuencia de toggleo del LED2 entre 100 ms y 500 ms cada vez que se presione la tecla.  El programa debe usar las funciones anti-rebote del módulo API_debounce y los retardos no bloqueantes del módulo API_delay.

La FSM tiene el siguiente esquema

![alt text](FSM_Pract4.jpg "FSM")
 
El tiempo de anti-rebote es de 40 ms y se utiliza un retardo no bloqueante implementado en el modulo API_delay.
Cuando se detecta pulsador presionado se "togglea" el led 1 (pin 7) y cuando se detecta pulsador liberado
se togglea el led 3 (pin 9). 

## Ubicacion y archivos
```bash
├───Drivers
│    ├───API
│    │   ├───Inc     <--- API_delay.h API_debounce.h
│    │   └───Scr     <--- API_delay.c API_debounce.c
│    ├───Core
│    ├───CMSIS
│    └───STM32F4xx_HAL_Driver
├───Inc      <--- main.h
├───Src      <--- main.c
```

## Variables y macros
```C
#define DURATION_MAX 86400000		// Define macro retardo maximo en [ms], equivale a un dia
#define DURATION_MIN 0				// Define macro retardo minimo en [ms], cero
#define DURATION_DELAY 40           // Define duracion del retardo en 40 ms

typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{				
	tick_t startTime;			
	tick_t duration;				
	bool_t running;				
} delay_t;

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RISING,
} debounceState_t;
```

## Funciones implementadas
```C
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );
void API_Error_Handler(void);

void debounceFSM_init();		// debe cargar el estado inicial
void debounceFSM_update();		// debe leer las entradas, resolver la lógica de
								// transición de estados y actualizar las salidas
void buttonPressed();			// debe invertir el estado del LED1
void buttonReleased();			// debe invertir el estado del LED3
```
