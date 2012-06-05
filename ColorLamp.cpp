/**
 * ColorLamp.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 30-05-2012
**/
#include "ColorLamp.h"
#include "Calculate.h"

/** CONSTRUCTORS **/
ColorLamp::ColorLamp()
{
  _x          	=  	0;
  _y          	=  	0;
  _hue			= 	0;
  _saturation	= 	0;
  _intensity 	=  	0;
  _red			=	0;
  _green		=	0;
  _blue			=	0;
  _channel	 	=  	0;
  _channelRed 	=  	0;
  _channelGreen =  	1;
  _channelBlue =  	2;
  _on		 	=	true;
  _autoWrite	=	false;
  _isAnimating	=	false;
  _animType		= 	0;
  // Serial.println("EMPTY CONSTRUCTIR CALL");
}
ColorLamp::ColorLamp( uint16_t channelRed  )
{
  _x          	=  	0;
  _y          	=  	0;
  _hue			= 	0;
  _saturation	= 	0;
  _intensity 	=  	0;
  _red			=	0;
  _green		=	0;
  _blue			=	0;
  _channel	 	=  	channelRed;
  _channelRed 	=  	channelRed;
  _channelGreen =  	channelRed + 1;
  _channelBlue =  	channelRed + 2;
  _on		 	=	true;
  _autoWrite	=	false;
  _isAnimating	=	false;
  _animType		= 	0;
}
ColorLamp::ColorLamp( uint16_t channelRed, bool autoWrite )
{
  _x          	=  	0;
  _y          	=  	0;
  _hue			= 	0;
  _saturation	= 	0;
  _intensity 	=  	0;
  _red			=	0;
  _green		=	0;
  _blue			=	0;
  _channel	 	=  	channelRed;
  _channelRed 	=  	channelRed;
  _channelGreen =  	channelRed + 1;
  _channelBlue =  	channelRed + 2;
  _on		 	=	true;
  _autoWrite	=	autoWrite;
  _isAnimating	=	false;
  _animType		= 	0;
}
ColorLamp::ColorLamp( uint16_t channelRed, uint16_t channelGreen, uint16_t channelBlue, bool autoWrite )
{
  _x          	=  	0;
  _y          	=  	0;
  _hue			= 	0;
  _saturation	= 	0;
  _intensity 	=  	0;
  _red			=	0;
  _green		=	0;
  _blue			=	0;
  _channel	 	=  	channelRed;
  _channelRed 	=  	channelRed;
  _channelGreen =  	channelGreen;
  _channelBlue =  	channelBlue;
  _on		 	=	true;
  _autoWrite	=	autoWrite;
  _isAnimating	=	false;
  _animType		= 	0;
}
ColorLamp::ColorLamp( uint16_t channelRed, uint16_t channelGreen, uint16_t channelBlue, bool autoWrite, uint16_t x, uint16_t y )
{
  _x          	=  	x;
  _y          	=  	y;
  _hue			= 	0;
  _saturation	= 	0;
  _intensity 	=  	0;
  _red			=	0;
  _green		=	0;
  _blue			=	0;
  _channel	 	=  	channelRed;
  _channelRed 	=  	channelRed;
  _channelGreen =  	channelGreen;
  _channelBlue =  	channelBlue;
  _on		 	=	true;
  _autoWrite	=	autoWrite;
  _isAnimating	=	false;
  _animType		= 	0;
}

ColorLamp::~ColorLamp()
{
}

/** VOID FUNCTIONS **/

void ColorLamp::setRGB( uint8_t r, uint8_t g, uint8_t b )
{
	if ( _red != r || _green != g || _blue != b ) 
	{
		_red 	= constrain( r, 0, 255 );
		_green 	= constrain( g, 0, 255 );
		_blue 	= constrain( b, 0, 255 );
		_hasNewValue = true;
	}
}

void ColorLamp::setHSB( uint8_t h, uint8_t s, uint8_t b )
{
	_hue 		= constrain( h, 0, 255 );
	_saturation = constrain( s, 0, 255 );
	_intensity 	= constrain( b, 0, 255 );
}

void ColorLamp::setHue( uint8_t h )
{
	_hue 		= constrain( h, 0, 255 );
}

void ColorLamp::setSaturation( uint8_t s )
{
	_saturation = constrain( s, 0, 255 );
}

