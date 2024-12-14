#include "auton.h"
#include <cmath>

void Robot::updateAutonState()
{
  switch (state)
  {
  case 1: // navTo()
  {
    if (location.hasReachedPoint(target))
    {
      if (similarAngle(robot.bearing, target_bearing, bearingTolerance))
      {
        substate = 5;
      }
      else
      {
        substate = 4;
      }
    }
    else
    {
      bool correctOrientation = similarAngle(robot.bearing, location.calcAngle(target), bearingTolerance);
      if (forwardDistance > wallFTolerance) // if nothing in front
      {
        if (substate < 2 || correctOrientation || (substate == 2 && similarAngle(robot.bearing, searchLimit, bearingTolerance)))
          substate = correctOrientation ? 0 : 1;                      // if not wall following or in right direction, or was starting wall follow and got to end of turn search (obstacle probably moved)
        else if (substate == 2 && rightwardDistance < wallRTolerance) // we have reoriented
        {
          substate = 3;
        }
      }
      else
      {
        substate = 2; // we must start turning left if we are blocked
        searchLimit = robot.bearing + PI / 2;
      }
    }
    break;
  }
  case 2: // attackClosest()
  {
    if (substate == 0)
    {
      if (forwardDistance < closestDistance)
      {
        closestDistance = forwardDistance;
        target_bearing = bearing;
      }
      if (rightwardDistance < closestDistance)
      {
        closestDistance = rightwardDistance;
        target_bearing = modAngle(bearing - PI / 2);
      }
      if (closestDistance < wallFTolerance * 3 || similarAngle(robot.bearing, searchLimit, bearingTolerance))
      {
        substate = 1;
      }
    }
    else if (substate == 1 && similarAngle(robot.bearing, target_bearing, bearingTolerance))
    {
      substate = 2;
    }
    else if (substate = 2)
    {
      if (forwardDistance > wallFTolerance * 5)
      {
        substate = 0;
      }
    }
  }
  case 3: // attackStructure()
  {
    if (substate == 5)
    { // if navTo() has finished
      substate = 6;
    }
  }
  }
}

void Robot::orientTo(float bearing)
{
  float angleDiff = modAngle(bearing - robot.bearing);
  lSpeed = aggresiveBearingConst * angleDiff;
  rSpeed = -1 * aggresiveBearingConst * angleDiff;
}

void Robot::navTo()
{
  switch (substate)
  {
  case 0: // Move Forward
  {
    float angleDiff = modAngle(location.calcAngle(target) - robot.bearing);
    lSpeed = 50 + passiveBearingConst / 10 * angleDiff;
    rSpeed = 50 - passiveBearingConst / 10 * angleDiff;
    break;
  }
  case 1: // Turn until we are oriented to target
  {
    orientTo(location.calcAngle(target));
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
    orientTo(target_bearing);
    break;
  }
  case 5: // done
  {
    substate = 0;
    break;
  }
  default:
    break;
  }
}

void Robot::attackClosest()
{
  switch (substate)
  {
  case 0: // Scan
  {
    attacking = false;
    lSpeed = -50;
    rSpeed = 50;
    break;
  }
  case 1: // Orient
  {
    attacking = false;
    orientTo(target_bearing);
    break;
  }
  case 2: // Attack
  {
    attacking = false;
    lSpeed = 50;
    rSpeed = 50;
    attacking = true;
  }
  }
}

void Robot::attackStructure()
{
  if (substate < 5)
  { // if still in navTo() logic
    navTo();
  }
  else
  {
    lSpeed = 50;
    rSpeed = 50;
    attacking = true;
  }
}
