/**
 * LED.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 13-08-2012
**/

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
	
  	/* Animation constants */
	static const uint8_t LINEAR 		= 0;
  	static const uint8_t QUADRATIC 		= 1;
  	static const uint8_t EXPONENTIAL 	= 2;
  	static const uint8_t CIRCULAR 		= 3;
  	static const uint8_t SINUS 			= 4;

	static const uint8_t INTENSITY		= 0;
	static const uint8_t CCT			= 1;
	static const uint8_t HUE			= 2;
	static const uint8_t SATURATION		= 3;

class LED
{
  public:
  
    LED();
    LED( uint16_t channel, bool autoWrite = true );
	LED( uint16_t channel, bool autoWrite, uint16_t x, uint16_t y );
    ~LED();
    
    void update();    
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
    
    uint16_t getX();
    uint16_t getY();
	
	/** The animation Objects that are used **/
	Animation * intensityAnim;

  protected:
  	bool _on, _autoWrite, _hasNewValue;
    uint16_t _channel; 
    uint8_t _intensity;
    uint16_t _x, _y;
};

#endif

