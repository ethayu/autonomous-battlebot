#include "auton.h"

#include <cmath>

// helper
float euclideanDist(int x1, int y1, int x2, int y2)
{
  float dist = hypot(x2 - x1, y2 - y1);
  return dist;
}

float calcAngle(int x1, int y1, int x2, int y2)
{
  float angle = atan2(y2 - y1, x2 - x1);
  return angle;
}

bool similarAngle(float angle1, float angle2, float tolerance)
{
  angle1 = modAngle(angle1);
  angle2 = modAngle(angle2);

  float diff = fabs(angle1 - angle2);

  diff = fmin(diff, 2 * PI - diff);

  return diff <= tolerance;
}

float modAngle(float angle)
{
  return fmod(angle + PI, 2 * PI) - PI;
}

bool hasReachedTarget(int currX, int currY, int targetX, int targetY)
{
  return euclideanDist(currX, currY, targetX, targetY) < distTolerance;
}

void Robot::updateNavToState()
{
  if (hasReachedTarget(x, y, target_x, target_y))
  {
    nav_substate = 4;
  }
  else
  {
    bool correctOrientation = similarAngle(robot.bearing, calcAngle(x, y, target_x, target_y), bearingTolerance);
    if (forwardDistance > wallFTolerance) // if nothing in front
    {
      if (nav_substate < 2 || correctOrientation || (nav_substate == 2 && similarAngle(robot.bearing, wallFollowBearing, bearingTolerance)))
        nav_substate = correctOrientation ? 0 : 1;                      // if not wall following or in right direction, or was starting wall follow and got to end of turn search (obstacle probably moved)
      else if (nav_substate == 2 && rightwardDistance < wallRTolerance) // we have reoriented
      {
        nav_substate = 3;
      }
    }
    else
    {
      nav_substate = 2; // we must start turning left if we are blocked
      wallFollowBearing = robot.bearing + PI / 2;
    }
  }
}

void Robot::navTo()
{
  updateNavToState();
  switch (nav_substate)
  {
  case 0: // Move Forward
  {
    float angleDiff = modAngle(calcAngle(x, y, target_x, target_y) - robot.bearing);
    lSpeed = 50 + passiveBearingConst / 10 * angleDiff;
    rSpeed = 50 - passiveBearingConst / 10 * angleDiff;
    break;
  }
  case 1: // Turn until we are oriented to target
  {
    float angleDiff = modAngle(calcAngle(x, y, target_x, target_y) - robot.bearing);
    lSpeed = aggresiveBearingConst * angleDiff;
    rSpeed = -1 * aggresiveBearingConst * angleDiff;
    break;
  }
  case 2: // Wall following initialization
  {
    lSpeed = -50;
    rSpeed = 50;
    break;
  }
  case 3: // Wall following
  {
    lSpeed = 50;
    rSpeed = 50 + wallFollowConst * (wallFollowRightDist - rightwardDistance);
    break;
  }
  case 4: // Orient to target bearing
  {
    float angleDiff = modAngle(target_bearing - robot.bearing);
    lSpeed = aggresiveBearingConst * angleDiff;
    rSpeed = -1 * aggresiveBearingConst * angleDiff;
    break;
  }
  default:
    break;
  }
}
