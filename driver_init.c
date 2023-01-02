#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

#include <hpl_adc_base.h>

struct adc_sync_descriptor ADC_0;

struct usart_sync_descriptor CONSOLE_UART;

void ADC_0_PORT_init(void){
	gpio_set_pin_direction(PB03, GPIO_DIRECTION_OFF);
	gpio_set_pin_function(PB03, PINMUX_PB03B_ADC_AIN11);
}

void ADC_0_CLOCK_init(void){
	_pm_enable_bus_clock(PM_BUS_APBC, ADC);
	_gclk_enable_channel(ADC_GCLK_ID, CONF_GCLK_ADC_SRC);
}

void ADC_0_init(void){
	ADC_0_CLOCK_init();
	ADC_0_PORT_init();
	adc_sync_init(&ADC_0, ADC, (void *)NULL);
}

void CONSOLE_UART_PORT_init(void){
	//PORT A THROUGH THE USB
	//gpio_set_pin_function(PA24, PINMUX_PA24D_SERCOM5_PAD2);					//GPIO 'TX' PIN CONFIGURATION WITH PB23 PORT DEFINITION FOR SERCOM5 PERIPHERAL
	//gpio_set_pin_function(PA25, PINMUX_PA25D_SERCOM5_PAD3);					//GPIO 'RX' PIN CONFIGURATION WITH PB22 PORT DEFINITION FOR SERCOM5 PERIPHERAL
	
	//PORT B THROUGH THE TX/RX Pins
	gpio_set_pin_function(PB22, PINMUX_PB22D_SERCOM5_PAD2);						//GPIO 'TX' PIN CONFIGURATION WITH PB23 PORT DEFINITION FOR SERCOM5 PERIPHERAL
	gpio_set_pin_function(PB23, PINMUX_PB23D_SERCOM5_PAD3);						//GPIO 'RX' PIN CONFIGURATION WITH PB22 PORT DEFINITION FOR SERCOM5 PERIPHERAL
}

void CONSOLE_UART_CLOCK_init(void){
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM5);									//POWER MANAGEMENT BUS APB BASE ADDRESS TO SERCOM5 
	_gclk_enable_channel(SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC);		//PORT DEFINITION FOR SERCOM5 PERIPHERAL INSTANCE SOURCED W/ GCLK:0
}

void CONSOLE_UART_init(void){
	CONSOLE_UART_CLOCK_init();
	usart_sync_init(&CONSOLE_UART, SERCOM5, (void *)NULL);
	CONSOLE_UART_PORT_init();
}

void stdio_redirect_init(void){
	usart_sync_enable(&CONSOLE_UART);
	stdio_io_init(&CONSOLE_UART.io);
}

void delay_driver_init(void){
	delay_init(SysTick);
}

void system_init(void){
	init_mcu();
	ADC_0_init();
	CONSOLE_UART_init();
	delay_driver_init();
	LED_0_init();
	PUMP_init();
	VALVE_0_init();
	VALVE_1_init();
	LOW_LVL_init();
	ALARM_init();
}

