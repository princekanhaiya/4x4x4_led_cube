#include <TimerOne.h> 
#include<SoftwareSerial.h>
#include "Arduino.h"
#define TIMER_US 50                                   // 50 uS timer duration 
#define TICK_COUNTS 4000                              // 200 mS worth of timer ticks
int numberOfShiftRegisters=2, serialDataPin=2,clockPin=8,latchPin=7;
const int trigPin = 12;
const int echoPin = 3;
const int ledPin = A5;
const int BLE_Tx = A4;
const int BLE_Rx = A3;
const int BLE_Gnd = A2;
const int BLE_Vcc = A1;
const int Reserve = A0;
// defines variables
long duration;
int distance;
volatile unsigned char cube[4][4];
volatile int current_layer = 0;
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;
int layer[4]={9,10,11,5};
volatile long echo_start = 0;                         // Records start of echo pulse 
volatile long echo_end = 0;                           // Records end of echo pulse
volatile long echo_duration = 0;                      // Duration - difference between end and start
volatile int trigger_time_count = 0;
volatile int patternNo=0;
volatile int intFlag=0;
SoftwareSerial bt(A4,A3); /* (Rx,Tx) */ 

class ShiftRegister74HC595 {
public:
    ShiftRegister74HC595(int numberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin);
    ~ShiftRegister74HC595();
    void setAll(const uint8_t * digitalValues);
#ifdef __AVR__
    void setAll_P(const uint8_t * digitalValuesProgmem); // Experimental, PROGMEM data
#endif
    uint8_t * getAll(); 
    void set(int pin, uint8_t value);
    void setNoUpdate(int pin, uint8_t value);
    void updateRegisters();
    void setAllLow();
    void setAllHigh(); 
    uint8_t get(int pin);
private:
    int _numberOfShiftRegisters;
    int _clockPin;
    int _serialDataPin;
    int _latchPin;
    uint8_t * _digitalValues;
};
// ShiftRegister74HC595 constructor
ShiftRegister74HC595::ShiftRegister74HC595(int numberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin){
    _numberOfShiftRegisters = numberOfShiftRegisters;
    _clockPin = clockPin;
    _serialDataPin = serialDataPin;
    _latchPin = latchPin;
    // define pins as outputs
    pinMode(clockPin, OUTPUT);
    pinMode(serialDataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    // set pins low
    digitalWrite(clockPin, LOW);
    digitalWrite(serialDataPin, LOW);
    digitalWrite(latchPin, LOW);
    // allocates the specified number of bytes and initializes them to zero
    _digitalValues = (uint8_t *)malloc(numberOfShiftRegisters * sizeof(uint8_t));
    memset(_digitalValues, 0, numberOfShiftRegisters * sizeof(uint8_t));
    updateRegisters();       // reset shift register
}
// ShiftRegister74HC595 destructor
// The memory allocated in the constructor is also released.
ShiftRegister74HC595::~ShiftRegister74HC595(){
    free(_digitalValues);
}
// Set all pins of the shift registers at once.
// digitalVAlues is a uint8_t array where the length is equal to the number of shift registers.
void ShiftRegister74HC595::setAll(const uint8_t * digitalValues){
    memcpy( _digitalValues, digitalValues, _numberOfShiftRegisters);   // dest, src, size
    updateRegisters();
}
#ifdef __AVR__
void ShiftRegister74HC595::setAll_P(const uint8_t * digitalValuesProgmem){
    PGM_VOID_P p = reinterpret_cast<PGM_VOID_P>(digitalValuesProgmem);
    memcpy_P( _digitalValues, p, _numberOfShiftRegisters);
    updateRegisters();
}
#endif
// Retrieve all states of the shift registers' output pins.
// The returned array's length is equal to the numbe of shift registers.
uint8_t * ShiftRegister74HC595::getAll(){
    return _digitalValues; 
}
// Set a specific pin to either HIGH (1) or LOW (0).
// The pin parameter is a positive, zero-based integer, indicating which pin to set.
void ShiftRegister74HC595::set(int pin, uint8_t value){
    if(intFlag){
      intFlag=0;
      return;
    }
    setNoUpdate(pin, value);
    updateRegisters();
}
// Updates the shift register pins to the stored output values.
// This is the function that actually writes data into the shift registers of the 74HC595
void ShiftRegister74HC595::updateRegisters(){
    for (int i = _numberOfShiftRegisters - 1; i >= 0; i--) {
        if(intFlag){
          intFlag=0;
          return;
        }
        shiftOut(_serialDataPin, _clockPin, MSBFIRST, _digitalValues[i]);
    }
    digitalWrite(_latchPin, HIGH); 
    digitalWrite(_latchPin, LOW); 
}
// Equivalent to set(int pin, uint8_t value), except the physical shift register is not updated.
// Should be used in combination with updateRegisters().
void ShiftRegister74HC595::setNoUpdate(int pin, uint8_t value){
    if (value == 1) {
        _digitalValues[pin / 8] |= 1 << (pin % 8);
    }
    else {
        _digitalValues[pin / 8] &= ~(1 << (pin % 8));
    }
}
// Returns the state of the given pin.
// Either HIGH (1) or LOW (0)
uint8_t ShiftRegister74HC595::get(int pin){
    return (_digitalValues[pin / 8] >> (pin % 8)) & 1;
}
// Sets all pins of all shift registers to HIGH (1).
void ShiftRegister74HC595::setAllHigh(){
    for (int i = 0; i < _numberOfShiftRegisters; i++) {
        if(intFlag){
          intFlag=0;
          return;
        }
        _digitalValues[i] = 255;
    }
    updateRegisters();
}
// Sets all pins of all shift registers to LOW (0).
void ShiftRegister74HC595::setAllLow(){
    for (int i = 0; i < _numberOfShiftRegisters; i++) {
        if(intFlag){
          intFlag=0;
          return;
        }
        _digitalValues[i] = 0;
    }
    updateRegisters();
}
ShiftRegister74HC595 sr (numberOfShiftRegisters,serialDataPin,clockPin,latchPin);
//sr.setAllLow(); // set all pins LOW
//sr.setAllHigh();
//uint8_t pinValues[] = { B10101010 }; 
//sr.setAll(pinValues);
// read pin (zero based, i.e. 6th pin)
//uint8_t stateOfPin5 = sr.get(5);
//set pins without immediate update
//sr.setNoUpdate(0, HIGH);
//sr.setNoUpdate(1, LOW);
//at this point of time, pin 0 and 1 did not change yet
//sr.updateRegisters(); // update the pins to the set values
void setup() {
  bt.begin(9600); /* Define baud rate for software serial communication */
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(ledPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(BLE_Vcc, OUTPUT);
  pinMode(BLE_Gnd, OUTPUT);
  digitalWrite(BLE_Vcc,HIGH);
  digitalWrite(BLE_Gnd,LOW);
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  for(int i = 0; i<4; i++)
     pinMode(layer[i],OUTPUT);

    // Reset any PWM configuration that the arduino may have set up automagically!
  TCCR2A = 0x00;
  TCCR2B = 0x00;
  turnEverythingOff();
  randomSeed(analogRead(10));  //seeding random for random pattern
  //Timer3.initialize(150000);
  Timer1.initialize(TIMER_US);                        // Initialise timer 1
  Timer1.attachInterrupt( timerIsr );                 // Attach interrupt to the timer service routine 
  attachInterrupt(digitalPinToInterrupt(echoPin), echo_interrupt, CHANGE);
}
void loop(){
// custom();
if (bt.available()) /* If data is available on serial port */
    {
     Serial.write(bt.read()); /* Print character received on to the serial monitor */
    }
  switch(patternNo){
    case 0:
      wireFrame();
      break;
    case 1:
      turnOnAndOffAllByLayerUpAndDownNotTimed();  
      break;
    case 2:
      turnOnAndOffAllByColumnSideways();   
      break;
    case 3:
      layerstompUpAndDown();
      break;
    case 4:
      flickerOff();   
      break;
    case 5:
      aroundEdgeDown();   
      break;
    case 6:
      randomflicker();  
      break;
    case 7:
      randomRain();  
      break;
    case 8:
      randomRainMultiDrop(); 
      break;
    case 9:
      diagonalRectangle();  
      break;
    case 10:
      propeller();   
      break;
    case 11:
      spiralInAndOut();  
      break;
    case 12:
      goThroughAllLedsOneAtATime(); 
      break;
    case 13:
      cubeFade();
      break;
    case 14:
      flickerOn();   
      break;
    case 15:
      flickerOn(); 
      flickerOff();   
      break;
  }
}
void timerIsr(){
    static volatile int state = 0;                 // State machine variable
      if (!(--trigger_time_count)){
         trigger_time_count = TICK_COUNTS;           // Reload
         state = 1;                                  // Changing to state 1 initiates a pulse
      }
      switch(state){
        case 0:                                      // Normal state does nothing
            break;
        case 1:                                      // Initiate pulse
           digitalWrite(trigPin, HIGH);              // Set the trigger output high
           state = 2;                                // and set state to 2
           break;
        case 2:                                      // Complete the pulse
        default:      
           digitalWrite(trigPin, LOW);               // Set the trigger output low
           state = 0;                                // and return state to normal 0
           break;
     }
}
void echo_interrupt(){
  switch (digitalRead(echoPin)){
    case HIGH:                                      // High so must be the start of the echo pulse
      echo_end = 0;                                 // Clear the end time
      echo_start = micros();                        // Save the start time
      break;
    case LOW:                                       // Low so must be the end of hte echo pulse
      echo_end = micros();                          // Save the end time
      echo_duration = echo_end - echo_start;        // Calculate the pulse duration
      break;
  }
  //echo_duration=map(echo_duration,0,1800016,0,10);
  if(echo_duration<=800){
     digitalWrite(ledPin,HIGH);
     intFlag=1;
     patternNo=random(0,16);
  }
  else
    digitalWrite(ledPin,LOW);
//  Serial.println(echo_duration);
  Serial.println(patternNo);
}
//turn all off
void turnEverythingOff(){
  sr.setAllLow();
  for(int i = 0; i<4; i++){
    if(intFlag){
      intFlag=0;
      return;
    }
    digitalWrite(layer[i],1);
  }
}
//turn all on
void turnEverythingOn(){
  sr.setAllHigh();
  for(int i = 0; i<4; i++){
    if(intFlag){
      intFlag=0;
      return;
    }
    digitalWrite(layer[i],0);
  }
}
//turn columns off
void turnColumnsOff(){
  sr.setAllLow();
  if(intFlag){
      intFlag=0;
      return;
  }
}
void flickerOn(void){
  int temp = 350;
  while(temp != 0){
    turnEverythingOn();
    delay_ms(temp);
    turnEverythingOff();
    delay_ms(temp);
    temp-= 5;
    if(intFlag){
      intFlag=0;
      return;
    }
  }
}
//turn everything on and off by layer up and down NOT TIMED
void turnOnAndOffAllByLayerUpAndDownNotTimed(){
  int x = 180;
  for(int i = 5; i != 0; i--){
    if(intFlag){
      intFlag=0;
      return;
    }
    turnEverythingOn();
    for(int i = 4; i!=0; i--){
      digitalWrite(layer[i-1], 1);
      delay_ms(x);
    }
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 0; i<4; i++){
      digitalWrite(layer[i], 0);
      delay_ms(x);
    }
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 0; i<4; i++){
      digitalWrite(layer[i], 1);
      delay_ms(x);
    }
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 4; i!=0; i--){
      digitalWrite(layer[i-1], 0);
      delay_ms(x);
    }
  }
}
//turn everything on and off by column sideways
void turnOnAndOffAllByColumnSideways(){
  int x = 125;
  turnEverythingOff();
  //turn on layers
  for(int i = 0; i<4; i++){
    digitalWrite(layer[i], 0);
  }
  for(int y = 0; y<3; y++)
  {
    //turn on 0-3
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 0; i<4; i++)
    {
      sr.set(i,1);
      delay_ms(x);
    }
    //turn on 4-7
    for(int i = 4; i<8; i++)
    {
      sr.set(i,1);
      delay_ms(x);
    }
    //turn on 8-11
    for(int i = 8; i<12; i++)
    {
      sr.set(i,1);
      delay_ms(x);
    }
    //turn on 12-15
    for(int i = 12; i<16; i++)
    {
      sr.set(i,1);
      delay_ms(x);
    }
    //turn off 0-3
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 0; i<4; i++)
    {
      sr.set(i,0);
      delay_ms(x);
    }
    //turn off 4-7
    for(int i = 4; i<8; i++)
    {
      sr.set(i,0);
      delay_ms(x);
    }
    //turn off 8-11
    for(int i = 8; i<12; i++)
    {
      sr.set(i,0);
      delay_ms(x);
    }
    //turn off 12-15
    for(int i = 12; i<16; i++)
    {
      sr.set(i,0);
      delay_ms(x);
    }
    //turn on 12-15
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 12; i<16; i++)
    {
      sr.set(i,1);
      delay_ms(x);
    }
    //turn on 8-11
    for(int i = 8; i<12; i++)
    {
      sr.set(i,1);
      delay_ms(x);
    }
    //turn on 4-7
    for(int i = 4; i<8; i++)
    {
      sr.set(i,1);
      delay_ms(x);
    }
    //turn on 0-3
    for(int i = 0; i<4; i++)
    {
      sr.set(i,1);
      delay_ms(x);
    }
    //turn off 12-15
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 12; i<16; i++)
    {
      sr.set(i,0);
      delay_ms(x);
    }
    //turn off 8-11
    for(int i = 8; i<12; i++)
    {
      sr.set(i,0);
      delay_ms(x);
    }
    //turn off 4-7
    for(int i = 4; i<8; i++)
    {
      sr.set(i,0);
      delay_ms(x);
    }
    //turn off 0-3
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 0; i<4; i++)
    {
      sr.set(i,0);
      delay_ms(x);
    }
  }
}
//up and down single layer stomp
void layerstompUpAndDown(){
  int x = 140;
  turnEverythingOn();
  for(int i = 0; i<4; i++){
    digitalWrite(layer[i], 1);
  }
  for(int y = 0; y<5; y++){
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int count = 0; count<1; count++){ 
       if(intFlag){
          intFlag=0;
          return;
        }
      for(int i = 0; i<4; i++){
        digitalWrite(layer[i], 0);
        delay_ms(x);
        digitalWrite(layer[i], 1);
      }
      if(intFlag){
        intFlag=0;
        return;
      }
      for(int i = 4; i !=0; i--){
        digitalWrite(layer[i-1], 0);
        delay_ms(x);
        digitalWrite(layer[i-1], 1);
      }
    }
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 0; i<4; i++){
      digitalWrite(layer[i], 0);
      delay_ms(x);
    }
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 4; i!=0; i--){
      digitalWrite(layer[i-1], 1);
      delay_ms(x);
    }
  }
}
//flicker off
void flickerOff(){
  turnEverythingOn();
  for(int i = 0; i!= 350; i+=5){
    if(intFlag){
      intFlag=0;
      return;
    }
    turnEverythingOff();
    delay_ms(i+10);
    turnEverythingOn();
    delay_ms(i);
  }
}
//around edge of the cube down
void aroundEdgeDown(){
  for(int x = 250; x != 0; x -=50){
    turnEverythingOff();
    for(int i = 4; i != 0; i--){
      digitalWrite(layer[i-1], 0);
      sr.set(5,1);
      sr.set(6,1);
      sr.set(9,1);
      sr.set(10,1);
      delay_ms(x);
      sr.set(0,1);
      sr.set(4,0);
      delay_ms(x);
      sr.set(0,0);
      sr.set(4,0);
      sr.set(8,1);
      delay_ms(x);
      sr.set(8,0);
      sr.set(12,1);
      delay_ms(x);
      sr.set(12,0);
      sr.set(13,1);
      delay_ms(x);
      sr.set(13,0);
      sr.set(15,1);
      delay_ms(x);
      sr.set(15,0);
      sr.set(14,1);
      delay_ms(x);
      sr.set(14,0);
      sr.set(11,1);
      delay_ms(x);
      sr.set(11,0);
      sr.set(7,1);
      delay_ms(x);
      sr.set(7,0);
      sr.set(3,1);
      delay_ms(x);
      sr.set(3,0);
      sr.set(2,1);
      delay_ms(x);
      sr.set(2,0);
      sr.set(1,1);
      delay_ms(x);
      sr.set(1,0);
    }
  }
}
//random flicker
void randomflicker(){
  turnEverythingOff();
  int x = 15;
  for(int i = 0; i !=750; i+=2){
  int randomLayer = random(0,4);
  int randomColumn = random(0,16);
  digitalWrite(layer[randomLayer], 0);
  sr.set(randomColumn, 1);
  delay_ms(x);
  digitalWrite(layer[randomLayer], 1);
  sr.set(randomColumn, 0);
  delay_ms(x); 
  }
}
//random rain
void randomRainUp(){
  turnEverythingOff();
  int x = 100;
  for(int i = 0; i!=60; i+=2){
    if(intFlag){
      intFlag=0;
      return;
    }
    int randomColumn = random(0,16);
    sr.set(randomColumn, 1);
    digitalWrite(layer[0], 0);
    delay_ms(x+50);
    digitalWrite(layer[0], 1);
    digitalWrite(layer[1], 0);
    delay_ms(x);
    digitalWrite(layer[1], 1);
    digitalWrite(layer[2], 0);
    delay_ms(x);
    digitalWrite(layer[2], 1);
    digitalWrite(layer[3], 0);
    delay_ms(x+50);
    digitalWrite(layer[3], 1);
    sr.set(randomColumn, 0);
  }
}
void randomRain(){
  turnEverythingOff();
  int x = 100;
  for(int i = 0; i!=60; i+=2){
    int randomColumn = random(0,16);
    sr.set(randomColumn, 1);
    digitalWrite(layer[3], 0);
    delay_ms(x+10);
    digitalWrite(layer[3], 1);
    digitalWrite(layer[2], 0);
    delay_ms(x);
    digitalWrite(layer[2], 1);
    digitalWrite(layer[1], 0);
    delay_ms(x);
    digitalWrite(layer[1], 1);
    digitalWrite(layer[0], 0);
    delay_ms(x+10);
    digitalWrite(layer[0], 1);
    sr.set(randomColumn, 0);
  }
}
void randomRainMultiDrop(){
  turnEverythingOff();
  int x = 280;
  for(int i = 0; i!=60; i+=2){
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int j=0;j<5;j++){
      sr.set(random(0,16), 1);
    }
    digitalWrite(layer[3], 0);
    delay_ms(x+10);
    digitalWrite(layer[3], 1);
    digitalWrite(layer[2], 0);
    delay_ms(x);
    digitalWrite(layer[2], 1);
    digitalWrite(layer[1], 0);
    delay_ms(x);
    digitalWrite(layer[1], 1);
    digitalWrite(layer[0], 0);
    delay_ms(x+10);
    digitalWrite(layer[0], 1);
    sr.setAllLow();
  }
}
//diagonal rectangle
void diagonalRectangle(){
  int x = 450;
  turnEverythingOff();
  for(int count = 0; count<5; count++)
  {
    //top left
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 0; i<8; i++)
      sr.set(i,1);
    digitalWrite(layer[3], 0);
    digitalWrite(layer[2], 0);
    delay_ms(x);
    turnEverythingOff();
    x-=5;
    //middle middle
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 4; i<12; i++)
      sr.set(i,1);
    digitalWrite(layer[1], 0);
    digitalWrite(layer[2], 0);
    delay_ms(x);
    turnEverythingOff();
    x-=5;
    //bottom right
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 8; i<16; i++)
      sr.set(i,1);
    digitalWrite(layer[0], 0);
    digitalWrite(layer[1], 0);
    delay_ms(x);
    turnEverythingOff();
    x-=5;
    //bottom middle
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 4; i<12; i++)
      sr.set(i,1);
    digitalWrite(layer[0], 0);
    digitalWrite(layer[1], 0);
    delay_ms(x);
    turnEverythingOff();
    x-=5;
    //bottom left
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 0; i<8; i++)
      sr.set(i,1);
    digitalWrite(layer[0], 0);
    digitalWrite(layer[1], 0);
    delay_ms(x);
    turnEverythingOff();
    x-=5;
    //middle middle
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 4; i<12; i++)
      sr.set(i,1);
    digitalWrite(layer[1], 0);
    digitalWrite(layer[2], 0);
    delay_ms(x);
    turnEverythingOff();
    x-=5;
    //top right
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 8; i<16; i++)
      sr.set(i,1);
    digitalWrite(layer[2], 0);
    digitalWrite(layer[3], 0);
    delay_ms(x);
    turnEverythingOff();
    x-=5;
    //top middle
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int i = 4; i<12; i++)
      sr.set(i,1);
    digitalWrite(layer[2], 0);
    digitalWrite(layer[3], 0);
    delay_ms(x);
    turnEverythingOff();
  }
  x-=5;
  //top left
  if(intFlag){
      intFlag=0;
      return;
    }
  for(int i = 0; i<8; i++)
    sr.set(i,1);
  digitalWrite(layer[3], 0);
  digitalWrite(layer[2], 0);
  delay_ms(x);
  turnEverythingOff();
}
//propeller
void propeller(){
  turnEverythingOff();
  int x = 120;
  for(int y = 4; y>0; y--)
  {
    for(int i = 0; i<6; i++)
    {
      if(intFlag){
        intFlag=0;
        return;
      }
      //turn on layer
      digitalWrite(layer[y-1], 0);
      //a1
      turnColumnsOff();
      sr.set(0,1);
      sr.set(5,1);
      sr.set(10,1);
      sr.set(15,1);
      delay_ms(x);
      //b1
      turnColumnsOff();
      sr.set(4,1);
      sr.set(5,1);
      sr.set(10,1);
      sr.set(11,1);
      delay_ms(x);
      //c1
      turnColumnsOff();
      sr.set(6,1);
      sr.set(7,1);
      sr.set(8,1);
      sr.set(9,1);
      delay_ms(x);
      //d1
      turnColumnsOff();
      sr.set(3,1);
      sr.set(6,1);
      sr.set(9,1);
      sr.set(12,1);
      delay_ms(x);
      //d2
      turnColumnsOff();
      sr.set(2,1);
      sr.set(6,1);
      sr.set(9,1);
      sr.set(13,1);
      delay_ms(x);
      //d3
      turnColumnsOff();
      sr.set(1,1);
      sr.set(5,1);
      sr.set(10,1);
      sr.set(14,1);
      delay_ms(x);
    }
  }
  //d4
  turnColumnsOff();
  sr.set(0,1);
  sr.set(5,1);
  sr.set(10,1);
  sr.set(15,1);
  delay_ms(x);
}
//spiral in and out
void spiralInAndOut(){
  turnEverythingOn();
  int x = 160;
  for(int i = 0; i<6; i++){
    //spiral in clockwise
    if(intFlag){
      intFlag=0;
      return;
    }
    sr.set(0, 0);
    delay_ms(x);
    sr.set(1, 0);
    delay_ms(x);
    sr.set(2, 0);
    delay_ms(x);
    sr.set(3, 0);
    delay_ms(x);
    sr.set(7, 0);
    delay_ms(x);
    sr.set(11, 0);
    delay_ms(x);
    sr.set(15, 0);
    delay_ms(x);
    sr.set(14, 0);
    delay_ms(x);
    sr.set(13, 0);
    delay_ms(x);
    sr.set(12, 0);
    delay_ms(x);
    sr.set(8, 0);
    delay_ms(x);
    sr.set(4, 0);
    delay_ms(x);
    sr.set(5, 0);
    delay_ms(x);
    sr.set(6, 0);
    delay_ms(x);
    sr.set(10,0);
    delay_ms(x);
    sr.set(9, 0);
    delay_ms(x);
//spiral out counter clockwise
    if(intFlag){
      intFlag=0;
      return;
    }
    sr.set(9, 1);
    delay_ms(x);
    sr.set(10, 1);
    delay_ms(x);
    sr.set(6, 1);
    delay_ms(x);
    sr.set(5, 1);
    delay_ms(x);
    sr.set(4, 1);
    delay_ms(x);
    sr.set(8, 1);
    delay_ms(x);
    sr.set(12, 1);
    delay_ms(x);
    sr.set(13, 1);
    delay_ms(x);
    sr.set(14, 1);
    delay_ms(x);
    sr.set(15, 1);
    delay_ms(x);
    sr.set(11, 1);
    delay_ms(x);
    sr.set(7, 1);
    delay_ms(x);
    sr.set(3, 1);
    delay_ms(x);
    sr.set(2, 1);
    delay_ms(x);
    sr.set(1, 1);
    delay_ms(x);
    sr.set(0, 1);
    delay_ms(x);
//spiral in counter clock wise
    if(intFlag){
      intFlag=0;
      return;
    }
    sr.set(0, 0);
    delay_ms(x);
    sr.set(4, 0);
    delay_ms(x);
    sr.set(8, 0);
    delay_ms(x);
    sr.set(12,0);
    delay_ms(x);
    sr.set(13,0);
    delay_ms(x);
    sr.set(14,0);
    delay_ms(x);
    sr.set(15,0);
    delay_ms(x);
    sr.set(11,0);
    delay_ms(x);
    sr.set(7, 0);
    delay_ms(x);
    sr.set(3, 0);
    delay_ms(x);
    sr.set(2, 0);
    delay_ms(x);
    sr.set(1, 0);
    delay_ms(x);
    sr.set(5, 0);
    delay_ms(x);
    sr.set(9, 0);
    delay_ms(x);
    sr.set(10,0);
    delay_ms(x);
    sr.set(6, 0);
    delay_ms(x);
//spiral out clock wise
    if(intFlag){
      intFlag=0;
      return;
    }
    sr.set(6, 1);
    delay_ms(x);
    sr.set(10, 1);
    delay_ms(x);
    sr.set(9, 1);
    delay_ms(x);
    sr.set(5, 1);
    delay_ms(x);
    sr.set(1, 1);
    delay_ms(x);
    sr.set(2, 1);
    delay_ms(x);
    sr.set(3, 1);
    delay_ms(x);
    sr.set(7, 1);
    delay_ms(x);
    sr.set(11,1);
    delay_ms(x);
    sr.set(15,1);
    delay_ms(x);
    sr.set(14,1);
    delay_ms(x);
    sr.set(13,1);
    delay_ms(x);
    sr.set(12,1);
    delay_ms(x);
    sr.set(8,1);
    delay_ms(x);
    sr.set(4,1);
    delay_ms(x);
    sr.set(0,1);
    delay_ms(x);
  }
}
//go through all leds one at a time
void goThroughAllLedsOneAtATime(){
  int x = 30;
  turnEverythingOff();
  for(int y = 0; y<5; y++){
    //0-3
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int count = 4; count != 0; count--){
      digitalWrite(layer[count-1], 0);
      for(int i = 0; i<4; i++){
        sr.set(i, 1);
        delay_ms(x);
        sr.set(i, 0);
        delay_ms(x);
      }
    digitalWrite(layer[count-1], 1);
    }
    //4-7
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int count = 0; count < 4; count++){
      digitalWrite(layer[count], 0);
      for(int i = 4; i<8; i++){
        sr.set(i, 1);
        delay_ms(x);
        sr.set(i, 0);
        delay_ms(x);
      }
    digitalWrite(layer[count], 1);
    }
    //8-11
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int count = 4; count != 0; count--){
      digitalWrite(layer[count-1], 0);
      for(int i = 8; i<12; i++){
        sr.set(i, 1);
        delay_ms(x);
        sr.set(i, 0);
        delay_ms(x);
      }
    digitalWrite(layer[count-1], 1);
    }
    //12-15
    if(intFlag){
      intFlag=0;
      return;
    }
    for(int count = 0; count < 4; count++){
      digitalWrite(layer[count], 0);
      for(int i = 12; i<16; i++)
      {
        sr.set(i, 1);
        delay_ms(x);
        sr.set(i, 0);
        delay_ms(x);
      }
    digitalWrite(layer[count], 1);
    }
  }
}
void cubeFade(void){
  sr.setAllHigh();
  analogWrite(5, brightness);
  analogWrite(11, brightness);
  analogWrite(10, brightness);
  analogWrite(9, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 255){
    fadeAmount = -fadeAmount;
  }
  delay_ms(80);
}
void wireFrame(){
  int time =30;
  int matrix[][16] = {
    {1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1,},
    {1,0,0,1, 0,0,0,0, 0,0,0,0, 1,0,0,1,},
    {1,0,0,1, 0,0,0,0, 0,0,0,0, 1,0,0,1,},
    {1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1,}
  };
  int matrix1[][16] = {
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,},
    {0,0,0,0, 0,1,1,0, 0,1,1,0, 0,0,0,0,},
    {0,0,0,0, 0,1,1,0, 0,1,1,0, 0,0,0,0,},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,}
  };
  for(int i = 0; i < time; i++){
    for(int lay = 0; lay < 4; lay++){ ///looping each layer
      turnEverythingOff();
      digitalWrite(layer[lay], 0);
      for(int col = 0; col < 16; col++){
        if(matrix[lay][col]==1){
           if(intFlag){
              intFlag=0;
              return;
           }
          sr.set(col,1);
        }
      }
    delay_ms(4);
   }
  }
  for(int i = 0; i < time; i++){
    for(int lay = 0; lay < 4; lay++){ ///looping each layer
      turnEverythingOff();
      digitalWrite(layer[lay], 0);
      for(int col = 0; col < 16; col++){
        if(matrix1[lay][col]==1){
          if(intFlag){
              intFlag=0;
              return;
           }
          sr.set(col,1);
        }
      }
    delay_ms(1);
   }
  }
}
void delay_ms(uint16_t x){  
  uint8_t y, z;
  for ( ; x > 0 ; x--) {
    for ( y = 0 ; y < 90 ; y++) {
      for ( z = 0 ; z < 6 ; z++) {
        if(intFlag){
          return;
          intFlag=0;
        }
        asm volatile ("nop");
      }
      if(intFlag){
        return;
        intFlag=0;
      }
    }
    if(intFlag){
      return;
      intFlag=0;
    }
  }
}

