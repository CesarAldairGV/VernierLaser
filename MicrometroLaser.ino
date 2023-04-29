#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 7
#define SHT_LOX2 6

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ACCURACY_PIN = A7;
int DEFAULT_ACCURACY = 29;
int accuracy = 200;

int count = 0;
long sum1 = 0;
long sum2 = 0;

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS,true)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS,true)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}

void setUpLcd(){
  lcd.begin(16, 2);
}

void printNumbers(float number1, float number2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(number1);
  lcd.setCursor(0, 1);
  lcd.print(number2);
}

void setUpAccuracy(){
  int a = analogRead(A7);
  accuracy = (a / 2) + DEFAULT_ACCURACY;
  float string_accuracy = accuracy / 5.4;
  Serial.print("Midiendo con una precision del ");
  Serial.print(string_accuracy);
  Serial.print(" % ( ");
  Serial.print(accuracy);
  Serial.println(" datos)");
}

void setup() {
  delay(50);
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  Serial.println(F("Starting..."));
  setID();
  setUpLcd();
  
  // Valor de la precision
  //pinMode(ACCURACY_PIN,INPUT);
  //setUpAccuracy();
}


void loop() {
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  int mm1 = measure1.RangeMilliMeter;
  int mm2 = measure2.RangeMilliMeter;

  if(count < accuracy){
    count++;
    sum1 += mm1;
    sum2 += mm2;
  }else{
    float avarage1 = sum1 / (float) count;
    float avarage2 = sum2 / (float) count;
    Serial.print("Las distancias son: ");
    Serial.print(avarage1);
    Serial.print(", ");
    Serial.println(avarage2);
    printNumbers(avarage1,avarage2);
    count = 0;
    sum1 = 0;
    sum2 = 0;
  }
}
