#include <LED.h>

/**
  // Set channel to the arduino pin to which the LED is connected, or use it for another channel (such as DMX Channel)
  LED * led1  =  new LED( uint32_t channel );
  // Set AutoWrite to TRUE if you use Arduino pins to directly control the LED value. The intensity will automatically be written to the ardunio pin ('channel') on update()
  LED * led1  =  new LED( uint32_t channel, bool autoWrite );
**/
LED * led1  =  new LED( 3, true ); 

// Or create an Array of LEDs
// LED * lamps[10]; 

void setup()
{
/** In case of an array, initialize it
  for (int i=0; i < 10; i++) {
      lamps[i] = new LED( );
  }
**/

/** Optionally, you may set the animation type for our LED to QUADRATIC with easing in and/or out;
    The default animation is LINEAR without easing **/
//   led1->setAnimationType( QUADRATIC , true , false );  
}

void loop()
{
  if( !led1->isAnimating()) // Returns true if the LED is in an animation
  {
    if (led1->getIntensity() == 0) // Returns the current intensity of the LED
    {
      led1->intensityTo( 255, 2000 ); // Sets the desired LED value and the time (in millis) it should take to get there
    }
    else {
      led1->intensityTo( 0, 2000 );
    }
  }
  // Always call the update function; if autoWrite is on, the Arduino will write the current intensity to the set channel 
  led1->update();
  /**
  // If autoWrite is OFF, manually set the desired value
  // In this case we write to arduino pins; but you can use this for instance to control DMX lighting
  analogWrite( led1->getChannel(), led1->getIntensity() );
  **/
}
