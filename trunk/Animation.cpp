/*
 * Animation.cpp
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 13-08-2012
*/
#include "Animation.h"
#include "Calculate.h"

/** DEFAULT CONSTRUCTOR **/
Animation::Animation()
{
	_startValue 	=  	0;
	_endValue 		=  	0;
	_startTime		= 	0;
	_endTime		= 	0;
	_isAnimating	=	false;
	_animType		= 	0;
	_easeIn			= true;
	_easeOut		= true;
}

Animation::~Animation()
{
}

/* Returns true if the animation is ongoing */
bool Animation::isAnimating()
{
	return _isAnimating;
}

void Animation::startAnimation(int16_t startValue, int16_t endValue, uint32_t duration, bool throughZero) 
{
	_startValue 	=  	startValue;
	_endValue 		=  	endValue;
	_startTime		= 	millis();
	_endTime		= 	millis() + duration;
	_isAnimating	=	true;
	_shortcutThroughZero = throughZero;
	
	/*
	Shortcut through zero allows use of the hue color wheel as expected. 
	If the shortest ruote from start value to end value goes through zero (red)
	we will take this route by default. 
	*/
	if ( _shortcutThroughZero ) {
		/* Start and End values are signed to show the negatives */
		if ( startValue - endValue > 127 ) 
		{
			// clockwise (++) through zero
			_endValue += 255;
		}
		else if (endValue - startValue > 127 ) 
		{
			// counter clockwise (--) through zero
			_startValue += 255;
		}
		else 
		{
			_shortcutThroughZero = false;
		}
	}
	
}

void Animation::stopAnimation() 
{
	_isAnimating = false;
}

void Animation::setAnimationType( uint8_t animType, bool easeIn, bool easeOut)
{
	_animType = animType;
	_easeIn = easeIn;
	_easeOut = easeOut;
}

uint16_t Animation::getValue() 
{
	uint16_t animatedValue	=	0;
	//Check if the animation is still valid
	if( millis() < _endTime )
	{
		uint32_t t	=	( millis() - _startTime );			//current time
		uint16_t b	=	_startValue;						//begin value
		int      c	=	( _endValue - _startValue );		//change (may be negative)
		uint32_t d	=	_endTime - _startTime;				//duration
		switch( _animType )
		{
			case QUADRATIC:
				animatedValue	=	Calculate.quadratic( (float)t, (float)b, (float)c, (float)d, _easeIn, _easeOut);
				break;
			case EXPONENTIAL:
				animatedValue	=	Calculate.exponential( (float)t, (float)b, (float)c, (float)d, _easeIn, _easeOut );
				break;
			case CIRCULAR:
				animatedValue	=	Calculate.circular( (float)t, (float)b, (float)c, (float)d, _easeIn, _easeOut );
				break;
			case SINUS:
				animatedValue	=	Calculate.sinus( (float)t, (float)b, (float)c, (float)d, _easeIn, _easeOut );
				break;		
			default:
				// Functions other than linear may not work as the float of c can somehow not be neagtive.
				animatedValue	=	Calculate.linear( t, b, c, d );
				break;
			
		}
		
		if (_shortcutThroughZero) 
		{
			if ( animatedValue > 255 ) 
			{
				animatedValue -= 255;
			}
		}
		return animatedValue;
	}
	else if( millis() >= _endTime)
	{
		_isAnimating = false;
		return _endValue;
	}
}