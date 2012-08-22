/*
 * Animation.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 13-08-2012
*/

#ifndef Animation_h
#define Animation_h


/* The following code makes the Library compatible with Arduino 1.0 */
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
	#include "cppfix.h"
#endif

#ifndef DEFAULT_DURATION
	#define DEFAULT_DURATION 1000
#endif

	static const uint8_t LINEAR 		= 0;
  	static const uint8_t QUADRATIC 		= 1;
  	static const uint8_t EXPONENTIAL 	= 2;
  	static const uint8_t CIRCULAR 		= 3;
  	static const uint8_t SINUS 			= 4;
	

class Animation
{
  public:

	Animation();
	~Animation();

	uint16_t getValue();
	void stopAnimation();
	void startAnimation(int16_t startValue, int16_t endValue, uint32_t duration, bool throughZero = false);
	void setAnimationType( uint8_t animType = LINEAR, bool easeIn = true, bool easeOut = true );
	bool isAnimating();
	
  protected:
	uint32_t	_startTime;
	uint32_t	_endTime;
	uint16_t	_startValue;
	uint16_t	_endValue;
	uint8_t	_isAnimating;
	uint8_t	_animType;
	
	bool _easeIn;
	bool _easeOut;
	bool	_shortcutThroughZero;
};

#endif

