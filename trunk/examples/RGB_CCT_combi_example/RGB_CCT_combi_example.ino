/** Include LED library. ColorLamp and CctLamp are only required when used **/
#include <LED.h>
#include <ColorLamp.h>
#include <CctLamp.h>

/** Create the Lamps; set channel number and autoWrite to true if you wish to  **/
LED * ledLamp  =  new LED( 3 );

/** Use pin 5 for warm white, and pin 6 for cool white
    Set autoWrite to false if you do not connect your LEDs to arduino pins directly
    but use other channels (e.g. DMX, shift registers, TLC5940, etc. **/  
CctLamp * warmCoolLamp  =  new CctLamp( 5, 6, true ); 

/** Use channel 9 for red, 10 for green and 11 for blue
  AutoWrite is set to true. 
  Because our LED has a common Anode (common +) we set the commonAnode variable to true; this inverts the output values **/
ColorLamp * rgbLamp  =  new ColorLamp( 9, 10, 11, true, true );   

void setup()
{
  /** We turn on our LEDs initially **/
  ledLamp->intensityTo( 255 );
  rgbLamp->hsbTo( 255, 255, 255, 5000 );
  
  /** Set the animation type for our LED to QUADRATIC with easing in and/or out;
  The default animation is LINEAR without easing **/
  ledLamp->setAnimationType( QUADRATIC , true , false );  

  /** If you intend to animate only ColorTemperature, make sure the light is turned on so we can see what is happening **/
  warmCoolLamp->intensityTo( 255, 1000 ); 
}

void loop()
{
  /** Do some value manipulation **/
  doLEDAnimation();
  doRGBAnimation();
  doCCTAnimation();

  /** ALWAYS CALL THE UPDATE FUNCTIONS; if autoWrite is on, the Arduino will write the current intensity to the set channels 
  If the update function is not called, nothing will be calcualted or actuated
  **/
  ledLamp->update(); 
  warmCoolLamp->update();
  rgbLamp->update();  
}

void doLEDAnimation() 
{
  if( !ledLamp->isAnimating()) // Returns true if the LED is in an animation
  {
    if (ledLamp->getIntensity() == 0) // Returns cuurent intensity
    {
      ledLamp->intensityTo( 255, 10000 ); // sets the intensity to 255 in 10.000 milliseconds
    }
    else {
      ledLamp->intensityTo( 0, 10000 );
    }
  }
}

void doCCTAnimation()
{
  // Now we do something with the CCT Lamp
  if( !warmCoolLamp->isAnimating(CCT)) // Returns true if the LED is in a CCT animation
  {
    if (warmCoolLamp->getCct() == 0) // Returns the current CCT Value of the LED
    {
      warmCoolLamp->cctTo( 100, 2000 ); // Sets the desired CCT value and the time (in millis) it should take to get there
    }
    else 
    {
      warmCoolLamp->cctTo( 0, 2000 );
    }
  }
}

void doRGBAnimation()
{
  // We do something with the RGB DMX Lamp (ping-pong along the hue)
  if( !rgbLamp->isAnimating(HUE)) // Returns true if the LED is in an animation
  {
    if (rgbLamp->getHue() == 85) // Returns the current RGB Value of the LED (85 = green)
    {
      rgbLamp->hueTo( 213, 10000 ); // Sets the desired RGB value (213 = purple) and the time (in millis) it should take to get there
    }
    else {
      /** Set the HUE to 85 (green) and the time to get there to 10.000 millis. 
      By default the animation takes the shortest route (through RED) to get to green, but you can also
      take the long way around the color wheel by setting the 3rd parameter to false**/
      rgbLamp->hueTo( 85, 10000, false ); 
    }
  }
  /**
  You may also use these functions
  hsbTo( 255, 255, 255, 2000 ); // byte h, byte s, byte b, long duration
  rgbTo( 255, 255, 255, 2000 ); // byte r, byte g, byte b, long duration
  **/
  
  /**
  You may also use the SET functions; e.g.
  setHue( 255 ); // this will immediatily set the hue value and stop any animation
  setRGB( 255 , 255 , 255 );
  **/
}

