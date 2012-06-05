/**
 * CctLamp.h
 * 
 * Copyright (c) 2011 
 
 * Remco Magielse & Serge Offermans
 
 * Intelligent Lighting Institute (ILI), TU/e.
 *
 * All rights reserved. LAST UPDATE: 30-05-2012
**/

#ifndef Cct_h
#define Cct_h

#include "LED.h"

class CctLamp:public LED
{
  public:
  	
    CctLamp();
	CctLamp( uint16_t channelWarm );
	CctLamp( uint16_t channelWarm , bool autoWrite );
    CctLamp( uint16_t channelWarm , uint16_t channelCool , bool autoWrite );
	CctLamp( uint16_t channelWarm , uint16_t channelCool , bool autoWrite , uint16_t x, uint16_t y );
    ~CctLamp();
    
	void update();
	void cctTo( uint8_t cct , uint32_t duration = DEFAULT_DURATION);
	void intensityTo( uint8_t to , uint32_t duration = DEFAULT_DURATION);
	void cctLampTo( uint8_t intens , uint8_t cct, uint32_t duration = DEFAULT_DURATION);
	
	uint8_t getCct();
    uint8_t getIntensityWarm();
	uint8_t getIntensityCool();
	
    uint16_t getChannelWarm();
	uint16_t getChannelCool();

  private:
	/* setCct is private because animation errors will occur 
	(also in intensity animations) if endCct is not used 
	to define the desired cct value */
    void setCct( uint8_t cct );
	
	/* setIntensityWarm/Cool is private to ensure that 
	actuation takes place */
	void setIntensityWarm( uint8_t intensityWarm );
	void setIntensityCool( uint8_t intensityCool );
	
	uint16_t _channelWarm; 
	uint16_t _channelCool; 
    uint8_t _cct;
		
	uint8_t _intensityWarm;
	uint8_t _intensityCool; 
	uint8_t _startCct;
	uint8_t _endCct;
	
	uint8_t calculateIntensityWarm();
	uint8_t calculateIntensityCool();
};

#endif

