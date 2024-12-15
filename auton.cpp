#include "auton.h"
#include <cmath>

void Robot::updateAutonState()
{
  switch (state)
  {
  case 2: // navTo()
  {
    if (location.hasReachedPoint(target) && forwardDistance <= wallFTolerance)
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
      while (!similarAngle(robot.bearing, location.calcAngle(target), bearingTolerance))
      {
        orientTo(location.calcAngle(target));
        delay(200);
      }
      while (forwardDistance <= wallFTolerance)
      {
        setLeftSpeed(-10);
        setRightSpeed(-10);
        delay(200);
      }
      orientTo(modAngle(bearing + PI / 2));
      bool correctOrientation = similarAngle(robot.bearing, location.calcAngle(target), bearingTolerance);
      Serial.println(location.calcAngle(target));
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
  case 3: // attackClosest()
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
  case 4: // attackStructure()
  {
    if (substate == 5)
    { // if navTo() has finished
      substate = 6;
    }
    break;
  }
  case 5:
  {
    if (forwardDistance < wallFTolerance)
    {
      setLeftSpeed(-5);
      setRightSpeed(-5);
      delay(2000);
#ifdef VIVE
      if (similarAngle(robot.bearing, target_bearing, bearingTolerance))
      {
        target_bearing = modAngle(target_bearing + PI);
      }
      orientTo(target_bearing);
      Serial.println(target_bearing);
#endif
#ifndef VIVE
      setLeftSpeed(-5);
      setRightSpeed(5);
      delay(2000);
#endif
    }
#ifdef VIVE
    if (!similarAngle(robot.bearing, target_bearing, bearingTolerance))
    {
      orientTo(target_bearing);
      Serial.println(target_bearing);
    }
    else
    {
#endif
      lSpeed = 6;
      rSpeed = 5;
      Serial.println("Step 5");
#ifdef VIVE
    }
#endif
    break;
  }
  case 6:
  {
    if (forwardDistance < wallFTolerance)
    {
      setLeftSpeed(-5);
      setRightSpeed(-5);
      delay(2000);
      setLeftSpeed(-5);
      setRightSpeed(5);
      delay(800);
    }
    lSpeed = 6;
    rSpeed = 5;
    Serial.println("Step 5");
    break;
  }
  default:
    break;
  }
}

void Robot::orientTo(float bearing)
{
  float angleDiff = modAngle(bearing - robot.bearing);
  lSpeed = -1 * aggresiveBearingConst * angleDiff;
  rSpeed = aggresiveBearingConst * angleDiff;
}

void Robot::navTo()
{
  substate = 3;
  switch (substate)
  {
  case 0: // Move Forward
  {
    lSpeed = 5;
    rSpeed = 5;
    break;
  }
  case 1: // Turn until we are oriented to target
  {
    orientTo(location.calcAngle(target));
    break;
  }
  case 2: // Wall following initialization
  {
    lSpeed = -10;
    rSpeed = 10;
    break;
  }
  case 3: // Wall following
  {
    lSpeed = min(2 - wallFollowConst * (wallFollowRightDist - rightwardDistance), 2.3);
    rSpeed = 5 + wallFollowConst * (wallFollowRightDist - rightwardDistance);
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
    lSpeed = -20;
    rSpeed = 20;
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
    lSpeed = 20;
    rSpeed = 20;
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
    lSpeed = 20;
    rSpeed = 20;
    attacking = true;
  }
}
