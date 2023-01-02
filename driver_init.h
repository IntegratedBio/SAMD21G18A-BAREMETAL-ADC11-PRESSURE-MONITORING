/* File:	driver_init.h
 * Author:	Integrated Bioengineering, LLC.
 * Processor: SAMD21G18A @ 48MHz, 3.3v
 * Program: Driver Initialization Library
 * Compiler: ARM-GCC Microchip Studio
 * Program Version 1.0
 * Program Description: Include file for driver initialization
 * Hardware Description: 
 * 
 * Change History:
 * Author                Rev     Date          Description
 * Brad				     1.0     02/12/2022    ADC Setup
 *
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
// INCLUDES AND DEFINITIONS
//////////////////////////////////////////////////////////////////////////

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_adc_sync.h>
#include <hal_usart_sync.h>
#include <hal_delay.h>

#include <stdio.h>
#include <stdio.h>
#include <stdio_io.h>

extern struct adc_sync_descriptor ADC_0;
extern struct usart_sync_descriptor CONSOLE_UART;
extern bool pump_state;
//////////////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////
void system_init(void);
//ADC 11
void ADC_0_PORT_init(void);
void ADC_0_CLOCK_init(void);
void ADC_0_init(void);
void ADC_11_read(void);
float average(int* values, int size);

void CONSOLE_UART_PORT_init(void);
void CONSOLE_UART_CLOCK_init(void);
void CONSOLE_UART_init(void);

void delay_driver_init(void);

// PUMP FUNCTION PROTOTYPES
void PUMP_init(void);
void PUMP_off(void);
void PUMP_on(void);

// LED
void LED_0_init(void);
void LED_0_on(void);
void LED_0_off(void);
void LED_0_toggle(void);

//VALVE 0
void VALVE_0_init(void);
void VALVE_0_off(void);
void VALVE_0_on(void);

//VALVE 1
void VALVE_1_init(void);
void VALVE_1_off(void);
void VALVE_1_on(void);

//LOW LEVEL SWITCH
void LOW_LVL_init(void);

//ALARM
void ALARM_init();
void ALARM_on();
void ALARM_off();

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
