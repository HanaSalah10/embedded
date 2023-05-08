#include "tm4c123gh6pm.h"
#include "bit_utilies.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "UART.h"
#include "GPIO.h"
#include "Lcd.h"
#include "systick.h"


int main()

{
	
	volatile float dist=0;//variable to save distance after calling function that calculate distance between two points
	float lat_arr[1000];/*array to save latitude */
	float long_arr[1000];/*array to save current Longitude*/
	float displacement;
    int counter=0;/*variable used as acounter to increased every cycle of distance calc (every second) */
	UART_Init(); /*initialize the uart of the gps*/	
	GPIO_initPORTF(); // initialize port f for leds
	LCD_init(); /*initialize the gpio port of the lcd currently B*/
	SysTick_Init(); //initialize systick for delays
		
		While(1)
	{
		GPS_read();
		GPS_format();
		
		float long_arr[counter] = currentLong;
		float lat_arr[counter] = currentLat;
		
		if(counter!=0)
			{
				dist += GPS_getDistance(lat_arr[counter-1],lat_arr[counter],long_arr[counter-1],long_arr[counter]);
				
				LCD_Clear();/*clear screen to not overwrite on screen*/
				LCD_displayfloat(dist);/*display distance on lcd*/
				
				displacement = GPS_getDistance(lat_arr[counter],finallat,long_arr[counter],finallong);
				//see 
				if(displacement>=100)
				{ 
			        led_on(GPIO_RED_LED);/*turn on red led when distance >100*/   
			    }
				
				if(displacement>=0 && displacement<1)
				{
					led_on(GPIO_GREEN_LED); /*turn on green led when 0<distance <1*/
				}	
				if(displacement>1 && displacement<100)
				{
					led_on(GPIO_RED_LED);
					led_on(GPIO_GREEN_LED); //turn on yellow led when 1<distance<100
				}	
		    }
			counter++;/*increase counter by 1*/
			SysTick_Wait1ms(3000);//delay 3 sec for gps to recieve new data
		
	}
	
}