#pragma once

#ifndef SEG_MODULE_H
#define SEG_MODULE_H

#define MAX_DIGITS 2 //for the compile-time memory allocating on the stack so we need to know how much of memory we need
                     // CHANGE THIS VALUE FOR THE OPERATIONS ON MORE THAN 8 DIGITS DISPLAY
#define MAX_SEGMENTS 7 // CHANGE THIS VALUE FOR THE OPERATIONS ON MORE THAN 7 SEGMENTS

#include <stdint.h>


typedef struct {
	const uint8_t* segmentPins; // collection containing mapping segments of display to the each pin
	const uint8_t* digitPins; // collection conating mapping numbers of display to the each pin
	uint8_t numSegments; // how many segments for the one number -> default 7 (without DP) or 8 (with DP)
	uint8_t numDigits; // how many digits have full display 
	bool isAnode; // is the display controlled by shared anode or shared kathode 
	uint8_t displayBuffer[MAX_DIGITS][MAX_SEGMENTS]; // the compile-time buffer for the concrete numbers from that main one that we would to show
	uint8_t whatNumber;
} SevenSegment_t;


void display_init(SevenSegment_t* display, const uint8_t segPins[], uint8_t nSeg, const uint8_t digPins[], uint8_t nDig, bool isAno);
void display_setNumber(SevenSegment_t* display, long number);
void display_setTempUnit(SevenSegment_t* display, char unit);
void display_setHumidityUnit(SevenSegment_t* display);
void display_worker(SevenSegment_t* display);







#endif /* ifndef SEG_MODULE_H */
