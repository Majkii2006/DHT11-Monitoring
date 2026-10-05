#include <dht.h>
#include <sys/types.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "7SEG_MODULE.h"

#include "driver/gpio.h"

#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

SevenSegment_t display;

const uint8_t segPins[] = { 14, 32, 33, 26, 25, 27, 22 }; //from A to G ordered
const uint8_t digPins[] = { 19, 23 };


void screen_task(void* params) {
	while(1) {
		display_worker(&display);
		vTaskDelay(pdMS_TO_TICKS(10));
	}	
	
}

void logic_task(void* params) {
	while(1) {
		display_setNumber(&display, 26);
		vTaskDelay(pdMS_TO_TICKS(1000));
		display_setTempUnit(&display, 'C');
		vTaskDelay(pdMS_TO_TICKS(2000));
		display_setNumber(&display, 85);
		vTaskDelay(pdMS_TO_TICKS(1000));
		display_setHumidityUnit(&display);
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
}





void app_main(void) {

	display_init(&display, segPins, 7, digPins, 2, true);

	xTaskCreate(screen_task, "Screen Task", 1024, NULL, 1, NULL);
	xTaskCreate(logic_task, "Logic Task", 1024, NULL, 1, NULL);
	


}
