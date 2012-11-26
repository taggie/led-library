/*
 * LED.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 13-08-2012
*/

#ifndef LED_h
#define LED_h

/* The following code makes the Library compatible with Arduino 1.0 */
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
	#include "cppfix.h"
#endif

/* */
#ifndef DEFAULT_DURATION
	#define DEFAULT_DURATION 1000
#endif

#include "Animation.h"
	
/* Animation constants 
static const uint8_t LINEAR 		= 0;
static const uint8_t QUADRATIC 		= 1;
static const uint8_t EXPONENTIAL 	= 2;
static const uint8_t CIRCULAR 		= 3;
static const uint8_t SINUS 			= 4;
*/

static const uint8_t PARAM_INTENSITY	= 0;
static const uint8_t PARAM_CCT			= 1;
static const uint8_t PARAM_HUE			= 2;
static const uint8_t PARAM_SATURATION	= 3;

/*! \mainpage The Arduino LED Library
 *	This Arduino Library is used to manage and control LEDs from the Arduino environment. It allows animation and actuation of single channel LEDs 
	as well as RGB and CCT (color temperature) LEDs.  <br /><br />
	<a href="functions_func.html">All available functions are documented here</a> 
 */

/** \brief The LED class is the main class which is used to manage LEDs and contains all functionality for for single channel LEDs **/

/** The LED class is the main class which is used to manage LEDs and contains all functionality for for single channel LEDs **/

class LED
{
  public:
    LED();
    LED( uint16_t channel, bool autoWrite = true );
	LED( uint16_t channel, bool autoWrite, uint16_t x, uint16_t y );
    ~LED();
    
    void update();
	void setBrightness( uint8_t intensity, bool stopAnimation = true );
    void setIntensity( uint8_t intensity, bool stopAnimation = true );
    void setX( uint16_t x );
    void setY( uint16_t y );
    void setPosition( uint16_t x, uint16_t y );
    void setChannel( uint16_t channel );
	void setAnimationType( uint8_t animType = LINEAR, bool easeIn = true, bool easeOut = true );
	
    void toggle();
	void setToggle( bool toggle );
    void intensityTo( uint8_t to, uint32_t duration = DEFAULT_DURATION );
    void intensityFromTo( uint8_t from, uint8_t to, uint32_t duration = DEFAULT_DURATION );
    void moveTo( uint16_t newX, uint16_t newY, uint32_t duration = DEFAULT_DURATION );

    bool isOn();
    bool isOff();
    bool isAnimating();
	bool hasNewValue();
    
    uint8_t getChannel();
    uint8_t getIntensity();
	uint8_t getTargetIntensity();
    
    uint16_t getX();
    uint16_t getY();
	
	/** The animation objects that is used to animate the LED **/
	Animation * intensityAnim;

  protected:
  	bool _on, _autoWrite, _hasNewValue;
    uint16_t _channel; 
    uint16_t _x, _y;
	uint8_t _intensity;
};

#endif

