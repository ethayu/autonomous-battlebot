#ifndef WEB_H
#define WEB_H

#include "html510.h"
#include "robot.h"
extern HTML510Server h;
void handleRoot();
void getSpeed();
void press();
void release();
void nav();
void startAttack();
void stopAttack();
#endif