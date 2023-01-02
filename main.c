/* File:	main.c
 * Author:	Integrated Bioengineering, LLC.
 * Processor: SAMD21G18A @ 48MHz, 3.3v
 * Program: Main Application File
 * Compiler: ARM-GCC Microchip Studio
 * Program Version 1.0
 * Program Description: Pump Control Application
 * Hardware Description: Pressure Transmitter and pump relay
 * 
 * Change History:
 * Author                Rev     Date          Description
 * Brad				     1.0     02/12/2022    ADC Setup
 */

// DEFINES AND INCLUDES
#include <atmel_start.h>

#define ADC_11_channel					11				//A7 Labeled on Board
#define ADC_11_raw_buffer_size			2				//Bytes
#define ADC_11_ave_buffer_size			5				//
#define ADC_11_voltage_reference		2.229729729730	//VDC INTERNAL

//PRESSURE MAPPING VALUES
//ADC VALUES
#define ADC_11_fsr		65520 	//FSR, HIGHEST VALYE ADC WILL READOUT. 4 BITS ARE DISCARDED AND USED INSTEAD AS MOST SIGNIFICANT BITS (MSB)
#define ADC_11_max		64638	//CALCULATED 2.20/2.23 * FSR VDC 
#define ADC_11_min		12927	//CALCULATED 0.44/2.23 * VDC 

/* TRAFAG INDSUTRIAL PRESSURE TRANSMITTER
TYPE:			8287.G8.2530
S/N:			832054-003 | 08/21
RANGE:			0....150 psi-G | max. 450 psi
POWER:			+9 - 32 VDC
OUTPUT:			4-20 ma converted to voltage with a shunt 110 Ohm 1/4 Watt resistor [YR1B110RCC] to ground
*/
#define voltage_at_max	2.20	// VDC	(2.20 Calculated @ 20ma | ~	150 PSI)
#define voltage_at_min	0.44	// VDC	(0.44 Calculated at 4ma	| ~	0  PSI)
#define min_pressure	0
#define max_pressure	150


//GLOBAL VARIABLES
uint16_t	ADC_11_value	=	0;	// 16 BIT UNSIGNED INTEGER FOR SINGLE ENDED (ALWAYS POSITVE)
int values;							// INTEADC VALUES THAT WILL BE
int size;

float		ADC_11_volts		=	0;	
float		ADC_11_pressure		=	0;		
int			ADC_11_pressure_int =	0;
uint8_t		set_pressure		=	100;	// PRESSURE SETPOINT

float map(float x, float in_min, float in_max, float out_min, float out_max) { return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; }

bool pump_state     =	0;     /*!< flag to represent the application state*/
bool Low_level_state =  0;
int main(void){
	atmel_start_init();
	adc_sync_enable_channel(&ADC_0, ADC_11_channel);

	char *setup_greeting = "\nSETUP COMPLETE!\n";	
	printf("\n%s", setup_greeting); //GREETING
	
	///////////////////////////END OF SETUP////////////////////////////////
	
	while (1)
	{
		do {
			ADC_11_read();
			
			
			if (pump_state == false){					//if pump is off, see if you need to turn it on
				if(ADC_11_pressure < set_pressure){		//CHECK PRESSURE
					PUMP_on();
					pump_state = true;
				}
			}		
		
			if (pump_state == true){					//IF the pump is not off, see if you need to turn it off
				if(ADC_11_pressure >= set_pressure){
					PUMP_off();
					pump_state = false;
				}
			}
	
			delay_ms(25);								
		} while (gpio_get_pin_level(LOW_LVL_PIN));
		
		do {
		delay_ms(100);
		} while (!gpio_get_pin_level(LOW_LVL_PIN));
		
	}//END OF APP LOOP
	
}//END OF MAIN

//************************************************************************
	void ADC_11_read(void){
		/* ///////////////////////////////////////////////////////////////
		 * ADC READ CONFIGURATION SUMMARY
		 * ADC CHANNEL:			11 (+) PB03/A7 
		 * ADC MODE:			SINGLE ENDED VS. INTERNAL GROUND GROUND	
		 * ADC CLOCK SOURCE:	GENERIC CLOCK 2 >> [OSC32K] HIGH-ACCURACY
		 * RESOLUTION:			16 BIT
		 * AVERAGED SAMPLES:	16
		 * REFERENCE:			1 / 1.48 VDDANA reference [2.23 VDC]
		 *////////////////////////////////////////////////////////////////
		LED_0_on();
		uint8_t ADC_11_raw_buffer[ADC_11_raw_buffer_size];						//Initializing an ADC buffer for raw outputs
		int ADC_11_ave_buffer[ADC_11_ave_buffer_size];							//Initializing an averaging buffer for the raw values	

		for (int i = 0; i < ADC_11_ave_buffer_size; i++){
			adc_sync_read_channel(&ADC_0, ADC_11_channel, ADC_11_raw_buffer, ADC_11_raw_buffer_size);
			ADC_11_value = ADC_11_raw_buffer[1];								//See adcbuff value
			ADC_11_value = ADC_11_value << 8;									//Moving to the upper 8 bits
			ADC_11_value = ADC_11_value | ADC_11_raw_buffer[0];					//Appending lower 8 bit value to previous value			//16-bit ADC value:
			//printf("\nADC 11 Value: %i",ADC_11_value);						//print integer readout to COM 1
			ADC_11_ave_buffer[i] = ADC_11_value;
			delay_ms(1);
		}

		//CALCULATING AVERAGE ADC CHANNEL 11 VALUE
		ADC_11_value = average(ADC_11_ave_buffer, ADC_11_ave_buffer_size);
		printf("\nADC 11:\t%i",ADC_11_value);
		
		ADC_11_volts = ((float)ADC_11_value / (float)ADC_11_fsr) * (float)ADC_11_voltage_reference;			//CALCULATING VOLTAGE READING FOR THE ADC 11
		ADC_11_pressure = map(ADC_11_volts,voltage_at_min,voltage_at_max,min_pressure,max_pressure);		//CALCULATING PRESSURE READING FOR THE ADC 11
		ADC_11_pressure_int = ADC_11_pressure;
		
		if (ADC_11_pressure_int <= 0){
		ADC_11_pressure_int = 0;	
		}
				
		char ADC_11_pressure_str_buffer[3] = {0};							//string buffer to print floats to 
		gcvt(ADC_11_volts,3,ADC_11_pressure_str_buffer);					//convert float to string that is 3 characters
		printf("\tVolts:\t%.3s", ADC_11_pressure_str_buffer);						//prints 3 characters of sting buffer
		
		//gcvt(ADC_11_pressure,3,ADC_11_pressure_int);						//string buffer to print floats to 
		printf("\tPressure:\t%.3i psi", ADC_11_pressure_int);				//convert float to string that is 3 characters
		
		LED_0_off(); //Visually indicate the Red cycle has concluded
	}	//END ADC_11_READ

	//ADC ACERAGING FUNCTION
	float average(int* values, int size){
		int sum = 0;
		for (int i = 0; i < size; i++){
			sum += values[i];
		}
		return (float)sum / size;
	}