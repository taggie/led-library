/*
 * ColorLamp.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 13-08-2012
*/

#include "ColorLamp.h"

/** hueAnim is the object that is used to manage the hue animations **/
Animation * hueAnim;

/** saturationAnim is the object that is used to manage the saturation animations **/
Animation * saturationAnim;

/* CONSTRUCTORS */

/** Empty ColorLamp Constructor. If you use this, make sure to set the channel manually afterwards using setChannel(); 
	this automatically assumes you use autoWrite (write to the arduino pins immediatily) and will set the pinMode for this channel to OUTPUT. 
	If you use another method (e.g. DMX), use the full constructor and set autoWrite to false **/
 ColorLamp::ColorLamp( )
{
  _x          	=  	0;
  _y          	=  	0;
  _hue			= 	0;
  _saturation	= 	0;
  _intensity 	=  	0;
  _red			=	0;
  _green		=	0;
  _blue			=	0;
  _channel	 	=  	1;
  _channelRed 	=  	1;
  _channelGreen =  	_channelRed + 1;
  _channelBlue 	=  	_channelRed + 2;
  _on		 	=	true;
  _autoWrite	=	true;
  _commonAnode 	= 	false;
  hueAnim  		=  	new Animation();
  saturationAnim  = new Animation();
}

/** Colorlamp Constructor. The first three parameters set channels for Red, Green and Blue. 
	If autowrite is set to true; the arduino will write the output values to the arduino pins.
	Set autoWrite to false if you do not connect your LEDs to arduino pins directly
    but use other channels (e.g. DMX, shift registers, TLC5940, etc.
	If you have LEDs with a common anode (common +), you set that to true in order to invert the output values.
 **/

ColorLamp::ColorLamp( uint16_t channelRed, uint16_t channelGreen, uint16_t channelBlue, bool autoWrite, bool commonAnode, uint16_t x, uint16_t y )
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
  _channelGreen   =  	channelGreen;
  _channelBlue 	=  	channelBlue;
  _on		 	=	true;
  _autoWrite	=	autoWrite;
  _commonAnode 	= 	commonAnode;
  hueAnim  		=  	new Animation();
  saturationAnim  = new Animation();
  
  if ( _autoWrite )
  {
	pinMode( _channelRed, OUTPUT );
	pinMode( _channelGreen, OUTPUT );
	pinMode( _channelBlue, OUTPUT );
  }
  _hasNewValue = true;
  update();
}

/** Default Destructor **/
ColorLamp::~ColorLamp()
{
}

/* VOID FUNCTIONS */

/** Allows you to set the channel of the LED. if autoWrite is on, this is the Arduino pin that is actuated
	If only one argument is supplied, the green and blue channel are the subsequent channel numbers
**/
void ColorLamp::setChannel( uint16_t channelRed, uint16_t channelGreen, uint16_t channelBlue )
{
	if (channelGreen == 0 && channelBlue == 0) // This is the case if the user does not supply arguments for green and blue
	{
		channelGreen = channelRed + 1;
		channelBlue = channelRed + 2;
	}
	_channel	=	channelRed;
	_channelRed 	=  	channelRed;
	_channelGreen   =  	channelGreen;
	_channelBlue 	=  	channelBlue;

	if ( _autoWrite )
	{
		pinMode( _channelRed, OUTPUT );
		pinMode( _channelGreen, OUTPUT );
		pinMode( _channelBlue, OUTPUT );
	}
}

/** This function immidatily sets the RED, GREEN and BLUE to the specified values. 
	Values are changed immediatily, but only actuated in the update() function.
	When setting RGB; the values are first converted to HSB,
	Conversion back to RGB happens during the update() function.
	Any ongoing animations are stopped unless 'stopanimation' is set to false.
**/
void ColorLamp::setRGB( uint8_t r, uint8_t g, uint8_t b, bool stopAnimation )
{
	if ( _red != r || _green != g || _blue != b ) 
	{	
		uint8_t hsb[3] = { 0 , 0 , 0 };
		uint8_t * hsbArray = rgbToHsb(r, g, b, hsb );
		
		setHSB(hsbArray[0], hsbArray[1], hsbArray[2], stopAnimation);
	}
}