void ColorLamp::rgbTo( uint8_t rTo, uint8_t gTo, uint8_t bTo )
{
	rgbTo( rTo , gTo , bTo , DEFAULT_DURATION );
}

void ColorLamp::rgbTo( uint8_t rTo, uint8_t gTo, uint8_t bTo, uint32_t duration )
{
	uint8_t hsb[3] = { 0 , 0 , 0 };
	uint8_t * hsbArray = rgbToHsb(rTo, gTo, bTo, hsb );
	_isAnimating	=	true;
	_startTime		=	millis();
	_endTime		=	millis() + duration;
	_startHue		= 	_hue;
	_startSaturation= 	_saturation;
	_startIntensity	= 	_intensity;
	_endHue 		=	hsbArray[0];
	_endSaturation	=	hsbArray[1];
	_endIntensity	=	hsbArray[2];
	_animType		= 	0;
}

void ColorLamp::hsbTo( uint8_t hTo, uint8_t sTo, uint8_t bTo )
{
	hsbTo( hTo , sTo , bTo , DEFAULT_DURATION );
}

void ColorLamp::hsbTo( uint8_t hTo, uint8_t sTo, uint8_t bTo, uint32_t duration )
{
	_isAnimating	=	true;
	_startTime		=	millis();
	_endTime		=	millis() + duration;
	_startHue		= 	_hue;
	_startSaturation= 	_saturation;
	_startIntensity	= 	_intensity;
	_endHue 		=	hTo;
	_endSaturation	=	sTo;
	_endIntensity	=	bTo;
	_animType		= 	0;
}

void ColorLamp::hueTo( uint8_t hTo, uint32_t duration )
{
	_isAnimating	=	true;
	_startTime		=	millis();
	_endTime		=	millis() + duration;
	_startHue		= 	_hue;
	/* the _startValues for Brightness and Saturation 
	are	also updated to the current values to 
	make sure that an ongoing animation is not disturbed 
	*/
	_startSaturation= 	_saturation;
	_startIntensity	= 	_intensity;
	_endHue 		=	hTo;
	_animType		= 	0;
}

void ColorLamp::saturationTo( uint8_t sTo, uint32_t duration )
{
	_isAnimating	=	true;
	_startTime		=	millis();
	_endTime		=	millis() + duration;
	_startSaturation= 	_saturation;
	/* the _startValues for Hue and Brightness 
	are	also updated to the current values to 
	make sure that an ongoing animation is not disturbed 
	*/
	_startHue		= 	_hue;
	_startIntensity	= 	_intensity;
	_endSaturation 	=	sTo;
	_animType		= 	0;
}

void ColorLamp::intensityTo( uint8_t to, uint32_t duration )
{
	_isAnimating	=	true;
	_startTime		=	millis();
	_endTime		=	millis() + duration;
	_startIntensity		=	_intensity;
	/* the _startValues for Hue and Saturation 
	are	also updated to the current values to 
	make sure that an ongoing animation is not disturbed 
	*/
	_startHue		= 	_hue;
	_startSaturation= 	_saturation;
	_endIntensity	=	to;
	_animType		= 	0;
}

