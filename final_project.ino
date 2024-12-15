#include "point.h"
#include "config.h"
#include "web.h"

TwoWire I2Ctof(0);
TwoWire I2Ctophat(1);

const char *ssid = "Beth";
void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssid, "", 8);
  #ifdef TOF
  I2Ctof.begin(TOF_SDA_PIN, TOF_SCL_PIN);
  #endif
  #ifdef TOPHAT
  I2Ctophat.begin(TOPHAT_SDA_PIN, TOPHAT_SCL_PIN, 40000);
  #endif
  robot.startup();
  h.begin();
  h.attachHandler("/", handleRoot);
  h.attachHandler("/press?val=", press);
  h.attachHandler("/release?val=", release);
  h.attachHandler("/speed?val=", getSpeed);
  h.attachHandler("/nav?val=", nav);
  h.attachHandler("/startAttack", startAttack);
  h.attachHandler("/stopAttack", stopAttack);
  h.attachHandler("/updateState", updateState);
  h.attachHandler("/attackClosest", attackClosest);
  h.attachHandler("/attackStructure?val=", attackStructure);
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