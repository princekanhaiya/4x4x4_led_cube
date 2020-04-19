#include <TimerOne.h> 
#include<SoftwareSerial.h>
#include "Arduino.h"
#include <setjmp.h>
#include <avr/pgmspace.h> 
#include <avr/interrupt.h>
#include <string.h>
#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_Z 3
#define ONE 1
#define ZERO 0
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0
#define TIMER_US 50                                   // 50 uS timer duration 
#define TICK_COUNTS 4000                              // 200 mS worth of timer ticks

//Macro definitions
#define SET_BIT(PORT,BIT) PORT |= (1<<BIT)
#define CLR_BIT(PORT,BIT) PORT &= ~(1<<BIT)
#define TOGGLE_BIT(PORT,BIT) PORT ^= (1<<BIT)
#define PIN_READ(PORT,PIN) (PORT&(1<<PIN))
#define UBLB(a,b,c,d) ((a&0x0f)<<12)|((b&0x0f)<<8)|((c&0x0f)<<4)|(d&0x0f);

typedef struct {
  float x;
  float y;
  float z;
} vertex;
typedef struct {
    int x;
    int y;
    int z;
} intvertex;

byte numberOfShiftRegisters=2, dataPin=2,clockPin=8,latchPin=7;
byte layer[4]={9,10,11,5};
byte echoPin = 3;
byte trigPin = 12;
const int BLE_Vcc = A1;
const int BLE_Gnd = A2; 
const int BLE_Rx = A3;
const int BLE_Tx = A4;
byte ledPin = A5;
jmp_buf env;

volatile unsigned char intFlag=0;
volatile unsigned char cube[4][4];
volatile unsigned char current_layer = 0;
volatile long echo_start = 0;                         // Records start of echo pulse 
volatile long echo_end = 0;                           // Records end of echo pulse
volatile long echo_duration = 0;                      // Duration - difference between end and start
volatile int trigger_time_count = 0;
volatile int patternNo=0;
byte firstTimeEffectFlag=2;
SoftwareSerial bt(A4,A3); /* (Rx,Tx) */ 
void setup(){
  DDRB |=0b00011111;//layer[4]={9,10,11,5},clk-8,trig-12(8-13--->PB0-PB5)
  DDRC |=0b00100110;//reserve-A0,BLE_VCC-A1,BLE_GND-A2,BLE_RX-A3,BLE_TX-A4 (A0-A5----->PC0-PC5)
  DDRD |=0b10100100;//RX-0,TX-1,DATA-2,ECHO-3,LAYER4-5,LATCH-7 (D0-D7---->PD0-PD7)
  CLR_BIT(DDRD,3);//Echo input pin

  PORTB |=0b00001110;
  CLR_BIT(PORTB,0);
  PORTC  =0b00000010;
  PORTD  =0b00100000;

  // Reset any PWM configuration that the arduino may have set up automagically!
  TCCR2A = 0x00;
  TCCR2B = 0x00;

  TCCR2A |= (0x01 << WGM21); // CTC mode. clear counter on TCNT2 == OCR2A
  OCR2A = 10; // Interrupt every 25600th cpu cycle (256*100)
  TCNT2 = 0x00; // start counting at 0
  TCCR2B |= (0x01 << CS22) | (0x01 << CS21); // Start the clock with a 256 prescaler

  TIMSK2 |= (0x01 << OCIE2A);
  Serial.begin(9600);
  randomSeed(analogRead(10));  //seeding random for random pattern
  Timer1.initialize(TIMER_US);                        // Initialise timer 1
  Timer1.attachInterrupt( timerIsr );                 // Attach interrupt to the timer service routine 
  attachInterrupt(digitalPinToInterrupt(echoPin), echo_interrupt, CHANGE);
  //delay_ms(25000);
}

ISR (TIMER2_COMPA_vect){
    unsigned char i;
    CLR_BIT(PORTD,7);  //Latch enable LOW.
    //CLR_BIT(PORTD,2); //clear shift register ready for sending data          
    for (int i=15; i>=0; i--)  {  // for each bit in dataOut send out a bit         
      CLR_BIT(PORTB,0); //set clockPin to LOW prior to sending bit 
      word temp=UBLB(cube[current_layer][3],cube[current_layer][2],cube[current_layer][1],cube[current_layer][0]);
      if(temp & (1<<i))
        SET_BIT(PORTD,2);
      else
        CLR_BIT(PORTD,2);   //Clear data pin     
      SET_BIT(PORTB,0); //send bit out on rising edge of clock                  
     // CLR_BIT(PORTD,2);         
    } 
    //stop shifting 
  SET_BIT(PORTD,7);  //Latch enable HIGH.

  PORTB |=0b00001110;
  SET_BIT(PORTD,5);
  if(current_layer <3)
    CLR_BIT(PORTB,current_layer+1);
  else
    CLR_BIT(PORTD,current_layer+2);
  current_layer++;
  
  if (current_layer == 4)
    current_layer = 0;

  if(intFlag){
      intFlag=0;
      longjmp(env, 1);  // go to start of loop
  }
}

