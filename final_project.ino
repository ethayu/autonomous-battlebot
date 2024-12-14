#include "point.h"
#include "config.h"
#include "web.h"

#define fidelity 0.05

const char *ssid = "webLED_Ethan";
void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssid, "");
  robot.startup();
  h.begin();
  h.attachHandler("/", handleRoot);
  h.attachHandler("/press?val=", press);
  h.attachHandler("/release?val=", release);
  h.attachHandler("/speed?val=", getSpeed);
  h.attachHandler("/nav", nav);
  h.attachHandler("/startAttack", startAttack);
  h.attachHandler("/stopAttack", stopAttack);
  h.attachHandler("/updateState", updateState);
  h.attachHandler("/attackClosest", attackClosest);
  h.attachHandler("/attackStructure", attackStructure);
}

void loop()
{
  h.serve();
  robot.updateState();
  #ifdef MOTORS
  robot.action();
  #endif
  robot.printState();
  delay(1000 * fidelity);
}