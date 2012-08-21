/*
 * CctLamp.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 13-08-2012
*/
#include "CctLamp.h"
#include "Calculate.h"

/** cctAnim is the object that is used to manage the color temperature animations **/
Animation * cctAnim;

/* CONSTRUCTORS */

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
  
  intensityAnim  =  new Animation();  
  cctAnim  =  new Animation();
  
  if ( _autoWrite )
  {
	pinMode( _channelWarm, OUTPUT );
	pinMode( _channelCool, OUTPUT );
  }
  
  _hasNewValue = true;
  update();
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

  if ( _autoWrite )
  {
	pinMode( _channelWarm, OUTPUT );
	pinMode( _channelCool, OUTPUT );
  }
  
  _hasNewValue = true;
  update();
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

  if ( _autoWrite )
  {
	pinMode( _channelWarm, OUTPUT );
	pinMode( _channelCool, OUTPUT );
  }
  
  _hasNewValue = true;
  update();
}

CctLamp::~CctLamp()
{
}

/* VOID FUNCTIONS */

/** Immediatily sets the color temperature of the lamp (range 0 - 100). An ongoing cct animation is stopped unless stopAnimation is set to false.
	Make sure to also set the intensity to a value that is greater than 0, 
	otherwise you won't see the effect of the color temperature change.
 **/
void CctLamp::setCct( uint8_t cct, bool stopAnimation )
{
	if( cct != _cct )
	{
		_cct = constrain( cct, 0, 100 );
		_hasNewValue = true;
		if (stopAnimation)
		{
			cctAnim->stopAnimation();
		}
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

/** Animates the color temperature of the lamp (range 0 - 100) with its duration in millis.
	Make sure to also set the intensity to a value that is greater than 0, 
	otherwise you won't see the effect of the color temperature change.
 **/
void CctLamp::cctTo( uint8_t cct, uint32_t duration )
{
	cctAnim->startAnimation( _cct, constrain(cct, 0, 100), duration );
}

/** Animates CCT Lamp with all its values; 
intensity, cct value and duration in millis **/
void CctLamp::cctLampTo( uint8_t intens, uint8_t cct, uint32_t duration )
{	
	intensityAnim->startAnimation( _intensity, constrain(intens, 0, 255), duration );
	cctAnim->startAnimation( _cct, constrain(cct, 0, 100), duration );
}

/** Sets the animation type for a CCT Lamp. The available animation types are LINEAR (no easing) and QUADRATIC. 
	QUADRATIC animations allow independant easing in and out **/
void CctLamp::setAnimationType( uint8_t animType, bool easeIn, bool easeOut )
{
	intensityAnim->setAnimationType( animType, easeIn, easeOut );
	cctAnim->setAnimationType( animType, easeIn, easeOut );
}

/** Update Function; should be called every loop to set, animate and actuate the lamps **/
void CctLamp::update()
{
	if( intensityAnim->isAnimating() )
	{
		setIntensity( intensityAnim->getValue(), false );
	}
	
	if( cctAnim->isAnimating() )
	{
		setCct( cctAnim->getValue(), false );
	}
	/* If there is no animation going on and the device is turned off
		and the intensity is not set to 0; turn the LED off
	*/
	if( !intensityAnim->isAnimating() && !cctAnim->isAnimating() && !_on && _intensity > 0 )
	{
		setIntensity( 0, false );
	}
	
	if ( _hasNewValue) 
	{
		/* Here we convert CCT and Intensity values to Warm and Cool white values so they can be actuated */
		setIntensityWarm( calculateIntensityWarm() );
		setIntensityCool( calculateIntensityCool() );
		
		/* Here we actually actuate the light if autowrite is on */
		if( _autoWrite )
		{
			analogWrite(_channelWarm, _intensityWarm);
			analogWrite(_channelCool, _intensityCool);
			_hasNewValue = false;
		}
	}
	
}

/* INTEGER FUNCTIONS */

/** Returns the channel to which the warm LED should be connected 
**/
uint16_t CctLamp::getChannelWarm()
{
	return _channelWarm;
}

/** Returns the channel to which the cool LED should be connected **/
uint16_t CctLamp::getChannelCool()
{
	return _channelCool;
}

/** Returns the current intensity of the warm LED **/
uint8_t CctLamp::getIntensityWarm()
{
	return _intensityWarm;
}

/** Returns the current intensity of the cool LED **/
uint8_t CctLamp::getIntensityCool()
{
	return _intensityCool;
}

/** Returns the current color temperature of the LED (range 0 - 100) **/
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

/* BOOL FUNCTIONS */

/** Returns true if the lamp is animating either intensity of cct **/
bool CctLamp::isAnimating()
{
	if ( !intensityAnim->isAnimating() && !cctAnim->isAnimating() )
	{
		return false;
	}
	else 
	{
		return true;
	}
}

/** Returns true if the supplied parameter (CCT or INTENSITY) is animating **/
bool CctLamp::isAnimating(uint8_t param)
{
	switch ( param )
	{
		case PARAM_INTENSITY:
			return intensityAnim->isAnimating();
		break;
		case PARAM_CCT:
			return cctAnim->isAnimating();
		break;
		default:
			return false;
		break;
	}
}