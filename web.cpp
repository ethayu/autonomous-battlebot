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

void press()
{
  String res = h.getText();
  robot.activeKeys.empty();
  if (res.indexOf('w') != -1) robot.activeKeys.insert(1);
  else robot.activeKeys.erase(1);
  if (res.indexOf('a') != -1) robot.activeKeys.insert(3);
  else robot.activeKeys.erase(3);
  if (res.indexOf('s') != -1) robot.activeKeys.insert(2);
  else robot.activeKeys.erase(2);
  if (res.indexOf('d') != -1) robot.activeKeys.insert(4);
  else robot.activeKeys.erase(4);
  robot.state = 1;
}

void release()
{
  String res = h.getText();
  robot.activeKeys.empty();
  if (res.indexOf('w') != -1) robot.activeKeys.insert(1);
  else robot.activeKeys.erase(1);
  if (res.indexOf('a') != -1) robot.activeKeys.insert(3);
  else robot.activeKeys.erase(3);
  if (res.indexOf('s') != -1) robot.activeKeys.insert(2);
  else robot.activeKeys.erase(2);
  if (res.indexOf('d') != -1) robot.activeKeys.insert(4);
  else robot.activeKeys.erase(4);
  robot.state = 0;
}