void custom(){
  int a[][16]={0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int b[][16]={0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int c[][16]={0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int d[][16]={1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1};
  int e[][16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0};
  int f[][16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0};
  int g[][16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0};  
  
  int h[][16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0};
  int i[][16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0};
  int j[][16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0};
  int k[][16]={1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1};
  int l[][16]={0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int m[][16]={0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int n[][16]={0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int time=100;
  animateBuff(a);
  delay_ms(time);
  animateBuff(b);
  delay_ms(time);
  animateBuff(c);
  delay_ms(time);
  animateBuff(d);
  delay_ms(time);
  animateBuff(e);
  delay_ms(time);
  animateBuff(f);
  delay_ms(time);
  animateBuff(g);
  delay_ms(time);
  animateBuff(h);
  delay_ms(time);
  animateBuff(i);
  delay_ms(time);
  animateBuff(j);
  delay_ms(time);
  animateBuff(k);
  delay_ms(time);
  animateBuff(l);
  delay_ms(time);
  animateBuff(m);
  delay_ms(time);
  animateBuff(n);
  delay_ms(time);
}
void animateBuff(int matrix[][16]){
  int time =30;
  for(int i = 0; i < time; i++){
    for(int lay = 0; lay < 4; lay++){ ///looping each layer
      turnEverythingOff();
      digitalWrite(layer[lay], 0);
      for(int col = 0; col < 16; col++){
        if(matrix[lay][col]==1){
          sr.set(col,1);
        }
      }
   }
  }
}
