/*
 * CctLamp.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 13-08-2012
*/

#ifndef Cct_h
#define Cct_h

#include "LED.h"

class CctLamp:public LED
{
  public:
	CctLamp( uint16_t channelWarm = 1 , bool autoWrite = true );
    CctLamp( uint16_t channelWarm , uint16_t channelCool , bool autoWrite );
	CctLamp( uint16_t channelWarm , uint16_t channelCool , bool autoWrite , uint16_t x, uint16_t y );
    ~CctLamp();
    
	void update();
	void cctTo( uint8_t cct , uint32_t duration = DEFAULT_DURATION);
	void cctLampTo( uint8_t intens , uint8_t cct, uint32_t duration = DEFAULT_DURATION);
    void setCct( uint8_t cct, bool stopAnimation = true );
	// Possibly include void setCctLamp( uint8_t intens , uint8_t cct );
	void setAnimationType( uint8_t animType = LINEAR, bool easeIn = true, bool easeOut = true );
	
	uint8_t getCct();
    uint8_t getIntensityWarm();
	uint8_t getIntensityCool();
	
    uint16_t getChannelWarm();
	uint16_t getChannelCool();
	
	bool isAnimating();
	bool isAnimating(uint8_t param);

  private:
	void setIntensityWarm( uint8_t intensityWarm );
	void setIntensityCool( uint8_t intensityCool );
	
	uint16_t _channelWarm; 
	uint16_t _channelCool; 
    uint8_t _cct;
	
	uint8_t _intensityWarm;
	uint8_t _intensityCool; 
	
	uint8_t calculateIntensityWarm();
	uint8_t calculateIntensityCool();
};

#endif

