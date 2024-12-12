#include "web.h"
HTML510Server h(80);

void handleRoot(){
  h.sendhtml(body);
}

void getSpeed(){
  robot.userSpeed = h.getVal();
}

void press(){
  robot.state = h.getVal();
  switch (robot.state) {
    case 0:
      Serial.println("Pressed Nothing (ERROR)");
      break;
    case 1:
      Serial.println("Pressed Forward");
      break;
    case 2:
      Serial.println("Pressed Backward");
      break;
    case 3:
      Serial.println("Pressed Left");
      break;
    case 4:
      Serial.println("Pressed Right");
      break;
    default:
      Serial.printf("invalid release state %d\n", state); 
      break;
  }
}

void release(){
  int val = h.getVal();
  robot.state = 0;
  switch (val) {
    case 0:
      Serial.println("Released Nothing (ERROR)");
      break;
    case 1:
      Serial.println("Released Forward");
      break;
    case 2:
      Serial.println("Released Backward");
      break;
    case 3:
      Serial.println("Released Left");
      break;
    case 4:
      Serial.println("Released Right");
      break;
    default:
      Serial.printf("invalid release state %d\n", state); 
      break;
  }
}