#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 35
#define SHT_LOX2 32

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

//#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int accuracy = 25;
int distance = 208;

int count = 0;
int sum1 = 0;

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
  lox1.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS,true)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
  lox2.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);
}

void setUpLcd(){
  //lcd.begin(16, 2);
}

void printNumbers(float total){
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print(total);
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
}


void loop() {
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  int mm1 = measure1.RangeMilliMeter;
  int mm2 = measure2.RangeMilliMeter;

  //Serial.print(mm1);
  //Serial.print(", ");
  //Serial.println(mm2);

  if(count < accuracy){
    count++;
    if(!mm1 == 0 && !mm2 == 0) {
      sum1 += distance - mm1 - mm2;
    }
  }else{
    float avarage1 = sum1 / (float) count;
    //printNumbers(avarage1,avarage2);
    if(avarage1 <= 0){
      avarage1 = 1;
    }
    Serial.print("El objeto mide: ");
    Serial.println(avarage1);
    count = 0;
    sum1 = 0;
    //while(1);
  }
}