void ColorLamp::update()
{
	if( _isAnimating )
	{
		//Check if the animation is still going on
		if( millis() < _endTime )
		{
			uint32_t t	=	( millis() - _startTime );	// current time
			uint16_t bi	=	_startIntensity;					// begin Intensity
			uint16_t bh	=	_startHue;					// begin Hue
			uint16_t bs	=	_startSaturation;			// begin Saturation
			int      ci	=	( _endIntensity - _startIntensity );	// difference in intensity 	(may be negative, this results in problems in some _animTypes)
			int      ch	=	( _endHue - _startHue );	// difference in hue 		(may be negative, this results in problems in some _animTypes)
			int      cs	=	( _endSaturation - _startSaturation );	// difference in Saturation
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
					/* we are animating the intensity and/or rgb 
					change the values that are being animated */
					if (_endIntensity != _intensity) 	
					{ 	
						setIntensity(	Calculate.linear( t, bi, ci, d ) );
					}
					if (_endHue != _hue) 		// we are animating the CCT
					{ 			
						setHue(			Calculate.linear( t, bh, ch, d ) );
					}
					if (_endSaturation != _saturation) 		// we are animating the CCT
					{ 			
						setSaturation(			Calculate.linear( t, bs, cs, d ) );
					}
					break;
			}
			/*  Update the RGB values to reflect the changes for actuation */
			uint8_t rgb[3];
			uint8_t * rgbArray = hsbToRgb(_hue, _saturation, _intensity, rgb );
			setRGB(	rgbArray[0], rgbArray[1], rgbArray[2] );
		}
		
		else if( millis() >= _endTime)
		{
			setHSB( _endHue, _endSaturation, _endIntensity );
			/*  Update the RGB values to reflect the changes for actuation */
			uint8_t rgb[3];
			uint8_t * rgbArray = hsbToRgb(_hue, _saturation, _intensity, rgb );
			setRGB(	rgbArray[0], rgbArray[1], rgbArray[2] );
			_isAnimating = false;
		}
		
		if( _autoWrite )
		{
			analogWrite(_channelRed, _red);
			analogWrite(_channelGreen, _green);
			analogWrite(_channelBlue, _blue);
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
uint16_t ColorLamp::getChannelRed()
{
	return _channelRed;
}
uint16_t ColorLamp::getChannelGreen()
{
	return _channelGreen;
}
uint16_t ColorLamp::getChannelBlue()
{
	return _channelBlue;
}

uint8_t ColorLamp::getRed()
{
	return _red;
}
uint8_t ColorLamp::getGreen()
{
	return _green;
}
uint8_t ColorLamp::getBlue()
{
	return _blue;
}

uint8_t ColorLamp::getHue()
{
	return _hue;
}
uint8_t ColorLamp::getSaturation()
{
	return _saturation;
}
uint8_t ColorLamp::getBrightness()
{
	return _intensity;
}

/** This function is based on the one implemented 
by robert Atkins in the RGB Converter Library
https://github.com/ratkins/RGBConverter
*/
uint8_t * ColorLamp::rgbToHsb(uint8_t r, uint8_t g, uint8_t b, uint8_t hsb[] ) 
{
	float rd = (float) r/255;
    float gd = (float) g/255;
    float bd = (float) b/255;
    float maxVal = max(rd, max(gd, bd));
    float minVal = min(rd, min(gd, bd));
    float h = maxVal;
    float s = maxVal;
    float v = maxVal;

    float d = maxVal - minVal;
    s = maxVal == 0 ? 0 : d / maxVal;

    if (maxVal == minVal) { 
        h = 0; // achromatic
    } else {
        if (maxVal == rd) {
            h = (gd - bd) / d + (gd < bd ? 6 : 0);
        } else if (maxVal == gd) {
            h = (bd - rd) / d + 2;
        } else if (maxVal == bd) {
            h = (rd - gd) / d + 4;
        }
        h /= 6;
    }
    h *= 255;
    s *= 255;
    v *= 255;

    hsb[0] = uint8_t(h);
    hsb[1] = uint8_t(s);
    hsb[2] = uint8_t(v);
	return hsb;
}


/** This function is based on the one implemented 
by Elco Jacobs in the ShiftPWM Library
https://github.com/elcojacobs/ShiftPWM
*/
uint8_t * ColorLamp::hsbToRgb( uint8_t h, uint8_t s, uint8_t v, uint8_t rgb[] ) 
{	
	uint8_t r,g,b;
	uint16_t hue 	= map(h,0,255,0,359);
	uint16_t phase 	= hue/60;
	uint16_t bottom = ( (255 - s) * (float(v) / 255.0));
	uint16_t top 	= v;
	uint8_t rising  = ((top-bottom)  *(hue%60   )  )  /  60  +  bottom;
	uint8_t falling = ((top-bottom)  *(60-hue%60)  )  /  60  +  bottom;
	
	switch(phase) {
	case 0:
		r = top;
		g = rising;
		b = bottom;
		break;

	case 1:
		r = falling;
		g = top;
		b = bottom;
		break;

	case 2:
		r = bottom;
		g = top;
		b = rising;
		break;

	case 3:
		r = bottom;
		g = falling;
		b = top;
		break;

	case 4:
		r = rising;
		g = bottom;
		b = top;
		break;

	case 5:
		r = top;
		g = bottom;
		b = falling;
		break;
	}

    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
	return rgb;
}