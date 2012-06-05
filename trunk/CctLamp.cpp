/**
 * CctLamp.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 30-05-2012
**/
#include "CctLamp.h"
#include "Calculate.h"

/** CONSTRUCTORS **/
CctLamp::CctLamp()
{
  _x          	=  	0;
  _y          	=  	0;
  _intensity 	=  	0;
  _cct			=	50;
  _channel	 	=  	0;
  _channelWarm 	=  	0;
  _channelCool 	=  	1;
  _on		 	=	true;
  _autoWrite	=	false;
  _isAnimating	=	false;
  _animType		= 	0;
  // Serial.println("EMPTY CONSTRUCTIR CALL");
}
CctLamp::CctLamp( uint16_t channelWarm  )
{
  _x          	=  	0;
  _y          	=  	0;
  _intensity 	=  	0;
  _cct			=	50;
  _channel	 	=  	channelWarm;
  _channelWarm 	=  	channelWarm;
  _channelCool 	=  	channelWarm + 1;
  _on		 	=	true;
  _autoWrite	=	false;
  _isAnimating	=	false;
  _animType		= 	0;
}
CctLamp::CctLamp( uint16_t channelWarm, bool autoWrite )
{
  _x          	=  	0;
  _y          	=  	0;
  _intensity  	=  	0;
  _cct			=	50;
  _channel	 	=  	channelWarm;
  _channelWarm 	=  	channelWarm;
  _channelCool 	=  	channelWarm + 1;
  _on		 	=	true;
  _autoWrite	=	autoWrite;
  _isAnimating	=	false;
  _animType		= 	0;
}
CctLamp::CctLamp( uint16_t channelWarm, uint16_t channelCool, bool autoWrite )
{
  _x          	=  	0;
  _y          	=  	0;
  _intensity  	=  	0;
  _cct			=	50;
  _channel	 	=  	channelWarm;
  _channelWarm 	=  	channelWarm;
  _channelCool 	=  	channelCool;
  _on		 	=	true;
  _autoWrite	=	autoWrite;
  _isAnimating	=	false;
  _animType		= 	0;
}
CctLamp::CctLamp( uint16_t channelWarm, uint16_t channelCool, bool autoWrite, uint16_t x, uint16_t y )
{
  _x          	=  	x;
  _y          	=  	y;
  _intensity  	=  	0;
  _cct			=	50;
  _channel	 	=  	channelWarm;
  _channelWarm 	=  	channelWarm;
  _channelCool 	=  	channelCool;
  _on		 	=	true;
  _autoWrite	=	autoWrite;
  _isAnimating	=	false;
  _animType		= 	0;
  
  	/*Serial.println("	Constructing CctLamp:: ");
	Serial.println("		[  x: "+String(int(_x),DEC)+"]");
	Serial.println("		[  y: "+String(int(_y),DEC)+"]");
	Serial.println("		[int: "+String(_intensity,DEC)+"]");
	Serial.println("		[cha: "+String(_channel,DEC)+"]");
	Serial.println("		[ani: "+String(_isAnimating, BIN)+"]");*/
}

CctLamp::~CctLamp()
{
}

/** VOID FUNCTIONS **/

void CctLamp::setCct( uint8_t cct )
{
	if( cct != _cct )
	{
		_cct = constrain( cct, 0, 100 );
		_hasNewValue = true;
	}
}

void CctLamp::setIntensityWarm( uint8_t intensityWarm )
{
	if( intensityWarm != _intensityWarm )
	{
		_intensityWarm = constrain( intensityWarm, 0, 255 );
	}
}

void CctLamp::setIntensityCool( uint8_t intensityCool )
{
	if( intensityCool != _intensityCool )
	{
		_intensityCool = constrain( intensityCool, 0, 255 );
	}
}

void CctLamp::cctTo( uint8_t to, uint32_t duration )
{
	_isAnimating	=	true;
	_startTime		=	millis();
	_endTime		=	millis() + duration;
	_startCct		= 	_cct;
	/* the _startIntensity value is also updated to the 
	current _intensity to make sure that 
	an ongoing animation of intensity is not disturbed 
	*/
	_startIntensity		= 	_intensity;
	_endCct 		=	constrain(to, 0, 100);
	_animType		= 	0;
}

void CctLamp::intensityTo( uint8_t to, uint32_t duration )
{
	_isAnimating	=	true;
	_startTime		=	millis();
	_endTime		=	millis() + duration;
	_startIntensity		=	_intensity;
	/* the _startCct value is also updated to the 
	current _cct to make sure that an ongoing 
	animation of cct is not disturbed 
	*/
	_startCct		= 	_cct;
	_endIntensity	=	to;
	_animType		= 	0;
}

/** Sets CCT Lamp with all its values; intensity, cct value and duration in millis **/
void CctLamp::cctLampTo( uint8_t intens, uint8_t cct, uint32_t duration )
{
	_isAnimating	=	true;
	_startTime		=	millis();
	_endTime		=	millis() + duration;
	_startIntensity		=	_intensity;
	_startCct		= 	_cct;
	_endIntensity	=	intens;
	_endCct			=	constrain(cct, 0, 100);
	_animType		= 	0;
}

