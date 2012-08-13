#include <ColorLamp.h>

/** Create an RGB lamp on pins 9, 10 and 11. Unless specified otherwise (by adding a 'false' parameter) this will automatically write values to these arduino pins. **/
ColorLamp * lamp  =  new ColorLamp( 9, 10, 11 ); 

// Or create an Array of LEDs
// ColorLamp * lamps[10]; 

void setup()
{
/** In case of an array, initialize it
  for (int i=0; i < 10; i++) {
      lamps[i] = new ColorLamp( 2*i );
  }
**/
  lamp->intensityTo( 255, 1000 ); // Make sure the light is turned on so we can see what is happening
  lamp->saturationTo( 255, 1000 ); // make sure we can see Hue changes
}

void loop()
{
  if( !lamp->isAnimating()) // Returns true if the LED is in an animation
  {
    if (lamp->getHue() == 0) // Returns the current Hue Value of the LED
    {
      /** Sets the desired Hue value and the time (in millis) it should take to get there.
          We want to ping-pong through the color circle; therefore we set 'shortcutThroughZero' to false.
          If we would not do that, the shortest route from 0 to 255 and vice versa would be to hop directly to the other value
          which would leave the LED red...
      **/
      lamp->hueTo( 255, 2000, false ); 
    }
    else {
      lamp->hueTo( 0, 2000, false );
    }
  }
  /**
  You may also use these functions
  hsbTo( 255, 255, 255, 2000 ); // byte h, byte s, byte b, long duration
  rgbTo( 255, 255, 255, 2000 ); // byte r, byte g, byte b, long duration
  **/
  
  // Always call the update function; if autoWrite is on, the Arduino will write the current intensity to the set channel 
  lamp->update();
  
  /**
  // If autoWrite is OFF, manually set the desired value
  // In this case we write to arduino pins; but you can use this for instance to control DMX lighting
  analogWrite( lamp->getChannelRed(),   lamp->getRed() );
  analogWrite( lamp->getChannelGreen(), lamp->getGreen() );
  analogWrite( lamp->getChannelBlue(),  lamp->getBlue() );
  **/
  
  /**
  // You can request the current values (also during animations) if you like
  Serial.print("Hue: "          + String( lamp->getHue() , DEC ) + "\t");
  Serial.print("Saturation: "   + String( lamp->getSaturation() , DEC ) + "\t");
  Serial.println("Brightness: " + String( lamp->getIntensity() , DEC ));

  Serial.print("r: "   + String( lamp->getRed() , DEC ) + "\t");
  Serial.print("g: "   + String( lamp->getGreen() , DEC ) + "\t");
  Serial.println("b: " + String( lamp->getBlue() , DEC ));
  delay(40);
  **/
}
