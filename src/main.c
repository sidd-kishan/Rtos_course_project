/*

This program blinks the onboard led, reads and onboard temperature
value and prints the context switch count of FreeRTOS Kernel
implemented in the Raspberry Pi Pico microcontroller.

Copyright (C) <2023>  <Emmanuel J>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
https://github.com/talon414

*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "conswitch.h"

#define LED_PIN PICO_DEFAULT_LED_PIN
#define TEMPERATURE_UNITS 'C'

void LED_BLINK(){
        for(;;){
                gpio_put(LED_PIN, 1);
                vTaskDelay(250);
                gpio_put(LED_PIN, 0);
                vTaskDelay(250);
        }
}

void PRINT_CONTEXT_SWITCH(){
        for(;;){
                printf("Context Switch Count:%lld\n", CONTEXT_SWITCH_COUNT);
                vTaskDelay(400);
        }
}

void TEMP_SENSE(){
        for(;;){
                const float conversion_factor = 3.3f / (1 << 12);
                float result = (float)adc_read() * conversion_factor;
                float tempC = 27.0f - (result - 0.706f) / 0.001721f;
                printf("Onboard temperature = %.02f %c\n", tempC, TEMPERATURE_UNITS);
                vTaskDelay(2000);
        }
}

int main(){
        stdio_init_all();
        adc_init();

        adc_set_temp_sensor_enabled(true);
        adc_select_input(4);

        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);
        gpio_put(LED_PIN,1);

        printf("Pico powered on\n");

        TaskHandle_t led_pin = NULL;
        TaskHandle_t context_switch = NULL;
        TaskHandle_t temp_sense = NULL;

        uint32_t task_led = xTaskCreate(
                           LED_BLINK,
                           "LEDBLINK",
                           1024,
                           NULL,
                           1,
                           &led_pin
        );

        uint32_t task_context_switch = xTaskCreate(
                PRINT_CONTEXT_SWITCH,
                "pCONTEXTSW",
                2048,
                NULL,
                3,
                &context_switch
        );
        
         uint32_t task_tempsense = xTaskCreate(
                TEMP_SENSE,
                "temp",
                2048,
                NULL,
                2,
                &temp_sense
        );

       

        vTaskStartScheduler();

        for (;;){
                // code never gets here
        }
}
