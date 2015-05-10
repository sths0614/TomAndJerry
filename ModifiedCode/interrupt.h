#include "Arduino.h"

volatile static int numTicks = 0;

void gotTick(){
  numTicks++;
  Serial.println("Got a tick");
}

int getCurrentTicks(){
  return numTicks;
}
