#include <ColorLamp.h>
#include <CctLamp.h>
#include <DmxSimple.h>   /** this required the DmxSimple Library: http://code.google.com/p/tinkerit/wiki/DmxSimple **/

CctLamp * warmCoolLamp  =  new CctLamp( 5, 6, true ); // Use pin 5 for warm white, and pin 6 for cool white
ColorLamp * rgbLamp  =  new ColorLamp( 1, false );   // Use channel 1 for red; chennel 2 and 3 are inferred from that, but can also be set manually

void setup()
{
  DmxSimple.usePin(11);  // Defines the pin used for DMX control; have a look at the DmxSimple Library for details
  DmxSimple.maxChannel(3);
  rgbLamp->hsbTo( 255, 255, 255, 2000 );
  warmCoolLamp->intensityTo( 255, 1000 ); // Make sure the light is turned on so we can see what is happening
}

void loop()
{
  // First we do something with the RGB DMX Lamp (ping-pong along the hue)
  if( !rgbLamp->isAnimating()) // Returns true if the LED is in an animation
  {
    if (rgbLamp->getHue() == 0) // Returns the current RGB Value of the LED
    {
      rgbLamp->hueTo( 255, 10000 ); // Sets the desired RGB value and the time (in millis) it should take to get there
    }
    else {
      rgbLamp->hueTo( 0, 10000 );
    }
  }
  /**
  You may also use these functions
  hsbTo( 255, 255, 255, 2000 ); // byte h, byte s, byte b, long duration
  rgbTo( 255, 255, 255, 2000 ); // byte r, byte g, byte b, long duration
  **/
  
  // Always call the update function; because autoWrite is off, for this lamp we write the values manually to DMX 
  rgbLamp->update();
  
  /** this required the DmxSimple Library: http://code.google.com/p/tinkerit/wiki/DmxSimple **/
  DmxSimple.write( rgbLamp->getChannelRed(),   rgbLamp->getRed() );
  DmxSimple.write( rgbLamp->getChannelGreen(), rgbLamp->getGreen() );
  DmxSimple.write( rgbLamp->getChannelBlue(),  rgbLamp->getBlue() );
  
  
  // Now we do something with the CCT Lamp
  if( !warmCoolLamp->isAnimating()) // Returns true if the LED is in an animation
  {
    if (warmCoolLamp->getCct() == 0) // Returns the current CCT Value of the LED
    {
      warmCoolLamp->cctTo( 100, 2000 ); // Sets the desired CCT value and the time (in millis) it should take to get there
    }
    else {
      warmCoolLamp->cctTo( 0, 2000 );
    }
  }
  // Always call the update function; if autoWrite is on, the Arduino will write the current intensity to the set channel 
  warmCoolLamp->update();
}