void CctLamp::update()
{
/*
	Serial.println("update:: ");
	Serial.print("anim ");
	Serial.print(_isAnimating, DEC);
	Serial.print(" - intens ");
	Serial.println(_intensity, DEC);
*/	
	if( _isAnimating )
	{
		//Check if the animation is still going on
		if( millis() < _endTime )
		{
			uint32_t t	=	( millis() - _startTime );	// current time
			uint16_t bi	=	_startIntensity;					// begin intensity
			uint16_t bc	=	_startCct;					// begin cct
			int      ci	=	( _endIntensity - _startIntensity );	// difference in intensity 	(may be negative, this results in problems in some _animTypes)
			int      cc	=	( _endCct - _startCct );	// difference in cct 		(may be negative, this results in problems in some _animTypes)
			uint32_t d	=	_endTime - _startTime;		// duration
			
			switch( _animType )
			{
				/* FIX: Functions other than linear may not work as the float of c can somehow not be neagtive. */
				/*case QUADRATIC:
					animatedIntensity	=	Calculate.quadratic( (float)t, (float)b, (float)c, (float)d, true, true);
					break;
				case EXPONENTIAL:
					animatedIntensity	=	Calculate.exponential( (float)t, (float)b, (float)c, (float)d, true, true );
					break;
				case CIRCULAR:
					animatedIntensity	=	Calculate.circular( (float)t, (float)b, (float)c, (float)d, true, true );
					break;
				case SINUS:
					animatedIntensity	=	Calculate.sinus( (float)t, (float)b, (float)c, (float)d, true, true );
					break;	*/	
				default:
					/* we are animating the intensity and/or cct 
					change the values that are being animated */
					if (_endIntensity != _intensity) 	
					{ 	
						setIntensity(	Calculate.linear( t, bi, ci, d ) );
					}
					if (_endCct != _cct) 		// we are animating the CCT
					{ 			
						setCct(			Calculate.linear( t, bc, cc, d ) );
					}
				break;
			}
				
			/* calculate and store warm and cool values for actuation */
			setIntensityWarm( calculateIntensityWarm() );
			setIntensityCool( calculateIntensityCool() );
		}
		else if( millis() >= _endTime)
		{
			/* calculate and store warm and cool values for actuation */
			setIntensityWarm( calculateIntensityWarm() );
			setIntensityCool( calculateIntensityCool() );
			/* Store cct and intensity values for reference */
			setIntensity( _endIntensity );
			setCct( _endCct );
			_isAnimating = false;
		}
		
		if( _autoWrite )
		{
			analogWrite(_channelWarm, _intensityWarm);
			analogWrite(_channelCool, _intensityCool);
		}
	}
	/** If there is no animation going on,
		and the device is turned off,
		and the intensity is not set to 0,
		turn the LED off
	**/
	if( !_isAnimating && !_on && _intensity > 0 )
	{
		setIntensity( 0 );
	}
}

/** INTEGER FUNCTIONS **/
uint16_t CctLamp::getChannelWarm()
{
	return _channelWarm;
}
uint16_t CctLamp::getChannelCool()
{
	return _channelCool;
}

uint8_t CctLamp::getIntensityWarm()
{
	return _intensityWarm;
}
uint8_t CctLamp::getIntensityCool()
{
	return _intensityCool;
}

uint8_t CctLamp::getCct()
{
	return _cct;
}

uint8_t CctLamp::calculateIntensityWarm()
{
	/* Calculate the Warm and Cool values based on the newly calculated cct and intensity values */
	uint8_t animatedIntensityWarm = 0;
	//uint8_t animatedIntensityCool = 0;
	
	/* first calculate based on the cct value */
	if( _cct <= 50 )
	{
	  //animatedIntensityCool  =  map( _cct, 0, 50, 0, 255 );
	  animatedIntensityWarm  =  255;
	}
	else
	{
	  animatedIntensityWarm  =  map( _cct, 50, 100, 255, 0);
	  //animatedIntensityCool  =  255;
	}
	
	/* second multiply with intensity to determine the overall level */
	animatedIntensityWarm  =  uint8_t ( (animatedIntensityWarm * _intensity) / 255 );
	//animatedIntensityCool  =  uint8_t ( (animatedIntensityCool * _intensity) / 255 );
	return animatedIntensityWarm;
}

uint8_t CctLamp::calculateIntensityCool()
{
	/* Calculate the Warm and Cool values based on the newly calculated cct and intensity values */
	//uint8_t animatedIntensityWarm = 0;
	uint8_t animatedIntensityCool = 0;
	
	/* first calculate based on the cct value */
	if( _cct <= 50 )
	{
	  animatedIntensityCool  =  map( _cct, 0, 50, 0, 255 );
	  //animatedIntensityWarm  =  255;
	}
	else
	{
	  //animatedIntensityWarm  =  map( _cct, 50, 100, 255, 0);
	  animatedIntensityCool  =  255;
	}
	
	/* second multiply with intensity to determine the overall level */
	//animatedIntensityWarm  =  uint8_t ( (animatedIntensityWarm * _intensity) / 255 );
	animatedIntensityCool  =  uint8_t ( (animatedIntensityCool * _intensity) / 255 );
	return animatedIntensityCool;
}