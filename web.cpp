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
    robot.target_x = x;
    robot.target_y = y;
    robot.target_bearing = angle * PI / 180;
    robot.state = 5;
    robot.sensors.usedWifi = true;
  }
  else
  {
    Serial.println("Error: Invalid input format. Expected 'x,y,angle'.");
  }
}

void updateState() {
  String buffer = String(robot.state) + "," + String(robot.health) + "," + String(robot.leftRPM) + "," + String(robot.rightRPM) + "," + String(robot.lSpeed) + "," + String(robot.rSpeed) + "," + String(robot.bearing) + "," + String(robot.x) + "," + String(robot.y) + "," + String(robot.forwardDistance) + "," + String(robot.rightwardDistance);
  h.sendplain(buffer);
}