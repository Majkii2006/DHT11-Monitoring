#include <sys/types.h>

#include <stdint.h>
#include <uchar.h>

#include "7SEG_MODULE.h"

#include "esp_etm.h"
#include "esp_rom_sys.h"

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

#include "hal/gpio_types.h"

#include "rom/rtc.h"

static const uint8_t digitPattern[] = {
	0b00111111, // 0 its for the 0b0GFEDCBA pattern, for default kathode controlled display 
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
        0b00000111, // 7
	0b01111111, // 8
	0b01101111  // 9
};


static const uint8_t charPattern[] = {
    ['A'] = 0b01110111,
    ['b'] = 0b01111100,
    ['C'] = 0b00111001,
    ['c'] = 0b01011000,
    ['o'] = 0b01011100,
    ['%'] = 0b01100011,
    ['d'] = 0b01011110,
    ['E'] = 0b01111001,
    ['F'] = 0b01110001,
    ['H'] = 0b01110110,
    ['L'] = 0b00111000,
    ['P'] = 0b01110011,
    ['U'] = 0b00111110,
    ['@'] = 0b01100011
};


void display_init(SevenSegment_t* display, const uint8_t segPins[], uint8_t nSeg, const uint8_t digPins[], uint8_t nDig, bool isAno) {

	//overfill protection	
	if (nDig > MAX_DIGITS) { 
		nDig = MAX_DIGITS;
	}

	display->segmentPins = segPins;
	display->digitPins = digPins;
	display->numDigits = nDig;
	display->numSegments = nSeg;
	display->isAnode = isAno;

	//initialization as output
	//initialization segments:
	
	for (uint8_t i = 0; i < display->numSegments; i++) {
		gpio_set_direction(display->segmentPins[i], GPIO_MODE_OUTPUT);
	}	

	//initializations displays:
	
	for (uint8_t i = 0; i < display->numDigits; i++) {
		gpio_set_direction(display->digitPins[i], GPIO_MODE_OUTPUT);
		gpio_set_level(display->digitPins[i], display->isAnode ? 0 : 1); 
	}

}

void display_setHumidityUnit(SevenSegment_t *display) {
	if ( MAX_DIGITS < 2) {
		return;
	}

	int8_t actual_screen_index = (int8_t) display->numDigits - 1;
	uint8_t mask_P = charPattern['%'];
	uint8_t mask_c = charPattern['o'];
	
	for (uint8_t actual_segment = 0; actual_segment < display->numSegments; actual_segment++) {
		uint8_t bit = (mask_P >> actual_segment) & 1;
		display->displayBuffer[actual_screen_index - 1][actual_segment] = bit;
	}

	for (uint8_t actual_segment = 0; actual_segment < display->numSegments; actual_segment++) {
		uint8_t bit = (mask_c >> actual_segment) & 1;
		display->displayBuffer[actual_screen_index][actual_segment] = bit;
	}
}



void display_setTempUnit(SevenSegment_t* display, char unit) {

	for (int8_t actual_screen_index = (int8_t) display->numDigits - 1; actual_screen_index >= 0; actual_screen_index--) {
		uint8_t mask_degree = charPattern['@']; // degree

		for (uint8_t actual_segment = 0; actual_segment < display->numSegments; actual_segment++) {
			uint8_t bit = (mask_degree >> actual_segment) & 1;
			display->displayBuffer[actual_screen_index][actual_segment] = bit;
		}	

		if (unit == 'C') {
			uint8_t mask = charPattern['C'];
			for (uint8_t actual_segment = 0; actual_segment < display->numSegments; actual_segment++) {
				uint8_t bit = (mask >> actual_segment) & 1;
				display->displayBuffer[actual_screen_index + 1][actual_segment] = bit;
			}
		}
		else if (unit == 'F') {
			uint8_t mask = charPattern['F'];
			for (uint8_t actual_segment = 0; actual_segment < display->numSegments; actual_segment++) {
				uint8_t bit = (mask >> actual_segment) & 1;
				display->displayBuffer[actual_screen_index + 1][actual_segment] = bit;
			}
		}
	}		
}

void display_setNumber(SevenSegment_t* display, long number) {
	//need to separate the long number provided by the user and then save this numbers to the buffer displayBuffer[]
	
	//check if the number is negative => show the number without the sign
	if (number < 0) number = -number;	

	for (int8_t actual_screen_index = (int8_t) display->numDigits - 1; actual_screen_index >= 0; actual_screen_index--) {

		uint8_t digit = number % 10;	
		number = number / 10;
		
		uint8_t mask = digitPattern[digit];

		for (uint8_t actual_segment = 0; actual_segment < display->numSegments; actual_segment++) {
			uint8_t bit = (mask >> actual_segment) & 1;	
			display->displayBuffer[actual_screen_index][actual_segment] = bit;
		}
	}

} 



// helper function for segments setting

static inline void dpSetSegments(SevenSegment_t* display, uint8_t digitIndex) {
	for (uint8_t actual_segment = 0; actual_segment < display->numSegments; actual_segment++) {
		uint8_t bit = display->displayBuffer[digitIndex][actual_segment];
		gpio_set_level(display->segmentPins[actual_segment], display->isAnode ? !bit : bit);
	}
}

void display_worker(SevenSegment_t* display) {
	//worker should only be the thing that displaying the number 		
	//worker only cares about the multiplexing 
	
	//turn off whole screen on each cycle
	for(uint8_t actual_number = 0; actual_number < display->numDigits; actual_number++) {
		gpio_set_level(display->digitPins[actual_number], display->isAnode ? 1 : 0);
	}	

	dpSetSegments(display, display->whatNumber);

	gpio_set_level(display->digitPins[display->whatNumber], display->isAnode ? 0 : 1);
	
	display->whatNumber++;

	if (display->whatNumber >= display->numDigits) {
		display->whatNumber = 0;	
	}

}







