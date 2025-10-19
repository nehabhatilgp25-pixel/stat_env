#include <NinjaIoT.h>
#include <DHT.h>

NinjaIoT iot;

#define DHTPIN D2      //connecting sensor to pin d2
#define DHTTYPE DHT11  //type of DHT sensor
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  iot.connect("wifi", "pwd", "uid");  //link: https://iot.roboninja.in/
  dht.begin();
  pinMode(D6, OUTPUT)
}

void loop() {
  float h = dht.readHumidity();     //storing humidity
  float t = dht.readTemperature();  //storing temperature

  iot.WriteVar("Temperature", t);
  iot.WriteVar("Humidity", h);

  iot.ReadAll();  //reading all values from cloud

  int GasValue = analogRead(A0);       //getting analog gas value from A0
  iot.WriteVar("GasValue", GasValue);  // uploading to cloud

  iot.SyncIN("D0");  // D0 doesn't connect to anything, it just outputs whether it's in automatic or manual mode.
  // 0 = manual
  // 1 = automatic
  if (digitalRead(D0) == LOW) {
    iot.SyncOut("D6");  // Telling LED whether or not to glow.
  } else {
    if (t > 28 || t < 18 || h > 60 || h < 40 || GasValue > 2000) {
      digitalWrite(D6, HIGH);
    } else {
      digitalWrite(D6, LOW);
    }
  }

  delay(500);
}
