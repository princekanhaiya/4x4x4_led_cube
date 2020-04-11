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
//  aroundEdgeDown();   
//  cubeFade();
//  diagonalRectangle();  
//  flickerOff(); 
//  flickerOff();   
//  flickerOn(); 
//  flickerOn();   
//  goThroughAllLedsOneAtATime(); 
//  layerstompUpAndDown();
//  propeller();   
//  randomflicker();  
//  randomRain();  
//  randomRainMultiDrop(); 
//  spiralInAndOut();  
//turnOnAndOffAllByColumnSideways();   
//turnOnAndOffAllByLayerUpAndDownNotTimed();    
//wireFrame(); 
//columnScrollingFromCorMainCoposition();
//frontToBackLineScroll(80);
//leftToRightLineScroll(40);
//for(int i=0;i<4;i++){
//  plane('Z',i);
//  delay(200);
//}
//for(int i=3;i>=0;i--){
//  plane('Z',i);
//  delay(100);
//}
//frontToBackLineScroll(100);
//for(int i=0;i<4;i++){
//  plane('X',i);
//  delay(200);
//}
//for(int i=3;i>=0;i--){
//  plane('X',i);
//  delay(100);
//}
//leftToRightLineScroll(100);
//for(int i=0;i<4;i++){
//  plane('Y',i);
//  delay(200);
//}
//for(int i=3;i>=0;i--){
//  plane('Y',i);
//  delay(100);
//}
//int k=-1;
//for(int i=0;i<4;i++){
//  for(int j=0;j<4;j++){
//    if(i%2==0)
//      k++;
//    else
//      k--;
//    line('H', i,k);
//    delay_ms(300);
//  }
//}
//for(int i=0;i<4;i++){
//  for(int j=0;j<4;j++){
//    line('V', i,j);
//    delay_ms(1000);
//  }
//}
//for(int k=0;k<4;k++){
//for(int i=0;i<4;i++){
//  for(int j=0;j<7;j++){
//    line('D', i,j,k);//FLL=0,FLR=1,BLL=2,BLR=3
//    delay_ms(200);
//  }
//}
//}
//for(int x=0;x<16;x++){
//    //for(int j=0;j<4;j++){
//      for(int i=0;i<7;i++)
//          lineDigBLL(i,0,x);
//          delay(500);
//    //}
//}
//    for(int j=0;j<4;j++){
//      for(int i=0;i<7;i++)
//          lineDigFLR(i,j);
//          delay(500);
//    }
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

