#include <Arduino.h>

// (1<<PB0) is pin 8
// (1<<PB1) is pin 9
// (1<<PB2) is pin 10
// (1<<PB3) is pin 11
// (1<<PB4) is pin 12
// (1<<PB5) is pin 13
// (1<<PD7) is pin 7
// (1<<PD6) is pin 6
// (1<<PD5) is pin 5
// (1<<PD4) is pin 4
// (1<<PD3) is pin 3
// (1<<PD2) is pin 2

// PORTB is PDB
// PORTD is PDD


#define LEDMASKRED (1<<PB0)
#define LEDMASKORANGE (1<<PB1)
#define LEDMASKGREEN (1<<PB2)
#define LEDMASKYELLOW (1<<PB3)
#define BUTTON 5
#define BUILTIN_LED PB5
#define BUILTINLEDMASK (1<<BUILTIN_LED)

unsigned long lastTime = 0;
unsigned long debounceDelay = 100;
unsigned long blinkinterval = 250;
unsigned long lastMillis = 0;

void setup()
{
  DDRB = DDRB | LEDMASKRED;
  DDRB = DDRB | LEDMASKORANGE;
  DDRB = DDRB | LEDMASKGREEN;
  DDRB = DDRB | BUILTINLEDMASK;
  DDRD = DDRD & ~BUILTINLEDMASK;
  Serial.begin(9600);
}

void builtinLed()
{
  //if button pressed turn on, otherwise it is off
  if (digitalRead(BUTTON) == HIGH)
  {
    PORTB = PORTB | BUILTINLEDMASK;
  }
  else
  {
    PORTB = PORTB & ~BUILTINLEDMASK;
  }
}

//blink at 4hz
void externalLed()
{
  if (millis() - lastMillis > blinkinterval)
  {
    PORTB = PORTB ^ LEDMASKRED;
    
    lastMillis = millis();
  }
}

void buttonhandler()
{
  if (digitalRead(BUTTON) == HIGH)
  {
    if (millis() - lastTime > debounceDelay)
    {
      PORTB = PORTB ^ LEDMASKRED;
      lastTime = millis();
    }
  }
}

void loop()
{
  externalLed();
  builtinLed();

}