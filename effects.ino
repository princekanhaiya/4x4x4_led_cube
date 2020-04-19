// Shift the entire contents of the cube along an axis
// This is great for effects where you want to draw something
// on one side of the cube and have it flow towards the other
// side. Like rain flowing down the Z axiz.
float distance2d (float x1, float y1, float x2, float y2){  
  float dist;
  dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
  return dist;
}
float distance3d (float x1, float y1, float z1, float x2, float y2, float z2){  
  float dist;
  dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
  return dist;
}
void shift (char axis, int direction){
  int i, x , y;
  int ii, iii;
  int state;

  for (i = 0; i < 4; i++){
    if (direction == -1){
      ii = i;
    } 
    else{
      ii = (3 - i);
    }


    for (x = 0; x < 4; x++){
      for (y = 0; y < 4; y++){
        if (direction == -1){
          iii = ii + 1;
        } 
        else{
          iii = ii - 1;
        }

        if (axis == AXIS_Z){
          state = getvoxel(x, y, iii);
          altervoxel(x, y, ii, state);
        }

        if (axis == AXIS_Y){
          state = getvoxel(x, iii, y);
          altervoxel(x, ii, y, state);
        }

        if (axis == AXIS_X){
          state = getvoxel(iii, y, x);
          altervoxel(ii, y, x, state);
        }
      }
    }
  }
  if (direction == -1){
    i = 3;
  } 
  else{
    i = 0;
  }

  for (x = 0; x < 4; x++){
    for (y = 0; y < 4; y++){
      if (axis == AXIS_Z)
        clrvoxel(x, y, i);

      if (axis == AXIS_Y)
        clrvoxel(x, i, y);

      if (axis == AXIS_X)
        clrvoxel(i, y, x);
    }
  }
}
void firework(int i, int j, int time) {
  fill(0x00);

  setvoxel(1 - i, 2 - j, 0);
  delay_ms(800 - time);

  clrvoxel(1 - i, 2 - j, 0);
  setvoxel(2 - i, 2 - j, 1);
  delay_ms(1100 - time);

  clrvoxel(2 - i, 2 - j, 1);
  setvoxel(2 - i, 2 - j, 2);
  delay_ms(1300 - time);

  clrvoxel(2 - i, 2 - j, 2);
  setvoxel(2 - i, 1 - j, 3);
  delay_ms(1600 - time);

  clrvoxel(2 - i, 1 - j, 3);
  delay_ms(1900 - time);

  //Explode
  clrvoxel(2 - i, 2 - j, 3);
  setvoxel(2 - i, 2 - j, 2);
  setvoxel(2 - i, 3 - j, 3);
  setvoxel(2 - i, 1 - j, 3);
  setvoxel(1 - i, 2 - j, 3);
  setvoxel(3 - i, 2 - j, 3);
  delay_ms(1900 - time);

   shift(AXIS_Z, -1);
  setvoxel(3 - i, 2 - j, 2);
  setvoxel(3 - i, 1 - j, 1);
  setvoxel(1 - i, 2 - j, 2);
  setvoxel(3 - i, 2 - j, 2);
  delay_ms(800 - time);

  shift(AXIS_Z, -1);
  setvoxel(2 - i, 1 - j, 1);
  setvoxel(2 - i, 0 - j, 0);
  setvoxel(1 - i, 3 - j, 1);
  setvoxel(3 - i, 3 - j, 1);
  delay_ms(800 - time);

  shift(AXIS_Z, -1);
  setvoxel(2 - i, 3 - j, 0);
  setvoxel(2 - i, 0 - j, 0);
  setvoxel(0 - i, 2 - j, 0);
  setvoxel(3 - i, 1 - j, 0);
  delay_ms(1300 - time);

  shift(AXIS_Z, -1);
  delay_ms(1300 - time);

  shift(AXIS_Z, -1);
  delay_ms(1300 - time);

  shift(AXIS_Z, -1);
  delay_ms(1300 - time);

  shift(AXIS_Z, -1);
  delay_ms(600 - time);

  fill(0x00);
}
void space(int iterations) {
  int i, ii;
  int rnd_x;
  int rnd_y;
  int rnd_z;
  int rnd_num;
  int time;
  time = 600;
  for (ii = 0; ii < iterations; ii++){
    time = time - (iterations / 20);
    rnd_num = rand() % 3;
    for (i = 0; i < rnd_num; i++){
      rnd_x = rand() % 4;
      rnd_y = rand() % 4;
      rnd_z = rand() % 4;
      setvoxel(rnd_x,0,rnd_z);
    }
    delay_ms(time);
    shift(AXIS_Y, 1);
  }
  for (ii = 0; ii < iterations; ii++)
  {
    time = time + (iterations / 18);
    rnd_num = rand() % 3;
    for (i = 0; i < rnd_num; i++)
    {
      rnd_x = rand() % 4;
      rnd_y = rand() % 4;
      rnd_z = rand() % 4;
      setvoxel(rnd_x,0,rnd_z);
    }
    delay_ms(time);
    shift(AXIS_Y, 1);
  }
}
void effect_intro() {
  int cnt, cnt_2;
  int time=2000;
  //Bottom To Top
  for (cnt = 0; cnt <= 3; cnt++) {
    box_wireframe(0, 0, 0, 3, 3, cnt);
    delay_ms(600);
  }
  for (cnt = 0; cnt <= 3; cnt++) {
    clrplane_z(cnt);
    delay_ms(600);
  }

  for (cnt = 0; cnt <= 3; cnt++) {
    box_wireframe(0, 0, 0, 3, 3, cnt);
    delay_ms(600);
  }
  for (cnt = 0; cnt < 3; cnt++) {
    clrplane_z(cnt);
    delay_ms(600);
  }

  //Shift Things Right
  //1
  shift(AXIS_Y, -1);
  for (cnt = 0; cnt <= 3; cnt++) {
    setvoxel(cnt, 0, 2);
  }
  delay_ms(1000);
  //2
  shift(AXIS_Y, -1);
  for (cnt = 0; cnt <= 3; cnt++) {
    setvoxel(cnt, 0, 1);
  }
  setvoxel(0, 0, 2);
  setvoxel(3, 0, 2);
  delay_ms(1000);
  //3
  shift(AXIS_Y, -1);
  for (cnt = 0; cnt <= 7; cnt++) {
    setvoxel(cnt, 0, 0);
  }
  setvoxel(0, 0, 1);
  setvoxel(3, 0, 1);
  setvoxel(0, 0, 2);
  setvoxel(3, 0, 2);
  delay_ms(1000);


  //Right To Left
  for (cnt = 0; cnt <= 3; cnt++) {
    box_wireframe(0, 0, 0, 3, cnt, 3);
    delay_ms(600);
  }
  for (cnt = 0; cnt < 3; cnt++) {
    clrplane_y(cnt);
    delay_ms(600);
  }


  //Shift to the bottom
  for (cnt_2 = 2; cnt_2 >= 0; cnt_2--) {
    shift(AXIS_Z, -1);
    for (cnt = 0; cnt <= 3; cnt++) {
      setvoxel(cnt, cnt_2, 0);
    }
    for (cnt = 2; cnt > cnt_2; cnt--) {
      setvoxel(0, cnt, 0);
      setvoxel(3, cnt, 0);
    }

    delay_ms(1200);
  }

  //Make All Wall Box

  for (cnt = 0; cnt <= 2; cnt++) {
    fill(0x00);
    box_walls(0, 0, 0, 3, 3, cnt);
    delay_ms(1000);
  }

  for (cnt_2 = 0; cnt_2 < 4; cnt_2++) {
    time = time - 300;
    //Make Box Smaller
    for (cnt = 0; cnt <= 1; cnt++) {
      fill(0x00);
      box_walls(cnt, cnt, cnt, 3 - cnt, 3 - cnt, 3 - cnt);
      delay_ms(time);
    }
  }
  for (cnt_2 = 0; cnt_2 < 4; cnt_2++) {
    time = time + 300;
    //Make Box Smaller
    for (cnt = 0; cnt <= 1; cnt++) {
      fill(0x00);
      box_walls(cnt, cnt, cnt, 3 - cnt, 3 - cnt, 3 - cnt);
      delay_ms(time);
    }
  }
  delay_ms(1500);
}
// Draw a plane on one axis and send it back and forth once.
void effect_planboing (int plane, int speed){
  int i;
  for (i = 0; i < 4; i++)
  {
    fill(0x00);
    setplane(plane, i);
    delay_ms(speed);
  }

  for (i = 3; i >= 0; i--)
  {
    fill(0x00);
    setplane(plane, i);
    delay_ms(speed);
  }
}
void zoom_pyramid() {
  int i, j, k, time;
  //1
  fill(0x00);
  box_walls(0, 0, 0, 3, 0, 3);
  delay_ms(1500);
  //2
  fill(0x00);
  //Pyramid
  box_wireframe(0, 0, 0, 3, 0, 1);
  box_walls(0, 1, 0, 3, 1, 3);
  delay_ms(1500);
  //3
  fill(0x00);
  //Pyramid
  box_wireframe(0, 0, 0, 3, 0, 1);
  box_wireframe(1, 1, 0, 2, 1, 3);
  box_wireframe(0, 1, 0, 3, 1, 2);
  box_walls(0, 2, 0, 3, 2, 3);
  delay_ms(1500);
  //4
  fill(0x00);
  //Pyramid
  box_wireframe(0, 0, 0, 3, 0, 1);
  box_wireframe(1, 1, 0, 2, 1, 3);
  box_wireframe(0, 1, 0, 3, 1, 2);
  box_wireframe(1, 2, 0, 2, 2, 3);
  box_wireframe(0, 2, 0, 3, 2, 2);
  box_walls(0, 3, 0, 3, 3, 3);
  delay_ms(1500);
   //5
  fill(0x00);
  //Pyramid
  box_wireframe(0, 0, 0, 3, 0, 1);
  box_wireframe(1, 1, 0, 2, 1, 3);
  box_wireframe(0, 1, 0, 3, 1, 2);
  box_wireframe(1, 2, 0, 2, 2, 3);
  box_wireframe(0, 2, 0, 3, 2, 2);
  box_wireframe(0, 3, 0, 3, 3, 1);
  delay_ms(10000);
}
// Flips a byte 180 degrees.
// MSB becomes LSB, LSB becomes MSB.
word flip4byte (word v){
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
// Flips a byte 180 degrees.
// MSB becomes LSB, LSB becomes MSB.
unsigned char flipbyte (unsigned char byte1){
  char flop = 0x00;

  flop = (flop & 0b11111110) | (0b00000001 & (byte1 >> 7));
  flop = (flop & 0b11111101) | (0b00000010 & (byte1 >> 5));
  flop = (flop & 0b11111011) | (0b00000100 & (byte1 >> 3));
  flop = (flop & 0b11110111) | (0b00001000 & (byte1 >> 1));
  flop = (flop & 0b11101111) | (0b00010000 & (byte1 << 1));
  flop = (flop & 0b11011111) | (0b00100000 & (byte1 << 3));
  flop = (flop & 0b10111111) | (0b01000000 & (byte1 << 5));
  flop = (flop & 0b01111111) | (0b10000000 & (byte1 << 7));
  return flop;
}
// This function validates that we are drawing inside the cube.
unsigned char inrange(int x, int y, int z){
  if (x >= 0 && x < 4 && y >= 0 && y < 4 && z >= 0 && z < 4){
    return 0x01;
  } 
  else{
    // One of the coordinates was outside the cube.
    return 0x00;
  }
}
// Fill a value into all  byts of the cube buffer
// Mostly used for clearing. fill(0x00)
// or setting all on. fill(0xff)
void fill (unsigned char pattern){
  int z;
  int y;
  for (z = 0; z < 4; z++){
    for (y = 0; y < 4; y++){
      cube[z][y] = pattern;
    }
  }
}
// Delay loop.
// This is not calibrated to milliseconds,
// but we had allready made to many effects using this
// calibration when we figured it might be a good idea
// to calibrate it.
void delay_ms(word x){
  uint8_t y, z;
  for ( ; x > 0 ; x--) {
    for ( y = 0 ; y < 90 ; y++) {
      for ( z = 0 ; z < 6 ; z++) {
        asm volatile ("nop");
      }
    }
  }
}
// Set a single voxel to ON
void setvoxel(int x, int y, int z){
  if (inrange(x, y, z))
    cube[z][y] |= (1 << x);
}
// Set a single voxel to OFF
void clrvoxel(int x, int y, int z){
  if (inrange(x, y, z))
    cube[z][y] &= ~(1 << x);
}
// Get the current status of a voxel
unsigned char getvoxel(int x, int y, int z){
  if (inrange(x, y, z)){
    if (cube[z][y] & (1 << x)){
      return 0x01;
    } 
    else{
      return 0x00;
    }
  } 
  else{
    return 0x00;
  }
}
// In some effect we want to just take bool and write it to a voxel
// this function calls the apropriate voxel manipulation function.
void altervoxel(int x, int y, int z, int state){
  if (state == 1)
  {
    setvoxel(x, y, z);
  } else
  {
    clrvoxel(x, y, z);
  }
}
// Flip the state of a voxel.
// If the voxel is 1, its turned into a 0, and vice versa.
void flpvoxel(int x, int y, int z){
  if (inrange(x, y, z))
    cube[z][y] ^= (1 << x);
}
// Makes sure x1 is alwas smaller than x2
// This is usefull for functions that uses for loops,
// to avoid infinite loops
void argorder(int ix1, int ix2, int *ox1, int *ox2){
  if (ix1 > ix2)
  {
    int tmp;
    tmp = ix1;
    ix1 = ix2;
    ix2 = tmp;
  }
  *ox1 = ix1;
  *ox2 = ix2;
}
// Sets all voxels along a X/Y plane at a given point
// on axis Z
void setplane_z (int z){
  int i;
  if (z >= 0 && z < 4)
  {
    for (i = 0; i < 4; i++)
      cube[z][i] = 0xff;
  }
}
// Clears voxels in the same manner as above
void clrplane_z (int z){
  int i;
  if (z >= 0 && z < 4)
  {
    for (i = 0; i < 4; i++)
      cube[z][i] = 0x00;
  }
}
void setplane_x (int x){
  int z;
  int y;
  if (x >= 0 && x < 4)
  {
    for (z = 0; z < 4; z++)
    {
      for (y = 0; y < 4; y++)
      {
        cube[z][y] |= (1 << x);
      }
    }
  }
}
void clrplane_x (int x){
  int z;
  int y;
  if (x >= 0 && x < 4)
  {
    for (z = 0; z < 4; z++)
    {
      for (y = 0; y < 4; y++)
      {
        cube[z][y] &= ~(1 << x);
      }
    }
  }
}
void setplane_y (int y){
  int z;
  if (y >= 0 && y < 4)
  {
    for (z = 0; z < 4; z++)
      cube[z][y] = 0xff;
  }
}
void clrplane_y (int y){
  int z;
  if (y >= 0 && y < 4)
  {
    for (z = 0; z < 4; z++)
      cube[z][y] = 0x00;
  }
}
void setplane (char axis, unsigned char i){
  switch (axis)
  {
    case AXIS_X:
      setplane_x(i);
      break;

    case AXIS_Y:
      setplane_y(i);
      break;

    case AXIS_Z:
      setplane_z(i);
      break;
  }
}
void clrplane (char axis, unsigned char i){
  switch (axis)
  {
    case AXIS_X:
      clrplane_x(i);
      break;

    case AXIS_Y:
      clrplane_y(i);
      break;

    case AXIS_Z:
      clrplane_z(i);
      break;
  }
}
void box_filled(int x1, int y1, int z1, int x2, int y2, int z2){
  int iy;
  int iz;

  argorder(x1, x2, &x1, &x2);
  argorder(y1, y2, &y1, &y2);
  argorder(z1, z2, &z1, &z2);

  for (iz = z1; iz <= z2; iz++)
  {
    for (iy = y1; iy <= y2; iy++)
    {
      cube[iz][iy] |= byteline(x1, x2);
    }
  }

}
// Darw a hollow box with side walls.
void box_walls(int x1, int y1, int z1, int x2, int y2, int z2){
  int iy;
  int iz;

  argorder(x1, x2, &x1, &x2);
  argorder(y1, y2, &y1, &y2);
  argorder(z1, z2, &z1, &z2);

  for (iz = z1; iz <= z2; iz++)
  {
    for (iy = y1; iy <= y2; iy++)
    {
      if (iy == y1 || iy == y2 || iz == z1 || iz == z2)
      {
        cube[iz][iy] = byteline(x1, x2);
      } else
      {
        cube[iz][iy] |= ((0x01 << x1) | (0x01 << x2));
      }
    }
  }
}
// Draw a wireframe box. This only draws the corners and edges,
// no walls.
void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2){
  int iy;
  int iz;

  argorder(x1, x2, &x1, &x2);
  argorder(y1, y2, &y1, &y2);
  argorder(z1, z2, &z1, &z2);

  // Lines along X axis
  cube[z1][y1] = byteline(x1, x2);
  cube[z1][y2] = byteline(x1, x2);
  cube[z2][y1] = byteline(x1, x2);
  cube[z2][y2] = byteline(x1, x2);

  // Lines along Y axis
  for (iy = y1; iy <= y2; iy++)
  {
    setvoxel(x1, iy, z1);
    setvoxel(x1, iy, z2);
    setvoxel(x2, iy, z1);
    setvoxel(x2, iy, z2);
  }

  // Lines along Z axis
  for (iz = z1; iz <= z2; iz++)
  {
    setvoxel(x1, y1, iz);
    setvoxel(x1, y2, iz);
    setvoxel(x2, y1, iz);
    setvoxel(x2, y2, iz);
  }

}
// Returns a byte with a row of 1's drawn in it.
// byteline(2,5) gives 0b00111100
char byteline (int start, int end){
  return ((0xff << start) & ~(0xff << (end + 1)));
}
// Flips a byte 180 degrees.
// MSB becomes LSB, LSB becomes MSB.
char flipbyte (char byte1){
  char flop = 0x00;

  flop = (flop & 0b11111110) | (0b00000001 & (byte1 >> 7));
  flop = (flop & 0b11111101) | (0b00000010 & (byte1 >> 5));
  flop = (flop & 0b11111011) | (0b00000100 & (byte1 >> 3));
  flop = (flop & 0b11110111) | (0b00001000 & (byte1 >> 1));
  flop = (flop & 0b11101111) | (0b00010000 & (byte1 << 1));
  flop = (flop & 0b11011111) | (0b00100000 & (byte1 << 3));
  flop = (flop & 0b10111111) | (0b01000000 & (byte1 << 5));
  flop = (flop & 0b01111111) | (0b10000000 & (byte1 << 7));
  return flop;
}
// Draw a line between any coordinates in 3d space.
// Uses integer values for input, so dont expect smooth animations.
void line(int x1, int y1, int z1, int x2, int y2, int z2){
  float xy; // how many voxels do we move on the y axis for each step on the x axis
  float xz; // how many voxels do we move on the y axis for each step on the x axis
  unsigned char x, y, z;
  unsigned char lasty, lastz;

  // We always want to draw the line from x=0 to x=7.
  // If x1 is bigget than x2, we need to flip all the values.
  if (x1 > x2)
  {
    int tmp;
    tmp = x2; x2 = x1; x1 = tmp;
    tmp = y2; y2 = y1; y1 = tmp;
    tmp = z2; z2 = z1; z1 = tmp;
  }
  if (y1 > y2)
  {
    xy = (float)(y1 - y2) / (float)(x2 - x1);
    lasty = y2;
  } else
  {
    xy = (float)(y2 - y1) / (float)(x2 - x1);
    lasty = y1;
  }

  if (z1 > z2)
  {
    xz = (float)(z1 - z2) / (float)(x2 - x1);
    lastz = z2;
  } else
  {
    xz = (float)(z2 - z1) / (float)(x2 - x1);
    lastz = z1;
  }

  // For each step of x, y increments by:
  for (x = x1; x <= x2; x++)
  {
    y = (xy * (x - x1)) + y1;
    z = (xz * (x - x1)) + z1;
    setvoxel(x, y, z);
  }
}
void line_3d (int x1, int y1, int z1, int x2, int y2, int z2){
  int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc,
  err_1, err_2, dx2, dy2, dz2;
  int pixel[3];
  pixel[0] = x1;
  pixel[1] = y1;
  pixel[2] = z1;
  dx = x2 - x1;
  dy = y2 - y1;
  dz = z2 - z1;
  x_inc = (dx < 0) ? -1 : 1;
  l = abs(dx);
  y_inc = (dy < 0) ? -1 : 1;
  m = abs(dy);
  z_inc = (dz < 0) ? -1 : 1;
  n = abs(dz);
  dx2 = l << 1;
  dy2 = m << 1;
  dz2 = n << 1;
  if ((l >= m) && (l >= n)) {
  err_1 = dy2 - l;
  err_2 = dz2 - l;
  for (i = 0; i < l; i++) {
  //PUT_PIXEL(pixel);
  setvoxel(pixel[0],pixel[1],pixel[2]);
  //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
  if (err_1 > 0) {
  pixel[1] += y_inc;
  err_1 -= dx2;
  }
  if (err_2 > 0) {
  pixel[2] += z_inc;
  err_2 -= dx2;
  }
  err_1 += dy2;
  err_2 += dz2;
  pixel[0] += x_inc;
  }
  } else if ((m >= l) && (m >= n)) {
  err_1 = dx2 - m;
  err_2 = dz2 - m;
  for (i = 0; i < m; i++) {
  //PUT_PIXEL(pixel);
  setvoxel(pixel[0],pixel[1],pixel[2]);
  //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
  if (err_1 > 0) {
  pixel[0] += x_inc;
  err_1 -= dy2;
  }
  if (err_2 > 0) {
  pixel[2] += z_inc;
  err_2 -= dy2;
  }
  err_1 += dx2;
  err_2 += dz2;
  pixel[1] += y_inc;
  }
  } else {
  err_1 = dy2 - n;
  err_2 = dx2 - n;
  for (i = 0; i < n; i++) {
  setvoxel(pixel[0],pixel[1],pixel[2]);
  //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
  //PUT_PIXEL(pixel);
  if (err_1 > 0) {
  pixel[1] += y_inc;
  err_1 -= dz2;
  }
  if (err_2 > 0) {
  pixel[0] += x_inc;
  err_2 -= dz2;
  }
  err_1 += dy2;
  err_2 += dx2;
  pixel[2] += z_inc;
  }
  }
  setvoxel(pixel[0],pixel[1],pixel[2]);
  //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
  //PUT_PIXEL(pixel);
}
void line_3d_float (vertex point1, vertex point2){
  float x1, y1, z1, x2, y2, z2;
  x1 = point1.x;
  y1 = point1.y;
  z1 = point1.z;
  x2 = point2.x;
  y2 = point2.y;
  z2 = point2.z;
  float i;
  float dx, dy, dz, l, m, n, x_inc, y_inc, z_inc,
  err_1, err_2, dx2, dy2, dz2;
  float pixel[3];
  pixel[0] = x1;
  pixel[1] = y1;
  pixel[2] = z1;
  dx = x2 - x1;
  dy = y2 - y1;
  dz = z2 - z1;
  x_inc = (dx < 0) ? -1 : 1;
  l = abs(dx);
  y_inc = (dy < 0) ? -1 : 1;
  m = abs(dy);
  z_inc = (dz < 0) ? -1 : 1;
  n = abs(dz);
  dx2 = l*l;
  dy2 = m*m;
  dz2 = n*n;
  if ((l >= m) && (l >= n)) {
    err_1 = dy2 - l;
    err_2 = dz2 - l;
    for (i = 0; i < l; i++) {
      //PUT_PIXEL(pixel);
      setvoxel((int)pixel[0],(int)pixel[1],(int)pixel[2]);
      //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
      if (err_1 > 0) {
        pixel[1] += y_inc;
        err_1 -= dx2;
      }
      if (err_2 > 0) {
        pixel[2] += z_inc;
        err_2 -= dx2;
      }
      err_1 += dy2;
      err_2 += dz2;
      pixel[0] += x_inc;
    }
  } else if ((m >= l) && (m >= n)) {
    err_1 = dx2 - m;
    err_2 = dz2 - m;
    for (i = 0; i < m; i++) {
      //PUT_PIXEL(pixel);
      //setvoxel(pixel[0]/scale,pixel[1]/scale,pixel[2]/scale);
      setvoxel((int)pixel[0],(int)pixel[1],(int)pixel[2]);
      //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
      if (err_1 > 0) {
        pixel[0] += x_inc;
        err_1 -= dy2;
      }
      if (err_2 > 0) {
        pixel[2] += z_inc;
        err_2 -= dy2;
      }
      err_1 += dx2;
      err_2 += dz2;
      pixel[1] += y_inc;
    }
  } else {
    err_1 = dy2 - n;
    err_2 = dx2 - n;
    for (i = 0; i < n; i++) {
      //setvoxel(pixel[0]/scale,pixel[1]/scale,pixel[2]/scale);
      setvoxel((int)pixel[0],(int)pixel[1],(int)pixel[2]);
      //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
      //PUT_PIXEL(pixel);
      if (err_1 > 0) {
        pixel[1] += y_inc;
        err_1 -= dz2;
      }
      if (err_2 > 0) {
        pixel[0] += x_inc;
        err_2 -= dz2;
      }
      err_1 += dy2;
      err_2 += dx2;
      pixel[2] += z_inc;
    }
  }
  //setvoxel(pixel[0]/scale,pixel[1]/scale,pixel[2]/scale);
      setvoxel((int)pixel[0],(int)pixel[1],(int)pixel[2]);
  //printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
  //PUT_PIXEL(pixel);
}
void flickerOn(int time){
  while(time != 0){
    fill(0xff);
    delay_ms(time);
    fill(0x00);
    delay_ms(time);
    time-= 5;
  }
}
void turnOnAndOffAllByLayerUpAndDownNotTimed(int x){ 
  for(int j = 5; j != 0; j--){
    fill(0x00);
    setplane_z(3);
    for(int i = 3; i>=0; i--){
      setplane_z(i);
      delay_ms(x);
    }
    for(int i = 0; i<4; i++){
      clrplane_z(i);
      delay_ms(x);
    }
    for(int i = 3; i>=0; i--){
      setplane_z(i);
      delay_ms(x);
    }
    for(int i = 0; i>4; i++){
      clrplane_z(i);
      delay_ms(x);
    }
  }
}
//turn everything on and off by column sideways
void turnOnAndOffAllByColumnSideways(int time){
  fill(0x00);
  //turn on layers
  for(int i = 0; i<4; i++){
    box_wireframe(0,0,0,i,0,3);
    delay_ms(time);
  }
  for(int i = 3; i>=0; i--){
    box_wireframe(3,1,0,i,1,3);
    delay_ms(time);
  }
  for(int i = 0; i<4; i++){
    box_wireframe(0,2,0,i,2,3);
    delay_ms(time);
  }
  for(int i = 3; i>=0; i--){
    box_wireframe(3,3,0,i,3,3);
    delay_ms(time);
  }
  fill(0x00);
  for(int i = 0; i<4; i++){
    box_wireframe(0,3,0,i,3,3);
    delay_ms(time);
  }
  for(int i = 3; i>=0; i--){
    box_wireframe(i,2,0,3,2,3);
    delay_ms(time);
  }
  for(int i = 0; i<4; i++){
    box_wireframe(0,1,0,i,1,3);
    delay_ms(time);
  }
  for(int i = 3; i>=0; i--){
    box_wireframe(i,0,0,3,0,3);
    delay_ms(time);
  }
  delay_ms(time);
}
void flickerOff(int time){
  fill(0xff);
  int x=0;
  while(x!= time){
    fill(0x00);
    delay_ms(x+10);
    fill(0xff);
    delay_ms(x);
    x+= 5;
  }
}
void randomflicker(int time){
  byte switch1=random(0,4);
  switch(switch1){
    case 0:
      randomflicker1(time);
      break;
    case 1:
      randomflicker2(time);
      break;
    case 2:
      randomflicker3(time);
      break;
    case 3:
      randomflickerMulti(time);
      break;
  }
}
void randomflicker1(int time1){
  fill(0x00);
  for(int i = 0; i !=400; i++){
    int time=random(20,time1);
    byte ranx = random(0,4);
    byte rany = random(0,4);
    byte ranz = random(0,4);
    setvoxel(ranx,rany,ranz);
    delay_ms(time);
    clrvoxel(ranx,rany,ranz);
    delay_ms(time); 
  }
}
void randomflicker2(int time1){
  fill(0x00);
  for(int i = 0; i !=400; i++){
    int time=random(20,time1);
    byte ranx = random(0,4);
    byte rany = random(0,4);
    byte ranz = random(0,4);
    byte ranx1 = random(0,4);
    byte rany1 = random(0,4);
    byte ranz1 = random(0,4);
    setvoxel(ranx,rany,ranz);
    setvoxel(ranx1,rany1,ranz1);
    delay_ms(time);
    clrvoxel(ranx,rany,ranz);
    clrvoxel(ranx1,rany1,ranz1);
    delay_ms(time); 
  }
}
void randomflicker3(int time1){
  fill(0x00);
  for(int i = 0; i !=400; i++){
    int time=random(20,time1);
    byte ranx = random(0,4);
    byte rany = random(0,4);
    byte ranz = random(0,4);
    byte ranx1 = random(0,4);
    byte rany1 = random(0,4);
    byte ranz1 = random(0,4);
    byte ranx2 = random(0,4);
    byte rany2 = random(0,4);
    byte ranz2 = random(0,4);
    setvoxel(ranx,rany,ranz);
    setvoxel(ranx1,rany1,ranz1);
    setvoxel(ranx2,rany2,ranz2);
    delay_ms(time);
    clrvoxel(ranx,rany,ranz);
    clrvoxel(ranx1,rany1,ranz1);
    clrvoxel(ranx2,rany2,ranz2);
    delay_ms(time); 
  }
}
void randomflickerMulti(int time1){
  fill(0x00);
  for(int i = 0; i !=400; i++){
    int time=random(20,time1);
    byte ranx = random(0,4);
    byte rany = random(0,4);
    byte ranz = random(0,4);
    byte ranx1 = random(0,4);
    byte rany1 = random(0,4);
    byte ranz1 = random(0,4);
    setvoxel(ranx,rany,ranz);
    setvoxel(rany,ranz,ranx);
    setvoxel(ranz,ranx,rany);
    setvoxel(ranx1,rany1,ranz1);
    setvoxel(rany1,ranz1,ranx1);
    setvoxel(ranz1,ranx1,rany1);
    delay_ms(time);
    clrvoxel(ranx,rany,ranz);
    clrvoxel(rany,ranz,ranx);
    clrvoxel(ranz,ranx,rany);
    clrvoxel(ranx1,rany1,ranz1);
    clrvoxel(rany1,ranz1,ranx1);
    clrvoxel(ranz1,ranx1,rany1);
    delay_ms(time); 
  }
}
// Flip the cube 180 degrees along the x axis
void mirror_x (void){
    unsigned char buffer[4][4];
    unsigned char y,z;
    memcpy(buffer, cube, 16); // copy the current cube into a buffer.
    fill(0x00);
    for (z=0; z<4; z++){
        for (y=0; y<4; y++){
            cube[z][y] = flipbyte(buffer[z][y]);
        }
    }
}
// Flip the cube 180 degrees along the y axis.
void mirror_y (void){
    unsigned char buffer[4][4];
    unsigned char x,y,z;

    memcpy(buffer, cube, 16); // copy the current cube into a buffer.

    fill(0x00);
    for (z=0; z<4; z++)
    {
        for (y=0; y<4; y++)
        {
            for (x=0; x<4; x++)
            {
                if (buffer[z][y] & (0x01 << x))
                    setvoxel(x,3-y,z);
            }
        }
    }

}
// flip the cube 180 degrees along the z axis
void mirror_z (void){
    unsigned char buffer[4][4];
    unsigned char z, y;

    memcpy(buffer, cube, 16); // copy the current cube into a buffer.

    for (y=0; y<4; y++)
    {
        for (z=0; z<4; z++)
        {
            cube[3-z][y] = buffer[z][y];
        }
    }
}
void linespin (int iterations, int delay){
  float top_x, top_y, top_z, bot_x, bot_y, bot_z, sin_base;
  float center_x, center_y;
  center_x = 1;
  center_y = 1;
  int i, z;
  for (i=0;i<iterations;i++){
    //printf("Sin base %f \n",sin_base);
    for (z = 0; z < 4; z++){
    sin_base = (float)i/50 + (float)z/(10+(3*sin((float)i/200)));
    top_x = center_x + sin(sin_base)*5;
    top_y = center_x + cos(sin_base)*5;
    //top_z = center_x + cos(sin_base/100)*2.5;
    bot_x = center_x + sin(sin_base+3.14)*10;
    bot_y = center_x + cos(sin_base+3.14)*10;
    //bot_z = 7-top_z;
    bot_z = z;
    top_z = z;
    //setvoxel((int) top_x, (int) top_y, 7);
    //setvoxel((int) bot_x, (int) bot_y, 0);
    //printf("P1: %i %i %i P2: %i %i %i \n", (int) top_x, (int) top_y, 7, (int) bot_x, (int) bot_y, 0);
    //line_3d((int) top_x, (int) top_y, (int) top_z, (int) bot_x, (int) bot_y, (int) bot_z);
    line_3d((int) top_z, (int) top_x, (int) top_y, (int) bot_z, (int) bot_x, (int) bot_y);
    }
    delay_ms(120);
    fill(0x00);
  }
}
void sinelines (int iterations, int delay){
  int i,x;
  float left, right, sine_base, x_dividor,ripple_height;
  for (i=0; i<iterations; i++){
    for (x=0; x<4 ;x++){
      x_dividor = 1 + sin((float)i/100)+1;
      ripple_height = 2 + (sin((float)i/200)+1)*6;
      sine_base = (float) i/40 + (float) x/x_dividor;
      left = 2 + sin(sine_base)*ripple_height;
      right = 2 + cos(sine_base)*ripple_height;
      right = 3-left;
      //printf("%i %i \n", (int) left, (int) right);
      line_3d(0-1, x, (int) left, 3+1, x, (int) right);
      //line_3d((int) right, 7, x);
    }
  delay_ms(delay);
  fill(0x00);
  }
}
// Display a sine wave running out from the center of the cube.
void ripples (int iterations, int delay){
  float origin_x, origin_y, distance, height, ripple_interval;
  int x,y,i;
  fill(0x00);
  for (i=0;i<iterations;i++){
    for (x=0;x<4;x++){
      for (y=0;y<8;y++){
        distance = distance2d(3.5,3.5,x,y)/9.899495*8;
        //distance = distance2d(3.5,3.5,x,y);
        ripple_interval =1.3;
        height = 4+sin(distance/ripple_interval+(float) i/50)*4;
        setvoxel(x,y,(int) height); 
      }
    }
    delay_ms(delay);
    fill(0x00);
  }
}
void sidewaves (int iterations, int delay){
  float origin_x, origin_y, distance, height, ripple_interval;
  int x,y,i;
  fill(0x00);
  for (i=0;i<iterations;i++){
    origin_x = 3.5+sin((float)i/500)*4;
    origin_y = 3.5+cos((float)i/500)*4;
    for (x=0;x<8;x++){
      for (y=0;y<8;y++){
        distance = distance2d(origin_x,origin_y,x,y)/9.899495*8;
        ripple_interval =2;
        height = 4+sin(distance/ripple_interval+(float) i/50)*3.6;
        setvoxel(x,y,(int) height);
        setvoxel(x,y,(int) height);
      }
    }
    delay_ms(delay);
    fill(0x00);
  }
}
void spheremove (int iterations, int delay){
  fill(0x00);
  float origin_x, origin_y, origin_z, distance, diameter;
  origin_x = 0;
  origin_y = 3.5;
  origin_z = 3.5;
  diameter = 3;
  int x, y, z, i;
  for (i=0; i<iterations; i++){
    origin_x = 0+sin((float)i/50)*2.5;
    origin_y = 0+cos((float)i/50)*2.5;
    origin_z = 1+cos((float)i/30)*2;
    diameter = 0+sin((float)i/150);
    for (x=0; x<4; x++){
      for (y=0; y<4; y++){
        for (z=0; z<4; z++){
          distance = distance3d(x,y,z, origin_x, origin_y, origin_z);
          //printf("Distance: %f \n", distance);
          if (distance>diameter && distance<diameter+1){
            setvoxel(x,y,z);
          }
        }
      }
    }
    delay_ms(delay);
    fill(0x00);
  }
}
vertex point_rotate_around_point (vertex point, vertex center, float rotation_x, float rotation_y, float rotation_z){
  float x, y, z;
  float sx,cx, sy,cy, sz,cz;
  float xy,xz, yx,yz, zx,zy;
  vertex newpoint;
  // Center all the points around 0,0,0
  x = point.x - center.x;
  y = point.y - center.y;
  z = point.z - center.z;
  // Precalculate sinus and cosinus for each axis rotation
  sx = sin(rotation_x);
  cx = cos(rotation_x);
  sy = sin(rotation_y);
  cy = cos(rotation_y);
  sz = sin(rotation_z);
  cz = cos(rotation_z);
  // Rotation around x
  xy = cx*y - sx*z;
  xz = sx*y + cx*z;
  // Rotation around y
  yz = cy*xz - sy*x;
  yx = sy*xz + cy*x;
  // Rotation around z
  zx = cz*yx - sz*xy;
  zy = sz*yx + cz*xy;
  newpoint.x = zx + center.x;
  newpoint.y = zy + center.y;
  newpoint.z = yz + center.z;
  return newpoint;
}
void effect_rotate_random_pixels (int iterations, int delay, int pixels){
  vertex points[pixels];
  vertex rotated[pixels];
  float fy, fx, fz;
  int x,y,z;
  int i,p;
  float rot_x = 0;
  float rot_y = 0;
  float rot_z = 0;
  vertex cube_center = {1.5, 1.5, 1.5};
  for (i=0; i<pixels; i++){
    x = rand()%800-200;  
    y = rand()%800-200;  
    z = rand()%800-200;
    fx = (float)x/100;
    fy = (float)y/100;
    fz = (float)z/100;
    points[i].x = fx;
    points[i].y = fy;
    points[i].z = fz;
    setvoxel((int)points[i].x, (int)points[i].y, (int)points[i].z);
    delay_ms(100);
  }
  delay_ms(10000);
  for (i=0; i<iterations; i++){
    rot_x = (float)i/75;
    rot_y = (float)i/150;
    rot_z = (float)i/200;
    for (p=0; p<pixels; p++){
      rotated[p] = point_rotate_around_point (points[p], cube_center, rot_x, rot_y, rot_z);
    }
    fill(0x00);
    for (p=0; p<pixels; p++){
      setvoxel((int)rotated[p].x, (int)rotated[p].y, (int)rotated[p].z);
    }
    delay_ms(delay);
  }
  fill(0x00);
}
// Calculate all 4 corners of a cube.
void calculate_cube_corners (vertex pnt[4], vertex center, float size1){
  // Distance from center on any axis.
  float dist = size1/2;
  // Points
  // X        Y       Z
  pnt[0].x = center.x+dist; pnt[0].y = center.y+dist; pnt[0].z = center.z+dist; // 0 right, front, upper
  pnt[1].x = center.x-dist; pnt[1].y = center.y+dist; pnt[1].z = center.z+dist; // 1 left, front, upper
  pnt[2].x = center.x+dist; pnt[2].y = center.y-dist; pnt[2].z = center.z+dist; // 2 right, back, upper
  pnt[3].x = center.x-dist; pnt[3].y = center.y-dist; pnt[3].z = center.z+dist; // 3 left, back, uppper
}
void effect_blinky2(){
  int i,r;
  fill(0x00);
  for (r=0;r<2;r++){
    i = 750;
    while (i>0){
      fill(0x00);
      delay_ms(i);
      fill(0xff);
      delay_ms(100);
      i = i - (15+(1000/(i/10)));
    }
    delay_ms(1000);
    i = 750;
    while (i>0){
      fill(0x00);
      delay_ms(751-i);
      fill(0xff);
      delay_ms(100);
      i = i - (15+(1000/(i/10)));
    }
  }
}
void effect_box_woopwoop (int iteration,int delay){
  int i;
  for(int j=0;j<iteration;j++){
    fill(0x00);
    for (i=0;i<2;i++){
      box_wireframe(2+i,2+i,2+i,1-i,1-i,1-i);
      delay_ms(delay);
      fill(0x00);
    }
  } 
}
// Send a voxel flying from one side of the cube to the other
// It its at the bottom, send it to the top..
void sendvoxel_z (unsigned char x, unsigned char y, unsigned char z, int delay){
  int i, ii;
  for (i=0; i<4; i++){
    if (z == 3){
      ii = 3-i;
      clrvoxel(x,y,ii+1);
    } 
    else{
      ii = i;
      clrvoxel(x,y,ii-1);
    }
    setvoxel(x,y,ii);
    delay_ms(delay);
  }
}
// Send all the voxels from one side of the cube to the other
// Start at z and send to the opposite side.
// Sends in random order.
void sendplane_rand_z (unsigned char z, int delay, int wait){
  unsigned char loop = 16;
  unsigned char x, y;
  fill(0x00);
  setplane_z(z);
  // Send voxels at random untill all 16 have crossed the cube.
  while(loop){
    x = rand()%4;
    y = rand()%4;
    if (getvoxel(x,y,z)){
      // Send the voxel flying
      sendvoxel_z(x,y,z,delay);
      delay_ms(wait);
      loop--; // one down, loop-- to go. when this hits 0, the loop exits. 
    }
  }
}
// Set or clear exactly 64 voxels in a random order.
void effect_random_filler (int delay, int state){
  int x,y,z;
  int loop = 0;
  if (state == 1){
    fill(0x00);
  } else{
    fill(0xff);
  }
  while (loop<64){
    x = rand()%4;
    y = rand()%4;
    z = rand()%4;
    if ((state == 0 && getvoxel(x,y,z) == 0x01) || (state == 1 && getvoxel(x,y,z) == 0x00)){
      altervoxel(x,y,z,state);
      delay_ms(delay);
      loop++;
    } 
  }
}
void effect_rain (int iterations,int time){ 
  int i, ii;
  int rnd_x;
  int rnd_y;
  int rnd_num;
  for (ii=0;ii<iterations;ii++){
    rnd_num = rand()%2;
    for (i=0; i < rnd_num;i++){
      rnd_x = rand()%4;
      rnd_y = rand()%4;
      setvoxel(rnd_x,rnd_y,3);
    }
    delay_ms(random(350,800)-time);
    shift(AXIS_Z,-1);
  }
}
void effect_z_updown (int iterations, int delay){
  unsigned char positions[16];
  unsigned char destinations[16];
  int i,y,move,px;
  for (i=0; i<16; i++){
    positions[i] = 2;
    destinations[i] = rand()%4;
  }
  for (i=0; i<4; i++){
    effect_z_updown_move(positions, destinations, AXIS_Z);
    delay_ms(delay);
  }
  for (i=0;i<iterations;i++){
    for (move=0;move<4;move++){
      effect_z_updown_move(positions, destinations, AXIS_Z);
      delay_ms(delay);
    }
    delay_ms(delay*2);
    for (y=0;y<10;y++){
        destinations[rand()%16] = rand()%4;
    }
  }
}
void effect_z_updown_move (unsigned char positions[16], unsigned char destinations[16], char axis){
  int px;
  for (px=0; px<16; px++){
    if (positions[px]<destinations[px]){
      positions[px]++;
    }
    if (positions[px]>destinations[px]){
      positions[px]--;
    }
  }
  draw_positions_axis (AXIS_Z, positions,0);
}
void effect_axis_updown_randsuspend (char axis, int delay, int sleep, int invert){
  unsigned char positions[16];
  unsigned char destinations[16];
  int i,px;
  for (i=0; i<16; i++){
    positions[i] = 0;
    destinations[i] = rand()%4;
  }
  for (i=0; i<4; i++){
    for (px=0; px<16; px++){
      if (positions[px]<destinations[px]){
        positions[px]++;
      }
      if (positions[px]>destinations[px]){
        positions[px]--;
      }
    }
    draw_positions_axis (axis, positions,invert);
    delay_ms(delay);
  }
  for (i=0; i<16; i++){
    destinations[i] = 3;
  }
  delay_ms(sleep);
  for (i=0; i<4; i++){
    for (px=0; px<16; px++){
      if (positions[px]<destinations[px]){
        positions[px]++;
      }
      if (positions[px]>destinations[px]){
        positions[px]--;
      }
    }
    draw_positions_axis (axis, positions,invert);
    delay_ms(delay);
  }
}
void draw_positions_axis (char axis, unsigned char positions[16], int invert){
  int x, y, p;
  fill(0x00);
  for (x=0; x<4; x++){
    for (y=0; y<4; y++){
      if (invert){
        p = (3-positions[(x*4)+y]);
      } 
      else{
        p = positions[(x*4)+y];
      }
      if (axis == AXIS_Z)
        setvoxel(x,y,p); 
      if (axis == AXIS_Y)
        setvoxel(x,p,y);
      if (axis == AXIS_X)
        setvoxel(p,y,x);
    }
  } 
}
void effect_boxside_randsend_parallel (char axis, int origin, int delay, int mode){
  int i;
  int done;
  unsigned char cubepos[64];
  unsigned char pos[64];
  int notdone = 1;
  int notdone2 = 1;
  int sent = 0;
  for (i=0;i<64;i++){
    pos[i] = 0;
  }
  while (notdone){
    if (mode == 1){
      notdone2 = 1;
      while (notdone2 && sent<64){
        i = rand()%64;
        if (pos[i] == 0){
          sent++;
          pos[i] += 1;
          notdone2 = 0;
        }
      }
    } 
    else if (mode == 2){
      if (sent<64){
        pos[sent] += 1;
        sent++;
      }
    }
    done = 0;
    for (i=0;i<64;i++){
      if (pos[i] > 0 && pos[i] <7){
        pos[i] += 1;
      }
      if (pos[i] == 7)
        done++;
    }
    if (done == 64)
      notdone = 0;
    for (i=0;i<64;i++){
      if (origin == 0){
        cubepos[i] = pos[i];
      } 
      else{
        cubepos[i] = (7-pos[i]);
      }
    }
    delay_ms(delay);
    draw_positions_axis(axis,cubepos,0);
    //LED_PORT ^= LED_RED;
  }
}
// Light all leds layer by layer,
// then unset layer by layer
void effect_loadbar(int delay){
  fill(0x00);
  int z,y;
  for (z=0;z<4;z++){
    for (y=0;y<4;y++)
      cube[z][y] = 0xff;
    delay_ms(delay);
  }
  delay_ms(delay*3);
  for (z=0;z<4;z++){
    for (y=0;y<4;y++)
      cube[z][y] = 0x00;
    delay_ms(delay);
  }
}
// Set n number of voxels at random positions
void effect_random_sparkle_flash (int iterations, int voxels, int delay){
  int i;
  int v;
  for (i = 0; i < iterations; i++){
    for (v=0;v<=voxels;v++)
      setvoxel(rand()%4,rand()%4,rand()%4);
    delay_ms(delay);
    fill(0x00);
  }
}
// blink 1 random voxel, blink 2 random voxels..... blink 20 random voxels
// and back to 1 again.
void effect_random_sparkle (void){
  int i;
  for (i=1;i<20;i++){
    effect_random_sparkle_flash(5,i,200);
  }
  for (i=20;i>=1;i--){
    effect_random_sparkle_flash(5,i,200);
  }
}
int effect_telcstairs_do(int x, int val, int delay){
  int y,z;
  for(y = 0, z = x; y <= z; y++, x--){
    if(x < 4 && y < 4){
      cube[x][y] = val;
    }
  }
  delay_ms(delay);
  return z;
}
int effect_telcstairs_do_clear(int x, int val, int delay){
  int y,z;
  for(y = 0, z = x; y <= z; y++, x--){
    if(x < 4 && y < 4){
      cube[x][y] = ~val;
    }
  }
  delay_ms(delay);
  return z;
}
void effect_telcstairs (int invert, int delay, int val){
  int x;
  if(invert==1){
    for(x = 4*2; x >= 0; x--){
      x = effect_telcstairs_do(x,val,delay);
    }
  }
  if(invert==2){
    for(x = 0; x < 4*2; x++){
      x = effect_telcstairs_do(x,val,delay);
    }
  }
  if(invert==3){
    for(x = 4*2; x >= 0; x--){
      x = effect_telcstairs_do_clear(x,val,delay);
    }
  }
  if(invert==4){
    for(x = 0; x < 4*2; x++){
      x = effect_telcstairs_do_clear(x,val,delay);
    }
  }
}
void effect_wormsqueeze (int size, int axis, int direction, int iterations, int delay){
  int x, y, i,j,k, dx, dy;
  int cube_size;
  int origin = 0;
  if (direction == -1)
    origin = 3;
  cube_size = 4-(size-1);
  x = rand()%cube_size;
  y = rand()%cube_size;
  for (i=0; i<iterations; i++){
    dx = ((rand()%3)-1);
    dy = ((rand()%3)-1);
    if ((x+dx) > 0 && (x+dx) < cube_size)
      x += dx;
    if ((y+dy) > 0 && (y+dy) < cube_size)
      y += dy;
    shift(axis, direction);
    for (j=0; j<size;j++){
      for (k=0; k<size;k++){
        if (axis == AXIS_Z)
          setvoxel(x+j,y+k,origin);
        if (axis == AXIS_Y)
          setvoxel(x+j,origin,y+k);
        if (axis == AXIS_X)
          setvoxel(origin,y+j,x+k);
      }
    }
    delay_ms(delay);
  }
}
void draw_cube_wireframe (vertex pnt[8]){
  // upper "lid"
  line_3d ((int)pnt[0].x,(int)pnt[0].y,(int)pnt[0].z,(int)pnt[1].x,(int)pnt[1].y,(int)pnt[1].z);
  line_3d ((int)pnt[2].x,(int)pnt[2].y,(int)pnt[2].z,(int)pnt[3].x,(int)pnt[3].y,(int)pnt[3].z);
  line_3d ((int)pnt[1].x,(int)pnt[1].y,(int)pnt[1].z,(int)pnt[3].x,(int)pnt[3].y,(int)pnt[3].z);
  line_3d ((int)pnt[2].x,(int)pnt[2].y,(int)pnt[2].z,(int)pnt[0].x,(int)pnt[0].y,(int)pnt[0].z);

  // lower "lid"
  line_3d ((int)pnt[4].x,(int)pnt[4].y,(int)pnt[4].z,(int)pnt[5].x,(int)pnt[5].y,(int)pnt[5].z);
  line_3d ((int)pnt[6].x,(int)pnt[6].y,(int)pnt[6].z,(int)pnt[7].x,(int)pnt[7].y,(int)pnt[7].z);
  line_3d ((int)pnt[5].x,(int)pnt[5].y,(int)pnt[5].z,(int)pnt[7].x,(int)pnt[7].y,(int)pnt[7].z);
  line_3d ((int)pnt[6].x,(int)pnt[6].y,(int)pnt[6].z,(int)pnt[4].x,(int)pnt[4].y,(int)pnt[4].z);

  // side walls
  line_3d ((int)pnt[0].x,(int)pnt[0].y,(int)pnt[0].z,(int)pnt[4].x,(int)pnt[4].y,(int)pnt[4].z);
  line_3d ((int)pnt[1].x,(int)pnt[1].y,(int)pnt[1].z,(int)pnt[5].x,(int)pnt[5].y,(int)pnt[5].z);
  line_3d ((int)pnt[2].x,(int)pnt[2].y,(int)pnt[2].z,(int)pnt[6].x,(int)pnt[6].y,(int)pnt[6].z);
  line_3d ((int)pnt[3].x,(int)pnt[3].y,(int)pnt[3].z,(int)pnt[7].x,(int)pnt[7].y,(int)pnt[7].z);
}
// Big ugly function :p but it looks pretty
void boingboing(uint16_t iterations, int delay, unsigned char mode, unsigned char drawmode){
  fill(0x00);   // Blank the cube
  int x, y, z;    // Current coordinates for the point
  int dx, dy, dz; // Direction of movement
  int lol, i;   // lol?
  unsigned char crash_x, crash_y, crash_z;
  // Coordinate array for the snake.
  int snake[4][3];
  for (i=0;i<4;i++){
    snake[i][0] = 0;
    snake[i][1] = 0;
    snake[i][2] = 0;
  }
  x = rand()%4;
  y = rand()%4;
  z = rand()%4;
  dx = 1;
  dy = 1;
  dz = 1;
  while(iterations){
    crash_x = 0;
    crash_y = 0;
    crash_z = 0;
    // Let's mix things up a little:
    if (rand()%3 == 0){
      // Pick a random axis, and set the speed to a random number.
      lol = rand()%3;
      if (lol == 0)
        dx = rand()%3 - 1;
      if (lol == 1)
        dy = rand()%3 - 1;
      if (lol == 2)
        dz = rand()%3 - 1;
    }
    if (dx == -1 && x == 0){
      crash_x = 0x01;
      if (rand()%3 == 1)
      {
        dx = 1;
      } else
      {
        dx = 0;
      }
    }  
    if (dy == -1 && y == 0){
      crash_y = 0x01;
      if (rand()%3 == 1)
      {
        dy = 1;
      } else
      {
        dy = 0;
      }
    }
    if (dz == -1 && z == 0){
      crash_z = 0x01;
      if (rand()%3 == 1)
      {
        dz = 1;
      } else
      {
        dz = 0;
      }
    } 
    if (dx == 1 && x == 3){
      crash_x = 0x01;
      if (rand()%3 == 1)
      {
        dx = -1;
      } else
      {
        dx = 0;
      }
    }   
    if (dy == 1 && y == 3){
      crash_y = 0x01;
      if (rand()%3 == 1)
      {
        dy = -1;
      } else
      {
        dy = 0;
      }
    }   
    if (dz == 1 && z == 3){
      crash_z = 0x01;
      if (rand()%3 == 1)
      {
        dz = -1;
      } else
      {
        dz = 0;
      }
    }
    // mode bit 0 sets crash action enable
    if (mode | 0x01){
      if (crash_x){
        if (dy == 0)
        {
          if (y == 3)
          {
            dy = -1;
          } else if (y == 0)
          {
            dy = +1;
          } else
          {
            if (rand()%2 == 0)
            {
              dy = -1;
            } else
            {
              dy = 1;
            }
          }
        }
        if (dz == 0)
        {
          if (z == 3)
          {
            dz = -1;
          } else if (z == 0)
          {
            dz = 1;
          } else
          {
            if (rand()%2 == 0)
            {
              dz = -1;
            } else
            {
              dz = 1;
            }
          } 
        }
      }   
      if (crash_y){
        if (dx == 0)
        {
          if (x == 3)
          {
            dx = -1;
          } else if (x == 0)
          {
            dx = 1;
          } else
          {
            if (rand()%2 == 0)
            {
              dx = -1;
            } else
            {
              dx = 1;
            }
          }
        }
        if (dz == 0)
        {
          if (z == 3)
          {
            dz = -1;
          } else if (z == 0)
          {
            dz = 1;
          } else
          {
            if (rand()%2 == 0)
            {
              dz = -1;
            } else
            {
              dz = 1;
            }
          } 
        }
      }
      if (crash_z){
        if (dy == 0)
        {
          if (y == 3)
          {
            dy = -1;
          } else if (y == 0)
          {
            dy = 1;
          } else
          {
            if (rand()%2 == 0)
            {
              dy = -1;
            } else
            {
              dy = 1;
            }
          } 
        }
        if (dx == 0)
        {
          if (x == 3)
          {
            dx = -1;
          } else if (x == 0)
          {
            dx = 1;
          } else
          {
            if (rand()%2 == 0)
            {
              dx = -1;
            } else
            {
              dx = 1;
            }
          } 
        }
      }
    }
    // mode bit 1 sets corner avoid enable
    if (mode | 0x02){
      if (  // We are in one of 8 corner positions
        (x == 0 && y == 0 && z == 0) ||
        (x == 0 && y == 0 && z == 3) ||
        (x == 0 && y == 3 && z == 0) ||
        (x == 0 && y == 3 && z == 3) ||
        (x == 3 && y == 0 && z == 0) ||
        (x == 3 && y == 0 && z == 3) ||
        (x == 3 && y == 3 && z == 0) ||
        (x == 3 && y == 3 && z == 3)
      )
      {
        // At this point, the voxel would bounce
        // back and forth between this corner,
        // and the exact opposite corner
        // We don't want that!
      
        // So we alter the trajectory a bit,
        // to avoid corner stickyness
        lol = rand()%3;
        if (lol == 0)
          dx = 0;   
        if (lol == 1)
          dy = 0;
        if (lol == 2)
          dz = 0;
      }
    }
    // Finally, move the voxel.
    x = x + dx;
    y = y + dy;
    z = z + dz;
    if (drawmode == 0x01){ // show one voxel at time
      setvoxel(x,y,z);
      delay_ms(delay);
      clrvoxel(x,y,z);  
    } 
    else if (drawmode == 0x02){ // flip the voxel in question
      flpvoxel(x,y,z);
      delay_ms(delay);
    } 
    if (drawmode == 0x03){
      for (i=3;i>=0;i--)
      {
        snake[i][0] = snake[i-1][0];
        snake[i][1] = snake[i-1][1];
        snake[i][2] = snake[i-1][2];
      }
      snake[0][0] = x;
      snake[0][1] = y;
      snake[0][2] = z;
        
      for (i=0;i<4;i++)
      {
        setvoxel(snake[i][0],snake[i][1],snake[i][2]);
      }
      delay_ms(delay);
      for (i=0;i<4;i++)
      {
        clrvoxel(snake[i][0],snake[i][1],snake[i][2]);
      }
    }
    iterations--;
  }
}
void aroundEdgeDown(int time){
  fill(0x00);
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      setvoxel(j,0,i);
      delay_ms(time);
      clrvoxel(j,0,i);
      delay_ms(time);
    }
    
    for(int j=0;j<4;j++){
      setvoxel(3,j,i);
      delay_ms(time);
      clrvoxel(3,j,i);
      delay_ms(time);
    }
    for(int j=3;j>=0;j--){
      setvoxel(j,3,i);
      delay_ms(time);
      clrvoxel(j,3,i);
      delay_ms(time);
    }
    for(int j=3;j>=0;j--){
      setvoxel(0,j,i);
      delay_ms(time);
      clrvoxel(0,j,i);
      delay_ms(time);
    }
  }

  for(int i=3;i>=0;i--){
     for(int j=1;j<3;j++){
      setvoxel(1,j,i);
      delay_ms(time);
      clrvoxel(1,j,i);
      delay_ms(time);
    }
    for(int j=1;j<3;j++){
      setvoxel(j,2,i);
      delay_ms(time);
      clrvoxel(j,2,i);
      delay_ms(time);
    }
    for(int j=2;j>=1;j--){
      setvoxel(2,j,i);
      delay_ms(time);
      clrvoxel(2,j,i);
      delay_ms(time);
    }
    for(int j=2;j>=1;j--){
      setvoxel(j,1,i);
      delay_ms(time);
      clrvoxel(j,1,i);
      delay_ms(time);
    }
  }
}
void hurricane (void){
  hurricane1();
  delay_ms(700);
  hurricane2();
  delay_ms(500);
}
void hurricane1(void){
fill(0x00);
    for(int i=0;i<4;i++){
      setvoxel(1,0,i);
      setvoxel(3,1,i);
      setvoxel(2,3,i);
      setvoxel(0,2,i);
    }
}
void hurricane2(void){
  fill(0x00);
    for(int i=0;i<4;i++){
      setvoxel(2,0,i);
      setvoxel(3,2,i);
      setvoxel(1,3,i);
      setvoxel(0,1,i);
    }
}
void randomBoxWireframe(int iteration, int time){
  fill(0x00);
  for(byte i=0;i<iteration;i++){
    fill(0x00);
    box_wireframe(rand()%4,rand()%4,rand()%4,rand()%4,rand()%4,rand()%4);
    delay_ms(time);
  }
  for(byte i=0;i<iteration;i++){
    fill(0x00);
    box_wireframe(1+random(-1,2),1+random(-1,2),1+random(-1,2),2+random(-1,2),2+random(-1,2),2+random(-1,2));
    delay_ms(time);
  }
}
void boxInSpace(int iteration,int time,int noofboxes){
  int x=rand()%3, y=rand()%3, z=rand()%3;    // Current coordinates for the point
  int dx, dy, dz; // Direction of movement
  switch(noofboxes){
    case 1: 
      for(int i=0;i<iteration;i++){
        fill(0x00);
        x=rand()%3, y=rand()%3, z=rand()%3;
        box_wireframe(x,y,z,x+1,y+1,z+1);
        delay_ms(time);
      }
      break;
    case 2:
      for(int i=0;i<iteration;i++){
        fill(0x00);
        x=rand()%2, y=rand()%2, z=rand()%2; 
        box_wireframe(x,y,z,x+2,y+2,z+2);
        delay_ms(time);
      }
      break;
  } 
}
void propeller(int time,int mode){
  switch(mode){
  case 1:
    for(int y=3; y>=0; y--){
      //turn on layer
      //a1
      fill(0x00);
      setvoxel(0,0,y);
      setvoxel(1,1,y);
      setvoxel(2,2,y);
      setvoxel(3,3,y);
      delay_ms(time);
      //b1
      fill(0x00);
      setvoxel(0,1,y);
      setvoxel(1,1,y);
      setvoxel(2,2,y);
      setvoxel(3,2,y);
      delay_ms(time);
      //c1
      fill(0x00);
      setvoxel(2,1,y);
      setvoxel(3,1,y);
      setvoxel(0,2,y);
      setvoxel(1,2,y);
      delay_ms(time);
      //d1
      fill(0x00);
      setvoxel(3,0,y);
      setvoxel(2,1,y);
      setvoxel(1,2,y);
      setvoxel(0,3,y);
      delay_ms(time);
      //d2
      fill(0x00);
      setvoxel(2,0,y);
      setvoxel(2,1,y);
      setvoxel(1,2,y);
      setvoxel(1,3,y);
      delay_ms(time);
      //d3
      fill(0x00);
      setvoxel(1,0,y);
      setvoxel(1,1,y);
      setvoxel(2,2,y);
      setvoxel(2,3,y);
      delay_ms(time);
    }
    //d4
    fill(0x00);
    for(int i=0;i<4;i++){
      setvoxel(0,0,i);
      setvoxel(1,1,i);
      setvoxel(2,2,i);
      setvoxel(3,3,i);
      delay_ms(time);
    }
    break;
    case 2:
    for(int y=3; y>=0; y--){
      //turn on layer
      //a1
      setvoxel(0,0,y);
      setvoxel(1,1,y);
      setvoxel(2,2,y);
      setvoxel(3,3,y);
    }
    delay_ms(time);
    fill(0x00);
    for(int y=3; y>=0; y--){
      //b1
      setvoxel(0,1,y);
      setvoxel(1,1,y);
      setvoxel(2,2,y);
      setvoxel(3,2,y);
    }
    delay_ms(time);
      //c1
    fill(0x00);
    for(int y=3; y>=0; y--){
      setvoxel(2,1,y);
      setvoxel(3,1,y);
      setvoxel(0,2,y);
      setvoxel(1,2,y);
    }
    delay_ms(time);
    fill(0x00);
    for(int y=3; y>=0; y--){
      //d1
      setvoxel(3,0,y);
      setvoxel(2,1,y);
      setvoxel(1,2,y);
      setvoxel(0,3,y);
    }
    delay_ms(time);
    fill(0x00);
    for(int y=3; y>=0; y--){
      //d2
      setvoxel(2,0,y);
      setvoxel(2,1,y);
      setvoxel(1,2,y);
      setvoxel(1,3,y);
    }
    delay_ms(time);
    fill(0x00);
    for(int y=3; y>=0; y--){
      //d3
      setvoxel(1,0,y);
      setvoxel(1,1,y);
      setvoxel(2,2,y);
      setvoxel(2,3,y);
    }
    delay_ms(time);
    break;
  }
}






