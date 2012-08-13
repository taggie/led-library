/*
 * LED.cpp
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 13-08-2012
*/
#include "LED.h"
#include "Calculate.h"

/* The animation Objects that are used */
  Animation * intensityAnim;
  
/* CONSTRUCTORS */

/** Default Constructor, make sure to set the channel correctly later on! **/
LED::LED()
{ 
  _x          	=  	0;
  _y          	=  	0;
  _intensity 	=  	0;
  _channel   	=  	0;
  _on		 	=	true;
  _autoWrite	=	true;
  intensityAnim  =  new Animation(); 
  _hasNewValue = true;
  update();
}

/** Constructor with supplied channel.
	autoWrite is true by default but can be set to false 
	if autoWrite == true, arduino will actuate the LEd on pin 'channel' when update() is called.
	**/
LED::LED( uint16_t channel, bool autoWrite )
{
  _x          	=  	0;
  _y          	=  	0;
  _intensity  	=  	0;
  _channel    	=  	channel;
  _on		 	=	true;
  _autoWrite	=	autoWrite;
  intensityAnim  =  new Animation(); 
  if ( _autoWrite )
  {
	pinMode( _channel, OUTPUT );
  }
  _hasNewValue = true;
  update();
}

LED::LED( uint16_t channel, bool autoWrite, uint16_t x, uint16_t y )
{
  _x          	=  	x;
  _y          	=  	y;
  _intensity  	=  	0;
  _channel    	=  	channel;
  _on		 	=	true;
  _autoWrite	=	autoWrite;
  intensityAnim  =  new Animation(); 
  
  if ( _autoWrite )
  {
	pinMode( _channel, OUTPUT );
  }
  _hasNewValue = true;
  update();
}

LED::~LED()
{
}

/* VOID FUNCTIONS */

/** Update Function; should be called every loop to set, animate and actuate the lamps **/
void LED::update()
{
	if( intensityAnim->isAnimating() )
	{
		setIntensity( intensityAnim->getValue(), false );
	}
	/* If there is no animation going on, and the device is turned off,
		and the intensity is not set to 0, turn the LED off
	*/
	if( !intensityAnim->isAnimating() && !_on && _intensity > 0 )
	{
		setIntensity( 0, false );
	}
	
	/* Here we actually actuate the light if autowrite is on */
	if( _autoWrite )
	{
		if ( hasNewValue() )
		{
			if ( _on )
			{
				analogWrite(_channel, _intensity);
			}
			else 
			{
				analogWrite(_channel, 0);
			}
		}
	}
}

/** Immediatily sets the intensity of the lamp (range 0 - 255). 
	Values are changed immediatily, but only actuated in the update() function.
	An ongoing intensity animation is stopped unless stopAnimation is set to false. 
**/
void LED::setIntensity( uint8_t intensity, bool stopAnimation )
{
	if( intensity != _intensity )
	{
		_intensity = constrain( intensity, 0, 255 );
		_hasNewValue = true;
		
		if (stopAnimation)
		{
			intensityAnim->stopAnimation();
		}
	}
}
void LED::setX( uint16_t x )
{
	_x	=	x;
}
void LED::setY( uint16_t y )
{
	_y	=	y;
}

void LED::setPosition( uint16_t x, uint16_t y )
{
	_x	=	x;
	_y	=	y;
}

/** Allows you to set the channel of the LED. if autoWrite is on, this is the Arduino pin that is actuated**/
void LED::setChannel( uint16_t channel )
{
	_channel	=	channel;
	if ( _autoWrite )
	{
		pinMode( _channel, OUTPUT );
	}
}

/** Allows different animation types with easing to be set. 
	Animation types are LINEAR and QUADRATIC **/
void LED::setAnimationType( uint8_t animType, bool easeIn, bool easeOut )
{
	intensityAnim->setAnimationType( animType, easeIn, easeOut );
}

/** Toggles LED on or off (inverts current state). **/
void LED::toggle()
{
	_on	=	!_on;
	_hasNewValue = true;
}

/** Turns the LED on or off depending on the supplied argument **/
void LED::setToggle( bool toggle )
{
	_on	=	toggle;
	_hasNewValue = true;
}

/** Animates the intensity of the LED to the desired value (range 0 - 255). 
	Duration of the animation can be set in millis.
**/
void LED::intensityTo( uint8_t to, uint32_t duration )
{
	intensityAnim->startAnimation( _intensity, constrain(to, 0, 255), duration );
}

/** Animates the intensity of the LED to the desired value from a set startingpoint (range 0 - 255). 
	At the start of the animation the LED will jump to the set start value.
	Duration of the animation can be set in millis.
**/
void LED::intensityFromTo( uint8_t from, uint8_t to, uint32_t duration )
{
	intensityAnim->startAnimation( from, constrain(to, 0, 255), duration, LINEAR);
}

void LED::moveTo( uint16_t newX, uint16_t newY, uint32_t duration )
{
	//Something will happen here
}

/* bool FUNCTIONS */

/** Returns true if the LED is on; this variable is independant of the intensity
**/
bool LED::isOn()
{
	return _on;
}

/** Returns true if the LED is off; this variable is independant of the intensity
**/
bool LED::isOff()
{
	return !_on;
}

/** Returns true if the LED is animating.
**/
bool LED::isAnimating()
{
	return intensityAnim->isAnimating();
}

/** Returns true if the LED has a new value. This is particularly interesting if autoWrite == false.
	If it returns true, you can for instance update your DMX channels.
**/
bool LED::hasNewValue()
{
	if (_hasNewValue)
	{
		_hasNewValue = false;
		return true;
	}
	else 
	{
		return false;
	}
}

/* INTEGER FUNCTIONS */

/** Returns the channel of the LED
**/
uint8_t LED::getChannel()
{
	return _channel;
}

/** Returns the current intensity of the LED; if the LEd is off it returns 0
**/
uint8_t LED::getIntensity()
{
	if (_on)
	{
		return _intensity;
	}
	else 
	{
		return 0;
	}
}

/* uint16_t FUNCTIONS */
uint16_t LED::getX()
{
	return _x;
}
uint16_t LED::getY()
{
	return _y;
}