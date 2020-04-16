
/**
 * Capacitive touch example, adapted by Eva Snyder from
 * http://www.instructables.com/id/Turn-a-pencil-drawing-into-a-capacitive-sensor-for/
 **/
 
int LEDPin = 13;
int capSensePin = 2;
int touchedCutoff = 100;
int onStart = 0;
int onDuration = 0;
int onMax = 5000;
int waitTime = 500;
bool lightsAreOn = false;

void setup(){
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
}


void loop()
{
  if (lightsAreOn)
  {
    if (touched())
    {
      turnOff(true);
    }
    else
    {
      onDuration = millis() - onStart;
      if (onDuration > onMax)
      {
        turnOff(false);
      }
    }
  }
  else
  {
    if (touched())
    {
      turnOn();
    }
  }
}

void turnOff(bool needsWait)
{
  digitalWrite(LEDPin, LOW);
  lightsAreOn = false;
  if (needsWait)
  {
    delay(waitTime);
  }
}

void turnOn()
{
  digitalWrite(LEDPin, HIGH);
  lightsAreOn = true;
  onStart = millis();
  delay(waitTime);
}

bool touched()
{
  int reading = readCapacitivePin(capSensePin);
  //Serial.println(reading);
  if (reading > touchedCutoff)
  {
    return true;
  }
  return false;
}




//
//void loop(){
//
//  
//  if (millis() - lastTouch > touchPause) {  // ok to listen.
//    
//    if (lightsAreOn) {
//      if (millis() - lastTouch > onMax) {
//        turnOff();
//      }
//    } else {
//      if (readCapacitivePin(capSensePin) > touchedCutoff) {
//        touched();
//      }
//    }
//  }
//
//
//  
//}
//
//void touched(){
//  if (lightsAreOn) {
//    turnOff();
//  } else {
//    turnOn();
//  }
//}
//
//void turnOn(){
//  digitalWrite(LEDPin, HIGH);
//  lastTouch = millis();
//  lightsAreOn = true;
//}
//
//void turnOff(){
//  digitalWrite(LEDPin, LOW);
//  lastTouch = millis();
//  lightsAreOn = false;
//}

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//          how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
//  In order for this to work now,
// The pin should have a resistor pulling
//  it up to +5v.
uint8_t readCapacitivePin(int pinToMeasure){
  // This is how you declare a variable which
  //  will hold the PORT, PIN, and DDR registers
  //  on an AVR
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  if ((pinToMeasure >= 0) && (pinToMeasure <= 7)){
    port = &PORTD;
    ddr = &DDRD;
    bitmask = 1 << pinToMeasure;
    pin = &PIND;
  }
  if ((pinToMeasure > 7) && (pinToMeasure <= 13)){
    port = &PORTB;
    ddr = &DDRB;
    bitmask = 1 << (pinToMeasure - 8);
    pin = &PINB;
  }
  if ((pinToMeasure > 13) && (pinToMeasure <= 19)){
    port = &PORTC;
    ddr = &DDRC;
    bitmask = 1 << (pinToMeasure - 13);
    pin = &PINC;
  }
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input WITHOUT the internal pull-up on
  *ddr &= ~(bitmask);
  // Now see how long the pin to get pulled up
  int cycles = 16000;
  for(int i = 0; i < cycles; i++){
    if (*pin & bitmask){
      cycles = i;
      break;
    }
  }
  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  
  return cycles;
}