///////////////////////////////////////////
// BUILT-IN LED INITIALIZATION
///////////////////////////////////////////
void LED_0_init(void){	//CONFIGURE GPIO ON PA17 TO FUNCTION AS ONBOARD LED INDICATOR
	gpio_set_pin_direction(LED_BUILTIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_pull_mode(LED_BUILTIN, GPIO_PULL_OFF);
	gpio_set_pin_function(LED_BUILTIN, GPIO_PIN_FUNCTION_OFF);
	gpio_set_pin_level(LED_BUILTIN,true);
}

void LED_0_on(void){
	gpio_set_pin_level(LED_BUILTIN,true);
}
void LED_0_off(void){
	gpio_set_pin_level(LED_BUILTIN,false);
}

void LED_0_toggle(void){
	gpio_toggle_pin_level(LED_BUILTIN);
}

///////////////////////////////////////////
// PUMP DRIVER INTILIZATION
///////////////////////////////////////////

void PUMP_init(void){
	//SET PIN DIRECTION (3 - Different expressions)
	gpio_set_pin_direction(PUMP_0_PIN, GPIO_DIRECTION_OUT);
	//SET PIN FUNCTION
	gpio_set_pin_function(PUMP_0_PIN, GPIO_PIN_FUNCTION_OFF);
	//CONFIGURE GPIO TO FUNCTION AS A RELAY OUTPUT FOR THE PUMP
	gpio_set_pin_level(PUMP_0_PIN,true);
}
void PUMP_off(void){
	gpio_set_pin_level(PUMP_0_PIN, true);//stop current when high
	printf("\t Pump Off");
};
void PUMP_on(void){
	gpio_set_pin_level(PUMP_0_PIN,false);//sink current when low
	printf("\t Pump On");
};

///////////////////////////////////////////
// VALVE DRIVER INTILIZATION
///////////////////////////////////////////
void VALVE_0_init(void){
	gpio_set_pin_direction(VALVE_0_PIN, GPIO_DIRECTION_OUT);	//SET PIN AS AN OUTPUT ->
	gpio_set_pin_function(VALVE_0_PIN, GPIO_PIN_FUNCTION_OFF);	//SET PIN FUNCTION OFF
	gpio_set_pin_level(VALVE_0_PIN,true);						//START IN THE OFF POSITION -> PULL DOWN TO ALARM
}

void VALVE_0_off(void){
	gpio_set_pin_level(VALVE_0_PIN, true);						//CYCLE OFF
	printf("\t Valve 0 Off");
}

void VALVE_0_on(void){
	gpio_set_pin_level(VALVE_0_PIN, false);						//CYCLE ON
	printf("\t Valve 0 On");
}

void VALVE_1_init(void){
	gpio_set_pin_direction(VALVE_1_PIN, GPIO_DIRECTION_OUT);	//SET PIN AS AN OUTPUT ->
	gpio_set_pin_function(VALVE_1_PIN, GPIO_PIN_FUNCTION_OFF);	//SET PIN FUNCTION OFF
	gpio_set_pin_level(VALVE_1_PIN,true);						//START IN THE OFF POSITION -> PULL DOWN TO ALARM
}

void VALVE_1_off(void){	
	gpio_set_pin_level(VALVE_1_PIN, true);						//SET PIN HIGH TO STOP SINKING
	printf("\t Valve 1 Off");
}

void VALVE_1_on(void){
	gpio_set_pin_level(VALVE_1_PIN, false);						//SET PIN LOW TO START SINKING
	printf("\t Valve 1 On");
}

///////////////////////////////////////////
// LOW LEVEL SWITCH DRIVER INITIALIZATION
///////////////////////////////////////////
void LOW_LVL_init(void){
	gpio_set_pin_direction(LOW_LVL_PIN, GPIO_DIRECTION_IN);		//SET PIN AS A A DIGITAL INPUT
	gpio_set_pin_pull_mode(LOW_LVL_PIN,GPIO_PULL_UP);			//PULLDOWN RESISTOR TO REDUCE CURRENT INTO THE BOARD
	gpio_set_pin_function(LOW_LVL_PIN, GPIO_PIN_FUNCTION_OFF);		
}

// LOW LEVEL SWITCH CALLBACK FUNCTION - TBD ASSING INTERRUPT
void LOW_LVL_cb(void){}

///////////////////////////////////////////
// ALARM DRIVER INTILIZATION
///////////////////////////////////////////
void ALARM_init(void){
	gpio_set_pin_direction(ALARM_PIN, GPIO_DIRECTION_OUT);		//SET PIN AS AN OUTPUT ->
	gpio_set_pin_function(ALARM_PIN, GPIO_PIN_FUNCTION_OFF);	//SET PIN FUNCTION OFF
	gpio_set_pin_level(ALARM_PIN,true);							//START IN THE OFF POSITION -> PULL DOWN TO ALARM
}

void ALARM_off(void){
	gpio_set_pin_level(ALARM_PIN, true);						//SET PIN HIGH TO STOP SINKING
	printf("\t Alarm Off");
}

void ALARM_on(void){
	gpio_set_pin_level(ALARM_PIN, false);						//SET PIN LOW TO START SINKING
	printf("\t Alarm On");
}
