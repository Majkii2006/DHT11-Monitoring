#include <dht.h>
#include <sys/types.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "7SEG_MODULE.h"
#include "esp_err.h"

#include "driver/gpio.h"

#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

#define DHT_PIN 13
#define DHT_TYPE DHT_TYPE_DHT11
#define QUEUE_LENGTH 5
#define TEMP_UNIT 'C'


SevenSegment_t display;
QueueHandle_t queue;

const uint8_t segPins[] = { 14, 32, 33, 26, 25, 27, 22 }; //from A to G ordered
const uint8_t digPins[] = { 19, 23 };

typedef struct {
	int16_t temperature;
	int16_t humidity;
} Reading;



void read_sensor(void* params) {
	Reading reading;
	gpio_reset_pin(DHT_PIN);
	while(1) {

		 esp_err_t res = dht_read_data(DHT_TYPE, DHT_PIN, &reading.humidity, &reading.temperature);	
		 if (res != ESP_OK) {
		 	printf("ERROR! <=> Can't read from the sensor, retrying...");
		 }
		xQueueSend(queue, &reading, 100);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


void screen_task(void* params) {
	while(1) {
		display_worker(&display);
		vTaskDelay(pdMS_TO_TICKS(10));
	}	
	
}

void logic_task(void* params) {
	Reading reading;
	while(1) {
		xQueueReceive(queue, &reading, 10);
		printf("Temperature: %d \n", reading.temperature / 10);
		printf("Humidity: %d \n", reading.humidity / 10);
		display_setNumber(&display, reading.temperature / 10);
		vTaskDelay(pdMS_TO_TICKS(1000));
		display_setTempUnit(&display, TEMP_UNIT);
		vTaskDelay(pdMS_TO_TICKS(1000));
		display_setNumber(&display, reading.humidity / 10);
		vTaskDelay(pdMS_TO_TICKS(1000));
		display_setHumidityUnit(&display);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


void app_main(void) {

	queue = xQueueCreate(QUEUE_LENGTH, sizeof(Reading*));

	display_init(&display, segPins, 7, digPins, 2, true);

	xTaskCreatePinnedToCore(screen_task, "Screen Task", 1024, NULL, 3, NULL, 0);
	xTaskCreatePinnedToCore(logic_task, "Logic Task", 1024, NULL, 3, NULL, 0);
	xTaskCreatePinnedToCore(read_sensor, "Read Sensor", 1024, NULL, 1, NULL, 1);
	


}