/*
void ColorLamp::setRed( uint8_t r, bool stopAnimation )
{
	uint8_t hsb[3] = { 0 , 0 , 0 };
	uint8_t * hsbArray = rgbToHsb(r, _green, _blue, hsb );
	
	setHSB(hsbArray[0], hsbArray[1], hsbArray[2], stopAnimation);
}

void ColorLamp::setGreen( uint8_t g, bool stopAnimation )
{
	uint8_t hsb[3] = { 0 , 0 , 0 };
	uint8_t * hsbArray = rgbToHsb(_red, g, _blue, hsb );
	
	setHSB(hsbArray[0], hsbArray[1], hsbArray[2], stopAnimation);
}

void ColorLamp::setBlue( uint8_t b, bool stopAnimation )
{
	uint8_t hsb[3] = { 0 , 0 , 0 };
	uint8_t * hsbArray = rgbToHsb(_red, _green, b, hsb );
	
	setHSB(hsbArray[0], hsbArray[1], hsbArray[2], stopAnimation);
} */

/** This function immidatily sets the hue, saturation and intensity to the specified values.
	Values are changed immediatily, but only actuated in the update() function. 
	Any ongoing animations are stopped unless 'stopAnimation' is set to false.
**/
void ColorLamp::setHSB( uint8_t h, uint8_t s, uint8_t b, bool stopAnimation )
{
	if ( _hue != h || _saturation != s || _intensity != b ) 
	{
		_hue 		= constrain( h, 0, 255 );
		_saturation = constrain( s, 0, 255 );
		_intensity 	= constrain( b, 0, 255 );
		_hasNewValue	= true;
		if (stopAnimation)
		{
			intensityAnim->stopAnimation();
			hueAnim->stopAnimation();
			saturationAnim->stopAnimation();
		}
	}
}

/** This function immidatily sets the Hue to the specified value and
	Values are changed immediatily, but only actuated in the update() function.
	maintains the other current variables. Any ongoing hue animation is stopped unless 'stopAnimation' is set to false.
**/
void ColorLamp::setHue( uint8_t h, bool stopAnimation )
{
	if( h != _hue )
	{
		_hue 			= constrain( h, 0, 255 );
		_hasNewValue	= true;
		if (stopAnimation)
		{
			hueAnim->stopAnimation();
		}
	}
}

/** This function immidatily sets the saturation to the specified value and
	maintains the other current variables. Any ongoing saturation animation is stopped unless 'stopAnimation' is set to false.
	Values are changed immediatily, but only actuated in the update() function.
**/
void ColorLamp::setSaturation( uint8_t s, bool stopAnimation )
{
	if( s != _saturation )
	{
		_saturation = constrain( s, 0, 255 );
		_hasNewValue	= true;
		if (stopAnimation)
		{
			saturationAnim->stopAnimation();
		}
	}
	
}

/** This function allows an animation in RGB terms.
	When animating RGB; the values are first converted to HSB
	Conversion back to RGB happens during the update function.
**/
void ColorLamp::rgbTo( uint8_t rTo, uint8_t gTo, uint8_t bTo, uint32_t duration )
{
	uint8_t hsb[3] = { 0 , 0 , 0 };
	uint8_t * hsbArray = rgbToHsb(rTo, gTo, bTo, hsb );
	
	hsbTo(hsbArray[0], hsbArray[1], hsbArray[2], duration);
}

