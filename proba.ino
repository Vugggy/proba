#include <LiquidCrystal_I2C.h>
#include "max6675.h"

// Defin pins
int reverseSwitch = 2;  // Push button for reverse
int driverPUL = 7;    // PUL- pin
int driverDIR = 6;    // DIR- pin
int spd = A0;     // Potentiometer 
int brz=0;


int thermoDO = 8;
int thermoCS = 9;
int thermoCLK =10;
float temperature;

const int RELAY_PIN = 3;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

const int TEMP_THRESHOLD_UPPER = 60; // upper threshold of temperature, change to your desire value
const int TEMP_THRESHOLD_LOWER = 40;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables 
int pd = 500;       // Pulse Delay period
int refresh = 0;
boolean setdir = LOW; // Set Direction 
// Interrupt Handler 
void revmotor (){
  setdir = !setdir; 
}
 
void setup() {
  Serial.begin(9600);
  Serial.println("test");
  lcd.init(); // initialize the lcd
  lcd.backlight(); 
      lcd.clear();                 // clear display
      lcd.setCursor(0, 0);         // move cursor to   (0, 0)
      lcd.print("priprema..."); 
  pinMode (driverPUL, OUTPUT);
  pinMode (driverDIR, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(reverseSwitch), revmotor, FALLING); 
  delay(500);
  
}

void loop(){
  loop_start();
  loop_radnja();
  }

void loop_start() { 
  refresh=0;
  temperature = thermocouple.readCelsius();
  digitalWrite(RELAY_PIN, HIGH);
  //while(temperature<(TEMP_THRESHOLD_UPPER+TEMP_THRESHOLD_LOWER)/2){
  while(temperature<TEMP_THRESHOLD_LOWER){ 
    temperature = thermocouple.readCelsius();
    Serial.println("start");
    Serial.println(temperature);
    delay(500);
  }
  digitalWrite(RELAY_PIN, LOW);
  return;
}

void loop_radnja() { 
  refresh = 0;
  while(true){
    if(refresh%500==0){
    temperature = thermocouple.readCelsius();
         
      if(temperature<TEMP_THRESHOLD_LOWER){
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("visoko");
      }
      else if (temperature>TEMP_THRESHOLD_UPPER-10){
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("nisko");
      }
      Serial.println("radnja");
      Serial.println(temperature);
    }
      pd = map(analogRead(spd)%1000,0,1023,2000,50);
      digitalWrite(driverDIR,setdir);
      digitalWrite(driverPUL,HIGH);
      delayMicroseconds(pd);
      digitalWrite(driverPUL,LOW);
      delayMicroseconds(pd);
      
   
    if(refresh == 1000) {
      brz=analogRead(spd);
      lcd.clear();                 // clear display
      lcd.setCursor(0, 0);         // move cursor to   (0, 0)
      lcd.print(brz);
      refresh = 0;
      lcd.setCursor(0, 1);
      lcd.print(temperature);
    }
    refresh++;
    
  } 
}
