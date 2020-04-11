// Returns a byte with a row of 1's drawn in it.
// byteline(2,8) gives 0b0000000111111100
word byteline (byte start, byte end){
  return ((0xffff << start) & ~(0xffff << (end + 1)));
}
// Flips a byte 180 degrees.
// MSB becomes LSB, LSB becomes MSB.
word flipbyte (word v){
  // swap odd and even bits
  v = ((v >> 1) & 0x5555) | ((v & 0x5555) << 1);
  // swap consecutive pairs
  v = ((v >> 2) & 0x3333) | ((v & 0x3333) << 2);
  // swap nibbles ... 
  v = ((v >> 4) & 0x0F0F) | ((v & 0x0F0F) << 4);
  // swap bytes
  v = ((v >> 8) & 0x00FF) | ((v & 0x00FF) << 8);
  return v;
}
// This function validates that we are drawing inside the cube.
unsigned char inrange(int x, int y, int z){
  if (x >= 0 && x < 4 && y >= 0 && y < 4 && z >= 0 && z < 4)
    return 0x01;
  else
    // One of the coordinates was outside the cube.
    return 0x00;
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
//turn all columns off at a time
void turnColumnsOff(){
  sr.setAllLow();
  if(intFlag){
      intFlag=0;
      return;
  }
}
//turn all columns on at a time
void turnColumnsOn(){
  sr.setAllHigh();
  if(intFlag){
      intFlag=0;
      return;
  }
}
//Turn on all rows at a time
void turnRowsOn(){
  for(int i = 0; i<4; i++){
    if(intFlag){
      intFlag=0;
      return;
    }
    digitalWrite(layer[i],0);
  }
}
//Turn off all rows at a time
void turnRowsOff(){
  for(int i = 0; i<4; i++){
    if(intFlag){
      intFlag=0;
      return;
    }
    digitalWrite(layer[i],1);
  }
}
//Turn on particular column
void columnOn(int column){
  if(column<0)
    sr.set(column,LOW);
  else
    sr.set(column,HIGH);
  if(intFlag){
      intFlag=0;
      return;
  }
}
//Turn off particular column
void columnOff(int column){
  if(column<0)
    sr.set(column,HIGH);
  else
    sr.set(column,LOW);
  if(intFlag){
      intFlag=0;
      return;
  }
}
//Turn on all particular row
void rowOn(int row){
  digitalWrite(layer[row],0);
  if(intFlag){
      intFlag=0;
      return;
  }
}
//Turn off all particular row
void rowOff(int row){
  digitalWrite(layer[row],1);
  if(intFlag){
      intFlag=0;
      return;
  }
}

void ledOn(int x,int y){
  turnEverythingOff();
  rowOn(x);
  columnOn(y);
}
void ledOff(int x,int y){
  rowOff(x);
  columnOff(y);
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

void columnScrollingFrom0cor(int timex){
  turnEverythingOff();
  turnRowsOn();
  byte temp1[7]={0,1,2,3,7,11,15};
  for(int i=0;i<7;i++){
    int flag=temp1[i];
    for(int j=0;j<4;j++){
      if(((i<4)&&(j-i)<1)||((i>3)&&(i+j)<7)){
        columnOn(flag); 
        flag+=3;
      }
    }
    delay(timex);
    turnColumnsOff();
  }
}

void columnScrollingFrom3cor(int timex){
  turnEverythingOff();
  turnRowsOn();
  byte temp2[7]={3,2,1,0,4,8,12};
  for(int i=0;i<7;i++){
    int flag=temp2[i];
    for(int j=0;j<4;j++){
      if(((i<4)&&(j-i)<1)||((i>3)&&(i+j)<7)){
        columnOn(flag); 
        flag+=5;
      }
    }
    delay(timex);
    turnColumnsOff();
  }
}

void columnScrollingFrom12cor(int timex){
  turnEverythingOff();
  turnRowsOn();
  byte temp3[7]={12,8,4,0,1,2,3};
  for(int i=0;i<7;i++){
    int flag=temp3[i];
    for(int j=0;j<4;j++){
      if(((i<4)&&(j-i)<1)||((i>3)&&(i+j)<7)){
        columnOn(flag); 
        flag+=5;
      }
    }
    delay(timex);
    turnColumnsOff();
  }
}

void columnScrollingFrom15cor(int timex){
  turnEverythingOff();
  turnRowsOn();
  byte temp4[7]={15,14,13,12,8,4,0};
  for(int i=0;i<7;i++){
    int flag=temp4[i];
    for(int j=0;j<4;j++){
      if(((i<4)&&(j-i)<1)||((i>3)&&(i+j)<7)){
        columnOn(flag); 
        flag-=3;
      }
    }
    delay(timex);
    turnColumnsOff();
  }
}

void columnScrollingFromCorMainCoposition(void){
  int temp=random(0,4);
  int timex=random(80,150);
  switch(temp){
    case 0:
      columnScrollingFrom0cor(timex);
      break;
    case 1:
      columnScrollingFrom3cor(timex);
      break;
    case 2:
      columnScrollingFrom12cor(timex);
      break;
    case 3:
      columnScrollingFrom15cor(timex);
      break;
  }
}

void frontToBackLineScroll(int timex){
  turnEverythingOff();

  //upper plane line scan
  byte temp1[4]={0,4,8,12};
  for(int i=0;i<4;i++){
    rowOn(3);
    int flag=temp1[i];
    for(int j=0;j<4;j++){
        columnOn(flag++); 
    }
    delay(timex);
    turnColumnsOff();
  }

  //Back plane line scan
  rowOff(3);
  for(int i=0;i<4;i++)
    columnOn(i+12); 
  for(int i=3;i>=0;i--){
    rowOn(i);
    delay(timex);
    rowOff(i);
  }

  //Down plane line scan
  byte temp3[4]={12,8,4,0};
  for(int i=0;i<4;i++){
    rowOn(0);
    int flag=temp3[i];
    for(int j=0;j<4;j++){
        columnOn(flag++); 
    }
    delay(timex);
    turnColumnsOff();
  }

  //Down plane line scan
  rowOff(0);
  for(int i=0;i<4;i++)
    columnOn(i); 
  for(int i=0;i<4;i++){
    rowOn(i);
    delay(timex);
    rowOff(i);
  }
}

void leftToRightLineScroll(int timex){
  turnEverythingOff();
  byte temp1[4]={0,1,2,3};
  for(int i=0;i<4;i++){
    rowOn(3);
    int flag=temp1[i];
    for(int j=0;j<15;j++){
        columnOn(flag); 
        flag+=4;
    }
    delay(timex);
    turnColumnsOff();
  }

  rowOff(3);
  for(int i=3;i<16;i+=4)
    columnOn(i); 
  for(int i=3;i>=0;i--){
    rowOn(i);
    delay(timex);
    rowOff(i);
  }

  byte temp3[4]={3,2,1,0};
  for(int i=0;i<4;i++){
    rowOn(0);
    int flag=temp3[i];
    for(int j=0;j<4;j++){
        columnOn(flag);
        flag+=4; 
    }
    delay(timex);
    turnColumnsOff();
  }

  rowOff(0);
  for(int i=0;i<13;i+=4)
    columnOn(i); 
  for(int i=0;i<4;i++){
    rowOn(i);
    delay(timex);
    rowOff(i);
  }
}

void plane(char plane, int no){
  turnEverythingOff();
  if(plane=='X'){
    rowOn(no);
    turnColumnsOn();
  }
  if(plane=='Y'){
    byte temp1[4]={0,4,8,12};
    for(int i=0;i<4;i++){
      columnOn(no+temp1[i]);
    }
    turnRowsOn();
  }
  if(plane=='Z'){
    byte temp1[4]={0,1,2,3};
    for(int i=0;i<4;i++){
      columnOn(4*no+temp1[i]);
    }
    turnRowsOn();
  }
}

void line(char plane, int planeno,int planeline,int cornerxy){
  turnEverythingOff();
  if(plane=='H'){
    rowOn(planeno);
    byte temp[4]={0,1,2,3};
    for(int i=0;i<4;i++)
      columnOn(temp[i]+4*planeline);
  }
  if(plane=='V'){
    rowOn(planeno);
    byte temp[4]={0,4,8,12};
    for(int i=0;i<4;i++)
      columnOn(temp[i]+planeline);
  }
  if(plane=='D'&&cornerxy==0){
    byte temp[7]={1,2,3,4,3,2,1};
    byte temp1[7]={0,1,2,3,7,11,15};
    rowOn(planeno);
    for(int i=0;i<temp[planeline];i++){
      int sum=temp1[planeline]+3*i;
      columnOn(sum);
    }
  }
  if(plane=='D'&&cornerxy==1){
    byte temp[7]={1,2,3,4,3,2,1};
    byte temp1[7]={3,2,1,0,4,8,12};
    rowOn(planeno);
    for(int i=0;i<temp[planeline];i++){
      int sum=temp1[planeline]+5*i;
      columnOn(sum);
    }
  }
  if(plane=='D'&&cornerxy==2){
    byte temp[7]={1,2,3,4,3,2,1};
    byte temp1[7]={12,8,4,0,1,2,3};
    rowOn(planeno);
    for(int i=0;i<temp[planeline];i++){
      int sum=temp1[planeline]+5*i;
      columnOn(sum);
    }
  }
  if(plane=='D'&&cornerxy==3){
    byte temp[7]={1,2,3,4,3,2,1};
    byte temp1[7]={15,11,7,3,2,1,0};
    rowOn(planeno);
    for(int i=0;i<temp[planeline];i++){
      int sum=temp1[planeline]+3*i;
      columnOn(sum);
    }
  }
}

void lineDigFLL(int line,int pat){
  word mat[4][7][4]={ 
                     {{0b0000000000000001,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000010,0b0000000000000001,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000100,0b0000000000000010,0b0000000000000001,0b0000000000000000},
                      {0b0000000000001000,0b0000000000000100,0b0000000000000010,0b0000000000000001},
                      {0b0000000000000000,0b0000000000001000,0b0000000000000100,0b0000000000000010},
                      {0b0000000000000000,0b0000000000000000,0b0000000000001000,0b0000000000000100},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0000000000001000}},
                    
                     {{0b0000000000000001,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000010000,0b0000000000000001,0b0000000000000000,0b0000000000000000},
                      {0b0000000100000000,0b0000000000010000,0b0000000000000001,0b0000000000000000},
                      {0b0001000000000000,0b0000000100000000,0b0000000000010000,0b0000000000000001},
                      {0b0000000000000000,0b0001000000000000,0b0000000100000000,0b0000000000010000},
                      {0b0000000000000000,0b0000000000000000,0b0001000000000000,0b0000000100000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0001000000000000}},

                     {{0b0000000000000001,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000010010,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000100100100,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0001001001001000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0010010010000000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0100100000000000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b1000000000000000,0b0000000000000000,0b0000000000000000,0b0000000000000000}},

                     {{0b0000000000000001,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000000,0b0000000000010010,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000100100100,0b0000000000000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0001001001001000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0000000000000000}}
                    };
  int time =30;
  for(int i = 0; i < time; i++){
    for(int lay = 0; lay < 4; lay++){ ///looping each layer
      turnEverythingOff();
      digitalWrite(layer[lay],0);
      for(int col = 0; col < 16; col++){
        if(bitRead(mat[pat][line][lay],col)==1){
          sr.set(col,1);
        }
      }
    }
  }  
}
void lineDigFLR(int line,int pat){
  word mat[4][7][4]={ 
                     {{0b0000000000001000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000010000000,0b0000000000001000,0b0000000000000000,0b0000000000000000},
                      {0b0000100000000000,0b0000000010000000,0b0000000000001000,0b0000000000000000},
                      {0b1000000000000000,0b0000100000000000,0b0000000010000000,0b0000000000001000},
                      {0b0000000000000000,0b1000000000000000,0b0000100000000000,0b0000000010000000},
                      {0b0000000000000000,0b0000000000000000,0b1000000000000000,0b0000100000000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b1000000000000000}},
                    
                     {{0b0000000000001000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000100,0b0000000000001000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000010,0b0000000000000100,0b0000000000001000,0b0000000000000000},
                      {0b0000000000000001,0b0000000000000010,0b0000000000000100,0b0000000000001000},
                      {0b0000000000000000,0b0000000000000001,0b0000000000000010,0b0000000000000100},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000001,0b0000000000000010},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0000000000000001}},

                     {{0b0000000000001000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000010000100,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000100001000010,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b1000010000100001,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0100001000010000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0010000100000000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0001000000000000,0b0000000000000000,0b0000000000000000,0b0000000000000000}},

                     {{0b0000000000001000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000000,0b0000000010000100,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000000,0b0000000000000000,0b0000100001000010,0b0000000000000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b1000010000100001},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0000000000000000}}
};
  int time =30;
  for(int i = 0; i < time; i++){
    for(int lay = 0; lay < 4; lay++){ ///looping each layer
      turnEverythingOff();
      digitalWrite(layer[lay],0);
      for(int col = 0; col < 16; col++){
        if(bitRead(mat[pat][line][lay],col)==1){
          sr.set(col,1);
        }
      }
    }
  }  
}
void lineDigBLL(int line,int pat,int shift){
  word mat[7][4]={ 
                      {0b0000000000000001,0b0000000000000000,0b0000000000000000,0b0000000000000000},
                      {0b0000000000010000,0b0000000000000001,0b0000000000000000,0b0000000000000000},
                      {0b0000000100000000,0b0000000000010000,0b0000000000000001,0b0000000000000000},
                      {0b0001000000000000,0b0000000100000000,0b0000000000010000,0b0000000000000001},
                      {0b0000000000000000,0b0001000000000000,0b0000000100000000,0b0000000000010000},
                      {0b0000000000000000,0b0000000000000000,0b0001000000000000,0b0000000100000000},
                      {0b0000000000000000,0b0000000000000000,0b0000000000000000,0b0001000000000000}
                    };
  int time =20;
  for(int i = 0; i < time; i++){
    for(int lay = 0; lay < 4; lay++){ ///looping each layer
      turnEverythingOff();
      digitalWrite(layer[lay],0);
      for(int col = 0; col < 16; col++){
        word temp=(mat[line][lay]<<shift);
        if(bitRead(temp,col)==1){
          sr.set(col,1);
        }
      }
    }
  }  
}

void square2x2(){





}


