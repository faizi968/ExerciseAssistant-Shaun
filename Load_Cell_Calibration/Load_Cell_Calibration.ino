#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 25;
const int LOADCELL_SCK_PIN = 26;

HX711 scale;

void setup() {
  Serial.begin(115200);
  Serial.print("Calibrating HX711");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();
  delay(1000);
  Serial.println("Put known weight on ");
  delay(3000);
  Serial.println(scale.get_units(10));
  
}

void loop() {
  
}
