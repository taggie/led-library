/**
 * ColorLamp.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 30-05-2012
**/

#ifndef RGB_h
#define RGB_h

#include "LED.h"

class ColorLamp:public LED
{
  public:
    ColorLamp();
	ColorLamp( uint16_t channelRed );
	ColorLamp( uint16_t channelRed , bool autoWrite );
    ColorLamp( uint16_t channelRed , uint16_t channelGreen , uint16_t channelBlue , bool autoWrite );
	ColorLamp( uint16_t channelRed , uint16_t channelGreen , uint16_t channelBlue , bool autoWrite , uint16_t x, uint16_t y );
    ~ColorLamp();
    
	void update();
    void rgbTo( uint8_t r, uint8_t g, uint8_t b );
	void rgbTo( uint8_t r, uint8_t g, uint8_t b, uint32_t duration);
	void hsbTo( uint8_t h, uint8_t s, uint8_t b );
	void hsbTo( uint8_t h, uint8_t s, uint8_t b , uint32_t duration);
	void hueTo( uint8_t hTo,  		uint32_t duration = DEFAULT_DURATION);
	void saturationTo( uint8_t sTo, uint32_t duration = DEFAULT_DURATION);
	void intensityTo(uint8_t to, 	uint32_t duration = DEFAULT_DURATION);
	
	uint8_t getRed();
	uint8_t getGreen();
	uint8_t getBlue();
	
	uint8_t getHue();
	uint8_t getSaturation();
	uint8_t getBrightness();
	
    uint16_t getChannelRed();
	uint16_t getChannelGreen();
	uint16_t getChannelBlue();

  private:
	/* setRGB is private because animation errors will occur 
	(also in intensity animations) if endRGB is not used 
	to define the desired rgb value; use rgbTo() instead */
    void setRGB( uint8_t r, uint8_t g, uint8_t b );
	void setHSB( uint8_t h, uint8_t s, uint8_t b );
	
	void setHue( uint8_t h );
	void setSaturation( uint8_t s );
	
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
	
	uint8_t _startHue;
	uint8_t _endHue;
	uint8_t _startSaturation;
	uint8_t _endSaturation;
};

#endif

