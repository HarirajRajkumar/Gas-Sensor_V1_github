

/*  Project Name : LPG Gas sensor
 *  This following code detects amount of LPG present in air using MQ-6 and include to display
 *  Temperature and Humidity using AM2302 (DHT22).
 *  
 *  Version 1
 * Programmer : Hariraj.R
 * Date : 4-Jan-2016
 *  
 * This version reads and displays Temperature , humidity from DHT22 & amount LPG concentration in air using MQ-6
 * and alarms ( beeps ) when it is above certain threshold concentration ( hardcoded ) 
 * 
 */
 
#include <Wire.h> 

#include <LiquidCrystal_I2C.h>

#include "DHT.h"

DHT dht;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Pin Declaration
const byte GASpin = A3;
const byte BuzzPin = 4;

// Variable Declaration
byte GASval =0;
byte mapGasVal = 0;
boolean BuzzState = LOW;

// Timer Variables
unsigned long MillisTimer ;
unsigned long previousMillis ;

void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("New Horizon");
  lcd.setCursor(2,1);
  lcd.print("Gas Sensor V1");
  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");

  dht.setup(3); // data pin 2
  pinMode(GASpin , INPUT);
  pinMode(BuzzPin , OUTPUT);
}

void loop()
{
  MillisTimer = millis(); // Start Millis
  
  delay(dht.getMinimumSamplingPeriod()); // Delay to get data from dht
  
  float humidity = dht.getHumidity();  // retrive humidity data from dht
  float temperature = dht.getTemperature(); // Temperature

  Serial.print(dht.getStatusString()); // Status String to display in serial monitor
  
  Serial.print("\t");
  
  Serial.print(humidity, 1); // prints humidity with one decimal point

  Serial.print("\t \t");

  Serial.print(temperature ,1);  // ( Value , format );

  GASval = 200; //analogRead(GASpin);
  mapGasVal = map(GASval , 0 , 255 , 0 , 100);
  Serial.print("\t"); 
  Serial.print(GASval);
  Serial.print("\t");
  Serial.print(mapGasVal);
  Serial.print("%");
   
  lcd.clear();
  lcd.print(humidity);
  lcd.print("RH% ");
  lcd.print(temperature);
  lcd.print("*C");
  lcd.setCursor(0,1);
  lcd.print("Gas in % =  ");
  lcd.print(mapGasVal);

  if(MillisTimer - previousMillis >= 100)
  { 
  previousMillis = MillisTimer;
    
    if(GASval >= 150 )
   { 
    if(BuzzState == LOW)
    {
      BuzzState = HIGH;
    }
    else
    BuzzState = LOW;
   
  }// close Gas compare
  }// close Timer if
  digitalWrite (BuzzPin , BuzzState);


}// close loop
