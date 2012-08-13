#include <CctLamp.h>

/** Create a lamp object with the warm white color on channel 5
    The cool white channel is automatically set warmWhite + 1 (6 in this case)
    You may also set     
**/
CctLamp * lamp  =  new CctLamp( 5 ); 

/** Or create an Array of LEDs **/
// Cct * lamps[10]; 

void setup()
{
/** In case of an array, initialize it, remember to set the channels correctly **/
/**
  for (int i=0; i < 10; i++) {
      lamps[i] = new Cct( );
  }
**/
  lamp->intensityTo( 255, 1000 ); // make sure we can see the color temperature change
}

void loop()
{
  if( !lamp->isAnimating()) // Returns true if the LED is in an animation
  {
    if (lamp->getCct() == 0) // Returns the current CCT Value of the LED
    {
      lamp->cctTo( 100, 2000 ); // Sets the desired CCT value and the time (in millis) it should take to get there
    }
    else {
      lamp->cctTo( 0, 2000 );
    }
  }
  // Always call the update function; if autoWrite is on, the Arduino will write the current intensity to the set channel 
  lamp->update();
  
  /**
  // If autoWrite is OFF, manually set the desired value
  // In this case we write to arduino pins; but you can use this for instance to control DMX lighting
  analogWrite( lamp->getChannelWarm(), lamp->getIntensityWarm() );
  analogWrite( lamp->getChannelWarm(), lamp->getIntensityWarm() );
  **/
  
  /**
  // You can request the current values (also during animations) if you like
  Serial.print("CCT: "         + String( lamp->getCct() , DEC ) + "\t");
  Serial.println("Intensity: " + String( lamp->getIntensity() , DEC ) );
  delay(10);
  **/
}
