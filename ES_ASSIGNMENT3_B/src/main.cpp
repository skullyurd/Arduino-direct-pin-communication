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

#define buttonMask1 (1<<PD3)
#define buttonMask2 (1<<PD4)

#define LEDMASKRED (1<<PB0)
#define LEDMASKORANGE (1<<PB1)
#define LEDMASKGREEN (1<<PB2)
#define LEDMASKYELLOW (1<<PB3)
#define LEDMASKBLUE (1<<PB4)

#define BUILTIN_LED PB5
#define BUILTINLEDMASK (1<<BUILTIN_LED)

unsigned long currentMillis = millis();
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long interval = 50;
unsigned long intervalblink4persecond = 250;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;

void setup()
{
  DDRB = DDRB | LEDMASKRED | LEDMASKORANGE | LEDMASKGREEN | LEDMASKYELLOW | BUILTINLEDMASK;
  DDRD = DDRD & ~buttonMask1 & ~buttonMask2;
  PORTD = PORTD | buttonMask1 | buttonMask2;

  Serial.begin(9600);
}

void buttonhandler()
{
  int reading = PIND & buttonMask1;
  if (reading == 0)
  {
    if (lastDebounceTime == 0)
    {
      lastDebounceTime = millis();
    }
    else
    {
      if ((millis() - lastDebounceTime) > debounceDelay)
      {
        Serial.println("button pressed");
        lastDebounceTime = 0;
      }
    }
  }
}

void buttonhandler2()
{
  int reading = PIND & buttonMask2;
  if (reading == 0)
  {
    if (lastDebounceTime == 0)
    {
      lastDebounceTime = millis();
    }
    else
    {
      if ((millis() - lastDebounceTime) > debounceDelay)
      {
        Serial.println("button pressed");
        lastDebounceTime = 0;
      }
    }
  }
}

void blinkInternalLed()
{
  int reading = PIND & buttonMask2;
  if (reading == 0)
  {
    PORTB = PORTB ^ BUILTINLEDMASK;
  }
}

void blinkBlueLed()
{
  currentMillis = millis();
  if (currentMillis - previousMillis2 >= intervalblink4persecond)
  {
    previousMillis2 = currentMillis;
    static int state = 0;
    switch(state) {
      case 0:
        PORTB = PORTB | LEDMASKBLUE;
        break;
      case 1:
        PORTB = PORTB & ~LEDMASKBLUE;
        break;
    }
    state++;
    if (state == 2) {
      state = 0;
    }
  }
}

void sweep1()
{
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    static int state = 0;
    switch(state) {
      case 0:
        PORTB = PORTB | LEDMASKRED;
        break;
      case 1:
        PORTB = PORTB & ~LEDMASKRED;
        PORTB = PORTB | LEDMASKORANGE;
        break;
      case 2:
        PORTB = PORTB & ~LEDMASKORANGE;
        PORTB = PORTB | LEDMASKGREEN;
        break;
      case 3:
        PORTB = PORTB & ~LEDMASKGREEN;
        PORTB = PORTB | LEDMASKYELLOW;
        break;
      case 4:
        PORTB = PORTB & ~LEDMASKYELLOW;
        PORTB = PORTB | LEDMASKGREEN;
        break;
      case 5:
        PORTB = PORTB & ~LEDMASKGREEN;
        PORTB = PORTB | LEDMASKORANGE;
        break;
      case 6:
        PORTB = PORTB & ~LEDMASKORANGE;
        PORTB = PORTB | LEDMASKRED;
        break;
    }
    state++;
    if (state == 7) {
      state = 0;
    }
  }
}

void sweep2()
{
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    static int state = 0;
    switch(state) {
      case 0:
        PORTB = PORTB | LEDMASKRED;
        break;
      case 1:
        PORTB = PORTB | LEDMASKORANGE;
        break;
      case 2:
        PORTB = PORTB | LEDMASKGREEN;
        break;
      case 3:
        PORTB = PORTB | LEDMASKYELLOW;
        break;
      case 4:
        PORTB = PORTB & ~LEDMASKRED;
        break;
      case 5:
        PORTB = PORTB & ~LEDMASKORANGE;
        break;
      case 6:
        PORTB = PORTB & ~LEDMASKGREEN;
        break;
      case 7:
        PORTB = PORTB | LEDMASKGREEN;
        break;
      case 8:
        PORTB = PORTB | LEDMASKORANGE;
        break;
      case 9:
        PORTB = PORTB | LEDMASKRED;
        break;
      case 10:
        PORTB = PORTB & ~LEDMASKYELLOW;
        break;
      case 11:
        PORTB = PORTB & ~LEDMASKGREEN;
        break;
      case 12:
        PORTB = PORTB & ~LEDMASKORANGE;
        break;
    }
    state++;
    if (state == 13) {
      state = 0;
    }
  }
}

void sweepLoop()
{
  int reading = PIND & buttonMask1;
  if (reading == 0)
  {
    sweep2();
  }
  else
  {
    sweep1();
  }
}


void loop()
{
  sweepLoop();
  blinkInternalLed();
  blinkBlueLed();
}