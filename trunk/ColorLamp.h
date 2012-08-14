/*
 * ColorLamp.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 13-08-2012
*/

#ifndef RGB_h
#define RGB_h

#include "LED.h"

/** The ColorLamp class is used to manage RGB LEDs with 3 channels (R,G and B) **/
class ColorLamp:public LED
{
  public:
	ColorLamp();
	ColorLamp( uint16_t channelRed, uint16_t channelGreen, uint16_t channelBlue, bool autoWrite = true, bool commonAnode = false, uint16_t x = 0, uint16_t y = 0 );
    ~ColorLamp();
    
	void setChannel( uint16_t channelRed, uint16_t channelGreen = 0, uint16_t channelBlue = 0);
	
	void update();
    
	void rgbTo( uint8_t r, uint8_t g, uint8_t b, uint32_t duration = DEFAULT_DURATION);
	void hsbTo( uint8_t h, uint8_t s, uint8_t b, uint32_t duration = DEFAULT_DURATION, bool shortcutThroughZero = true);
	
	void hueTo( uint8_t hTo,  		uint32_t duration = DEFAULT_DURATION, bool shortcutThroughZero = true);
	void saturationTo( uint8_t sTo, uint32_t duration = DEFAULT_DURATION);
	
	void setRGB( uint8_t r, uint8_t g, uint8_t b, bool stopAnimation = true );
	void setHSB( uint8_t h, uint8_t s, uint8_t b, bool stopAnimation = true );
	
	void setHue( uint8_t h , bool stopAnimation = true);
	void setSaturation( uint8_t s , bool stopAnimation = true);
	
	/* Implemented, but not yet functioning properly. Conversion through HSB goes wrong
	void redTo(uint8_t r,  	uint32_t duration = DEFAULT_DURATION);
	void greenTo(uint8_t g, uint32_t duration = DEFAULT_DURATION);
	void blueTo(uint8_t b,  uint32_t duration = DEFAULT_DURATION);
	
	void setRed(uint8_t r, bool stopAnimation = true);
	void setGreen(uint8_t g, bool stopAnimation = true);
	void setBlue(uint8_t b, bool stopAnimation = true);
	*/
	
	void setAnimationType( uint8_t animType = LINEAR, bool easeIn = true, bool easeOut = true );
	void setCommonAnode( bool commonAnode = true);
	
	uint8_t getRed();
	uint8_t getGreen();
	uint8_t getBlue();
	
	uint8_t getHue();
	uint8_t getSaturation();
	uint8_t getBrightness();
	
    uint16_t getChannelRed();
	uint16_t getChannelGreen();
	uint16_t getChannelBlue();
	
	bool isAnimating();
	bool isAnimating(uint8_t param);

  private:
	uint16_t _channelRed; 
	uint16_t _channelGreen; 
    uint16_t _channelBlue; 
	
	uint8_t * rgbToHsb ( uint8_t r, uint8_t g, uint8_t b, uint8_t hsv[3] ); 
	uint8_t * hsbToRgb ( uint8_t h, uint8_t s, uint8_t b, uint8_t rgb[3] );
	
	uint8_t _red ;
	uint8_t _green;
	uint8_t _blue;
	
	uint8_t _hue;
	uint8_t _saturation;
	uint8_t _brightness;
	
	bool _commonAnode;
};

#endif