/*
void ColorLamp::redTo( uint8_t rTo, uint32_t duration )
{
	uint8_t hsb[3] = { 0 , 0 , 0 };
	uint8_t * hsbArray = rgbToHsb(rTo, _green, _blue, hsb );
	
	hsbTo(hsbArray[0], hsbArray[1], hsbArray[2], duration);
}

void ColorLamp::greenTo( uint8_t gTo, uint32_t duration )
{
	uint8_t hsb[3] = { 0 , 0 , 0 };
	uint8_t * hsbArray = rgbToHsb(_red, gTo, _blue, hsb );
	
	hsbTo(hsbArray[0], hsbArray[1], hsbArray[2], duration);
}

void ColorLamp::blueTo( uint8_t bTo, uint32_t duration )
{
	uint8_t hsb[3] = { 0 , 0 , 0 };
	uint8_t * hsbArray = rgbToHsb(_red, _green, bTo, hsb );
	
	hsbTo(hsbArray[0], hsbArray[1], hsbArray[2], duration);
} */

/** This function is used for animating to a desired HSB value. The duration of the animation can be set in millis().
	shortcutThroughZero allows use of the hue color wheel as expected. If the shortest route from start value to end value goes through zero (red)
	we will take this route by default. Set it to false if you wish to take the longer route.
	**/
void ColorLamp::hsbTo( uint8_t hTo, uint8_t sTo, uint8_t bTo, uint32_t duration, bool shortcutThroughZero )
{	
	hueAnim->startAnimation( _hue, constrain(hTo, 0, 255), duration, shortcutThroughZero );
	saturationAnim->startAnimation( _saturation, constrain(sTo, 0, 255), duration );
	intensityAnim->startAnimation( _intensity, constrain(bTo, 0, 255), duration );
}

/** This function is used for animating to a desired HUE value while maintaining Saturation and Brightness values.
	The duration of the animation can be set in millis().
	shortcutThroughZero allows use of the hue color wheel as expected. If the shortest route from start value to end value goes through zero (red)
	we will take this route by default. Set it to false if you wish to take the longer route.
	**/
void ColorLamp::hueTo( uint8_t hTo, uint32_t duration, bool shortcutThroughZero )
{
	hueAnim->startAnimation( _hue, constrain(hTo, 0, 255), duration, shortcutThroughZero );
}

/** This function is used for animating to a desired Saturation value while maintaining Hue and Brightness values.
	The duration of the animation can be set in millis().
	**/
void ColorLamp::saturationTo( uint8_t sTo, uint32_t duration )
{
	saturationAnim->startAnimation( _saturation, constrain(sTo, 0, 255), duration );
}

/** Tells the program you are using LEDs with a common anode (common +), if set to true (default = false).
	This will invert the output values. If autoWrite == true, the arduino will make the ouput pins LOW in order to 
	turn the LEDs on. If autoWrite == false, getRed(), getGreen() and getBlue() will return the inverted value (0 if it is full on). 
 **/
void ColorLamp::setCommonAnode( bool commonAnode )
{
	_commonAnode = commonAnode;
}

/** Sets the animation type for a colorLamp. The available animation types are LINEAR (no easing) and QUADRATIC. 
	QUADRATIC animations allow independant easing in and out **/
void ColorLamp::setAnimationType( uint8_t animType, bool easeIn, bool easeOut )
{
	intensityAnim->setAnimationType( animType, easeIn, easeOut );
	hueAnim->setAnimationType( animType, easeIn, easeOut );
	saturationAnim->setAnimationType( animType, easeIn, easeOut );
}

