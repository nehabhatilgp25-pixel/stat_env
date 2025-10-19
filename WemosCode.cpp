#include <NinjaIoT.h>
#include "DHT.h"

NinjaIoT iot;
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  iot.connect("wifi", "pwd", "uid");

  dht.begin();

  pinMode(D0, INPUT);
  pinMode(D6, OUTPUT);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int GasValue = analogRead(A0);

  iot.WriteVar("Temperature", t);
  iot.WriteVar("Humidity", h);
  iot.WriteVar("GasValue", GasValue);
  iot.ReadAll();

  iot.SyncIN("D0");

  if (digitalRead(D0) == LOW) {
    iot.SyncOut("D6");
  } else {
    if (t > 28 || t < 18 || h > 60 || h < 40 || GasValue > 2000) {
      digitalWrite(D6, HIGH);
    } else {
      digitalWrite(D6, LOW);
    }
  }

  delay(500);
}