void loop(){
  //Serial.println("above of interrupt call");
  setjmp(env);  // landing point from ISR exit
  if(firstTimeEffectFlag){
    patternNo=0;
    firstTimeEffectFlag--;
  } 
  //Serial.print("pattern NO=");Serial.print(patternNo);Serial.print(" firstTimeEffectFlag=");Serial.println(firstTimeEffectFlag);
  switch(patternNo){
    case 0:
    {
      byte i;
      for(i=0;i<10;i++)
        effect_planboing(random(1,4),random(200,700));
      for(i=0;i<3;i++)
        effect_intro();
      for(i=0;i<5;i++)
        zoom_pyramid();
      for(i=0;i<3;i++)
        space(100);
      for(i=0;i<5;i++)
        boxInSpace(50,random(300,1000),random(1,3));
      for(i=0;i<5;i++)
        effect_box_woopwoop (20,random(400,900));
      for(i=0;i<20;i++)
        effect_random_sparkle_flash(40,rand()%4,random(50,100));
      for(i=0;i<5;i++)
        aroundEdgeDown(80); 
      for(i=0;i<50;i++)
        propeller(250,random(1,3));
      for(i=0;i<5;i++)
        effect_rain(100,0);
      for(i=0;i<5;i++)
        randomflicker(100);
      for(i=0;i<5;i++){
        effect_telcstairs(1, 350, 15);
        effect_telcstairs(4, 350, 15);
        effect_telcstairs(2, 350, 15);
        effect_telcstairs(3, 350, 15);
      }
      for(i=0;i<16;i++){
        effect_telcstairs(1, 350, i);
      }
      for(i=0;i<16;i++){
        effect_telcstairs(2, 350, i);
      }
      boingboing(3000, 500, 2, 3);
      for(i=0;i<3;i++){
        sendplane_rand_z (3,350, 1500);
        sendplane_rand_z (3,350, 1500);
        sendplane_rand_z (3,350, 1500);
        delay_ms(2000);
        sendplane_rand_z (0,350, 1500);
        sendplane_rand_z (0,350, 1500);
        sendplane_rand_z (0,350, 1500);
        delay_ms(2000);
      }
      randomBoxWireframe(150,1000);
    }
      break;
    case 1:
      effect_planboing(random(1,4),random(200,700));
      break;
    case 2:
      effect_intro();
      break;
    case 3:
      zoom_pyramid();
      break;
    case 4:
      space(100);
      break;
    case 5:
      boxInSpace(50,random(300,1000),random(1,3));
      break;
    case 6:
      effect_box_woopwoop (20,random(400,900));
      break;
    case 7:
      effect_random_sparkle_flash(40,rand()%4,random(50,100));
      break;
    case 8:
      aroundEdgeDown(80); 
      break;
    case 9:
      propeller(250,random(1,3));
      break;
    case 10:
      effect_rain(random(40,80),0);
      flickerOff(70);
      flickerOn(70);
      fill(0x00);
      effect_rain(random(60,200),350);
      flickerOff(70);
      flickerOn(70);
      fill(0x00);
      break;
    case 11:
      randomflicker(100);
      break;
   case 12:
      effect_telcstairs(1, 350, 15);
      effect_telcstairs(4, 350, 15);
      effect_telcstairs(2, 350, 15);
      effect_telcstairs(3, 350, 15);
      break;
   case 13:
      for(int i=0;i<16;i++){
        effect_telcstairs(1, 350, i);
      }
      for(int i=0;i<16;i++){
        effect_telcstairs(2, 350, i);
      }
      break;
   case 14:
      sendplane_rand_z (3,350, 1500);
      break;
   case 15:
      sendplane_rand_z (0,350, 1500);
      break;
   case 16:
      turnOnAndOffAllByColumnSideways(350);
      break;
   case 17:
      effect_blinky2();
      break;
   case 18:
      turnOnAndOffAllByLayerUpAndDownNotTimed(400);
      break;
   case 19:
      firework(random(-2,3),random(-1,3),random(-100,400));
      break;
    case 20:
      flickerOn(450);
      break;
    case 21:
      flickerOff(450);
      break;
    case 22:
      sinelines(100,80);
      break;
    case 23:
      sidewaves(100,50);
      break;
    case 24:
      effect_random_filler(300,0);
      delay_ms(5000);
      effect_random_filler(300,1);
      delay_ms(5000);
      break;
    case 25:
      effect_z_updown(100,400);
      break;
    case 26:
      effect_loadbar(400);
      break;
    case 27:
      effect_random_sparkle();
      break;
    case 28:
      effect_wormsqueeze(2,1,-1,100,700);
      break;
    case 29:
      boingboing(200, 800, 1, 1);
      boingboing(200, 500, 2, 3);
      break;
    case 30:
      ripples(2000,100);
      break;
    case 31:
      linespin(2000,100);
      break;
    case 32:
      sinelines(2000,100);
      break;
    case 33:
      hurricane();
      break;
    case 34:
      randomBoxWireframe(20,1000);
      break;
    case 35:
      randomBoxWireframe(20,1000);
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
     patternNo++;
     if(patternNo==35)
        patternNo=0;
  }
  else
    digitalWrite(ledPin,LOW);
}