/** Update Function; should be called every loop to set, animate and actuate the lamps **/
void ColorLamp::update()
{
	if( hueAnim->isAnimating() )
	{
		setHue( hueAnim->getValue(), false );
	}
	if( saturationAnim->isAnimating() )
	{
		setSaturation( saturationAnim->getValue(), false );
	}
	if( intensityAnim->isAnimating() )
	{
		setIntensity( intensityAnim->getValue(), false );
	}
	
	/* If there is no animation going on and the device is turned off,
	   and the intensity is not set to 0, turn the LED off */
	if( !intensityAnim->isAnimating() && !hueAnim->isAnimating() && !saturationAnim->isAnimating() && !_on && _intensity > 0 )
	{
		setIntensity( 0, false );
	}
	
	if ( _hasNewValue )
	{
		/*  Update the RGB values based on HSB to reflect the changes for actuation 
			This overwrites the current RGB values, even if they were recently set.
			However, this is not a problem because if an RGB value is set; 
			it is also converted to matching HSB values that are thus up-to-date.
		*/
		uint8_t rgb[3];
		uint8_t * rgbArray = hsbToRgb(_hue, _saturation, _intensity, rgb );
		
		_red 	= constrain( rgbArray[0], 0, 255 );
		_green 	= constrain( rgbArray[1], 0, 255 );
		_blue 	= constrain( rgbArray[2], 0, 255 );
		
		if( _autoWrite )
		{
			if ( !_commonAnode ) 
			{
				analogWrite(_channelRed, _red);
				analogWrite(_channelGreen, _green);
				analogWrite(_channelBlue, _blue);
			}
			else 
			{
				analogWrite(_channelRed, 255-_red);
				analogWrite(_channelGreen, 255-_green);
				analogWrite(_channelBlue, 255-_blue);
			}
			_hasNewValue = false;
		}
	}
}

/* INTEGER FUNCTIONS */

/** Returns the set channel for the RED LED **/
uint16_t ColorLamp::getChannelRed()
{
	return _channelRed;
}
/** Returns the set channel for the GREEN LED **/
uint16_t ColorLamp::getChannelGreen()
{
	return _channelGreen;
}
/** Returns the set channel for the BLUE LED **/
uint16_t ColorLamp::getChannelBlue()
{
	return _channelBlue;
}

/** This function returns the current RED output value. Be careful that they are inverted if commonAnode is set to true! **/
uint8_t ColorLamp::getRed()
{
	if ( !_commonAnode )
	{	
		return _red;
	}
	else 
	{
		return 255-_red;
	}
}

/** This function returns the current GREEN output value. Be careful that they are inverted if commonAnode is set to true! **/
uint8_t ColorLamp::getGreen()
{
	if ( !_commonAnode )
	{	
		return _green;
	}
	else 
	{
		return 255-_green;
	}
}

/** This function returns the current GREEN output value. Be careful that they are inverted if commonAnode is set to true! **/
uint8_t ColorLamp::getBlue()
{
	if ( !_commonAnode )
	{	
		return _blue;
	}
	else 
	{
		return 255-_blue;
	}
}

/** This function returns the current HUE output value. **/
uint8_t ColorLamp::getHue()
{
	return _hue;
}

/** This function returns the current SATURATION output value. **/
uint8_t ColorLamp::getSaturation()
{
	return _saturation;
}

/** This function returns the current INTENSITY output value. **/
uint8_t ColorLamp::getBrightness()
{
	return _intensity;
}

/** This function converts RGB to HSB and is based on the one implemented 
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


/** This function converts RGB to HSB and is based on the one implemented 
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

/* BOOL FUNCTIONS */

/** This function returns true if any animation (either HUE, SATURATION, BRIGHTNESS) is ongoing in this ColorLamp **/ 
bool ColorLamp::isAnimating()
{
	if ( !intensityAnim->isAnimating() && !hueAnim->isAnimating() && !saturationAnim->isAnimating() )
	{
		return false;
	}
	else 
	{
		return true;
	}
}

/** Returns true if the supplied parameter (HUE, SATURATION, BRIGHTNESS) is animating **/
bool ColorLamp::isAnimating(uint8_t param)
{
	switch ( param )
	{
		case PARAM_INTENSITY:
			return intensityAnim->isAnimating();
		break;
		case PARAM_HUE:
			return hueAnim->isAnimating();
		break;
		case PARAM_SATURATION:
			return saturationAnim->isAnimating();
		break;
	}
}