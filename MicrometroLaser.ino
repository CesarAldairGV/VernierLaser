#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int counter = 0;
long sum = 0;

int ACCURACY_PIN = A7;
int DEFAULT_ACCURACY = 29;
int accuracy = 0;

void setup() {
  delay(50);
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin(0x29,true)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n")); 

  pinMode(ACCURACY_PIN,INPUT);

  // Valor de la precision
  int a = analogRead(A7);
  accuracy = (a / 2) + DEFAULT_ACCURACY;
  float string_accuracy = accuracy / 5.4;
  Serial.print("Midiendo con una precision del ");
  Serial.print(string_accuracy);
  Serial.print(" % ( ");
  Serial.print(accuracy);
  Serial.println(" datos)");
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  int mm = measure.RangeMilliMeter;

  if(counter < accuracy){
    counter++;
    sum += mm;
  }else{
    float avarage = sum / (float) counter;
    Serial.print("Distancia: ");
    Serial.println(avarage);
    while(true);
  }
}
