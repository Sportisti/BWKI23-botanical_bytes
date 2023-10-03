#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <SD.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define SENSOR_PIN 9
#define RANGE 5000

File myFile;

Adafruit_BME680 bme; // I2C

int i = 0;

void setup() {

  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT);
  pinMode(5, INPUT);

  while (!Serial);
  Serial.print("Initializing SD card...");

  // Error Checking
  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }

  // Set up oversampling and filter initialization
  
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  Serial.println("initialization done.");
  
  document_setup();
}

void loop() {
 while (digitalRead(5)) {
  
    myFile = SD.open("data.txt", FILE_WRITE);

    if (myFile) {
      Serial.print("Writing to data.txt...");
      } else {
      Serial.println("error opening data.txt");
    }
    
    int pulse_high, pulse_high_2, pulse_low;
    unsigned long time_start;
    pulse_high = pulseIn(SENSOR_PIN, HIGH, 2200000UL) / 1000;
    time_start = millis();
    pulse_high_2 = pulseIn(SENSOR_PIN, HIGH, 1100000UL) / 1000;
    pulse_low = millis() - time_start - pulse_high_2;
    
    if (!bme.endReading()) {
      Serial.println(F("Failed to complete reading :("));
      return;
    }
    
    writeData(pulse_high, RANGE);
    //delay(6000);
    Serial.println("finished Writing one Dataset!");
    myFile.close();
    Serial.println("Closed File!");
  
    delay(60000);
  } 
  delay(60000);
}
void writeData(int pulse_high, int Range) {

  // *C
  myFile.print(bme.temperature);
  myFile.print(";");

  // hPa
  myFile.print(bme.pressure / 100.0);
  myFile.print(";");

  // %
  myFile.print(bme.humidity);
  myFile.print(";");

  // KOhms
  myFile.print(bme.gas_resistance / 1000.0);
  myFile.print(";");

  // m
  myFile.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  myFile.print(";");

  // CO2
  myFile.print(analogRead(A3));
  myFile.print(";");

  //CO2 Neuer Wert
  myFile.print((pulse_high - 2) * Range / 1000);
  myFile.print(";");
  
  //Feutchtigkeit
  myFile.println(analogRead(A2));
}

void document_setup() {
  
  myFile = SD.open("data.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("Writing to data.txt...");
    } else {
    Serial.println("error opening data.txt");
  }
  
  Serial.println("finished Writing Start Line!");
  myFile.println("\n\n");
  myFile.println("Tempeture;pressure;humidity;gas_resistance;realAltitude;CO2;CO2New;Feuchtigkeit");
  myFile.close();
}
