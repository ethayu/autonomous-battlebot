#include "web.h"
#include "config.h"
HTML510Server h(80);

void handleRoot()
{
  h.sendhtml(body);
}

void getSpeed()
{
  robot.userSpeed = h.getVal();
}

void startAttack() 
{
  robot.attacking = true;
  robot.attackingTime = millis();
  robot.sensors.usedWifi += 1;
}

void stopAttack() 
{
  robot.attacking = false;
  robot.sensors.usedWifi += 1;
}

void press()
{
  int direction = h.getVal();
  // robot.activeKeys = std::set<int>();
  // String res = h.getText();
  // for (int i = 0; i < res.length(); i++) {
  //   char currentChar = res.charAt(i); 
  //   if (currentChar == 'w') {
  //     robot.activeKeys.insert(1);
  //   } else if (currentChar == 'a') {
  //     robot.activeKeys.insert(3);
  //   } else if (currentChar == 's') {
  //     robot.activeKeys.insert(2);
  //   } else if (currentChar == 'd') {
  //     robot.activeKeys.insert(4);
  //   }
  // }
  robot.activeKeys.insert(direction);
  robot.state = 1;
  robot.sensors.usedWifi += 1;

  // #ifdef DEBUG
  // Serial.print("Set contents after press request: ");
  // for (const int &element : robot.activeKeys) {
  //     Serial.print(element);
  //     Serial.print(" "); // Space between elements
  // }
  // Serial.println(); // Newline at the end
  // #endif
}

void release()
{
  // robot.activeKeys = std::set<int>();
  // String res = h.getText();
  // for (int i = 0; i < res.length(); i++) {
  //   char currentChar = res.charAt(i); 
  //   if (currentChar == 'w') {
  //     robot.activeKeys.insert(1);
  //   } else if (currentChar == 'a') {
  //     robot.activeKeys.insert(3);
  //   } else if (currentChar == 's') {
  //     robot.activeKeys.insert(2);
  //   } else if (currentChar == 'd') {
  //     robot.activeKeys.insert(4);
  //   }
  // }
  int direction = h.getVal();
  robot.activeKeys.erase(direction);
  robot.state = 0;
  robot.sensors.usedWifi += 1;
}

void nav()
{
  String res = h.getText();

  int x = 0, y = 0, angle = 0;

  int firstComma = res.indexOf(',');
  int secondComma = res.indexOf(',', firstComma + 1);

  if (firstComma != -1 && secondComma != -1)
  {
    x = res.substring(0, firstComma).toInt();
    y = res.substring(firstComma + 1, secondComma).toInt();
    angle = res.substring(secondComma + 1).toInt();
    robot.target.setPoint(x, y);
    robot.target_bearing = angle * PI / 180;
    robot.state = 2;
  robot.sensors.usedWifi += 1;
    robot.substate = 0;
  }
  else
  {
    Serial.println("Error: Invalid input format. Expected 'x,y,angle'.");
  }
}

void attackClosest()
{
  robot.target_bearing = robot.bearing;
  robot.state = 3;
  robot.attacking = false;
  robot.substate = 0;
  robot.sensors.usedWifi += 1;
}

void attackStructure()
{
 robot.state = 4;
 robot.attacking = false;
 robot.substate = 0; 
 int structure = h.getVal();
 switch (structure) {
  case 0: {
    robot.target.setPoint(RED_NEXUS);
    robot.target_bearing = LOOK_AT_RED_NEXUS;
    break;
  }
  case 1: {
    robot.target.setPoint(BLUE_NEXUS);
    robot.target_bearing = LOOK_AT_BLUE_NEXUS;
    break;
  }
  case 2: {
    robot.target.setPoint(RED_TOWER);
    robot.target_bearing = LOOK_AT_RED_NEXUS;
    break;
  }
  case 3: {
    robot.target.setPoint(BLUE_TOWER);
    robot.target_bearing = LOOK_AT_BLUE_NEXUS;
    break;
  }
  case 4: {
    robot.target.setPoint(RED_UPPER_TOWER);
    robot.target_bearing = LOOK_AT_TOP;
    break;
  }
  case 5: {
    robot.target.setPoint(BLUE_UPPER_TOWER);
    robot.target_bearing = LOOK_AT_TOP;
    break;
  }
  case 6: {
    #ifdef VIVE
    if ((robot.bearing > 90 && robot.bearing <= 190) || (robot.bearing < -180 && robot.bearing < 0)) robot.target_bearing = PI;
    if (robot.bearing < -90 && robot.bearing >= -180) robot.target_bearing = -PI / 2;
    if (robot.bearing < 0 && robot.bearing >= -90) robot.target_bearing = 0;
    if (robot.bearing < 90 && robot.bearing >= 0) robot.target_bearing = PI / 2;
    #endif
    robot.state = 5;
    break;
  }
  case 7: {
    robot.state = 6;
  }
  default:
    break;
 }
  robot.sensors.usedWifi += 1;
}
void updateState() {
  String buffer = String(robot.state) + "," + String(robot.health) + "," + String(robot.leftRPM) + "," + String(robot.rightRPM) + "," + String(robot.lSpeed) + "," + String(robot.rSpeed) + "," + String(robot.bearing_deg) + "," + String(robot.location.x) + "," + String(robot.location.y) + "," + String(robot.forwardDistance) + "," + String(robot.rightwardDistance);
  h.sendplain(buffer);
}