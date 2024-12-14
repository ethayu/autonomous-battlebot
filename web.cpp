#include "web.h"
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
}

void stopAttack() 
{
  robot.attacking = false;
}

void press()
{
  String res = h.getText();
  robot.activeKeys.empty();
  if (res.indexOf('w') != -1)
    robot.activeKeys.insert(1);
  else
    robot.activeKeys.erase(1);
  if (res.indexOf('a') != -1)
    robot.activeKeys.insert(3);
  else
    robot.activeKeys.erase(3);
  if (res.indexOf('s') != -1)
    robot.activeKeys.insert(2);
  else
    robot.activeKeys.erase(2);
  if (res.indexOf('d') != -1)
    robot.activeKeys.insert(4);
  else
    robot.activeKeys.erase(4);
  robot.state = 1;
  robot.sensors.usedWifi = true;
}

void release()
{
  String res = h.getText();
  robot.activeKeys.empty();
  if (res.indexOf('w') != -1)
    robot.activeKeys.insert(1);
  else
    robot.activeKeys.erase(1);
  if (res.indexOf('a') != -1)
    robot.activeKeys.insert(3);
  else 
    robot.activeKeys.erase(3);
  if (res.indexOf('s') != -1)
    robot.activeKeys.insert(2);
  else
    robot.activeKeys.erase(2);
  if (res.indexOf('d') != -1)
    robot.activeKeys.insert(4);
  else
    robot.activeKeys.erase(4);
  robot.state = 0;
  robot.sensors.usedWifi = true;
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
    robot.sensors.usedWifi = true;
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
  default:
    break;
 }
}
void updateState() {
  String buffer = String(robot.state) + "," + String(robot.health) + "," + String(robot.leftRPM) + "," + String(robot.rightRPM) + "," + String(robot.lSpeed) + "," + String(robot.rSpeed) + "," + String(robot.bearing_deg) + "," + String(robot.location.x) + "," + String(robot.location.y) + "," + String(robot.forwardDistance) + "," + String(robot.rightwardDistance);
  h.sendplain(buffer);
}