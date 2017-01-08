// Version 1.3


#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include "DHT.h"


DHT dht;  // CLASS object

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Thresholds
const byte gasThres = 20; // gas conc threshold


//Pin Declaration
const byte GASpin = A3;
const byte BuzzPin = 11;

// Variable Declaration
byte GASval = 0;
int mapGasVal = 0;
boolean BuzzState = LOW;

// Timer Variables
//unsigned long MillisTimer ;
unsigned long previousMillis ;
unsigned long setupTimer ;

// Millis Timer
unsigned long MillisTimer; // Start Millis

// Funcn declaration

/* This function changes buzzer delay interval, with respective to
    Gas concentration range(in %) .
    ie for example : If the input value conc is in 20 to 25 range , then buzzer delay interval becomes 1000

    It is mainly written to indicate the user that LPG ppm is more near Gas_Sensor_V1.2.
*/
int beepTimerfunc(byte mapGasVal);

/*
   Funcn Name : heatCoil
   Description :
    To wait for coil to heat up.
   Waiting Time : 200 == 20 seconds
   Variable to change the waiting Time is
    -> heatCoilTimer;
*/
byte heatCoilTimer = 200; // timer variable

void heatCoil(byte heatCoilTimer); // funcn



void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  // Print lines and scroll left
  lcd.setCursor(8, 0);
  lcd.print("New Horizon");
  for ( int i = 0 ; i < 6; i++)
  {
    lcd.setCursor(6, 1);
    lcd.print("Gas Sensor V1.2");
    lcd.scrollDisplayLeft();

    delay(500);
  }


  delay(2000);

  lcd.clear();
  lcd.print("By HariRaj");//,Gotham");
  //lcd.setCursor(0,1);
  // lcd.print("Jana,Sid");

  delay(1200);

  // lcd print
  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");

  dht.setup(3); // data pin 3

  // Setup pin mode
  pinMode(GASpin , INPUT);
  pinMode(BuzzPin , OUTPUT);

  // inital buzzer check (beep)
  digitalWrite(BuzzPin , HIGH);
  delay(200);
  digitalWrite(BuzzPin , LOW);
  delay(100);

  lcd.clear();

  // Heat coil for 20 seconds and beep consecutively
 // heatCoil(heatCoilTimer);


}// close setup

void loop()
{

  MillisTimer = millis(); // start millis timer

  long dhtTimer = 2000;
   long previousDhtTimer =0;

   
 float humidity ;
 float temperature;
 
  if(MillisTimer - previousDhtTimer >= dhtTimer)
  {
    previousDhtTimer = MillisTimer;
   humidity = dht.getHumidity();  // retrive humidity data from dht
   temperature = dht.getTemperature(); // Temperature
   Serial.print(dht.getStatusString()); // Status String to display in serial monitor
  }
  // Prints data in serial monitor

 

  Serial.print("\t");

  Serial.print(humidity, 1); // prints humidity with one decimal point

  Serial.print("\t \t");

  Serial.print(temperature, 1); // ( Value , format );

  GASval = analogRead(GASpin);
  mapGasVal = 36;//map(GASval , 0 , 255 , 0 , 100);
  Serial.print("\t");
  Serial.print(GASval);
  Serial.print("\t");
  Serial.print(mapGasVal);
  Serial.print("%");




  
  lcd.clear();
  lcd.print(humidity, 1);
  lcd.print("RH%   ");
  lcd.print(temperature, 1);
  lcd.print((char)223); // to display  ° in LCD ( ASCII )
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Gas conc(%) = ");
  lcd.print(mapGasVal);
  delay(200);
  
  // print data in LCD over


  int beepTimer = beepTimerfunc(mapGasVal) ;

  if (MillisTimer - previousMillis >= beepTimer)
  {
    previousMillis = MillisTimer;
    Serial.print("\t");
    Serial.print(MillisTimer);
    Serial.print("ms\t");
    Serial.println(previousMillis);

    if (mapGasVal >= gasThres)
    {
      if (BuzzState == LOW)
      {
        BuzzState = HIGH;
      }
      else
        BuzzState = LOW;

    }// close Gas compare
  }// close Timer if
  digitalWrite (BuzzPin , BuzzState);


}// close loop


int buzzTimer_2 = 0;

int beepTimerfunc(int mapGasVal_2 ) // funcn 1
{
  
  if (  (mapGasVal_2 > 20) && (mapGasVal_2 < 25) )
    return buzzTimer_2 = 1000;
  else if (  (mapGasVal_2 > 25) || (mapGasVal_2 < 30) )
    return buzzTimer_2 = 750;
  else if (  (mapGasVal_2 >30) || (mapGasVal_2 < 40) )
    return buzzTimer_2 = 500;
  else if (  (mapGasVal_2 >40) )// || (mapGasVal_2 < 40) )
    return  buzzTimer_2 = 200;


}

void heatCoil(byte heatCoilTimer) // funcn 2
{
  lcd.setCursor(3, 0);

  for ( int j = 0 ; j < (heatCoilTimer + 1) ; j++)
  {
    lcd.clear();
    lcd.print("! Heating Coil !");
    lcd.setCursor(8, 1);
    lcd.print(j);
    digitalWrite(BuzzPin , HIGH);
    delay(j / 2);
    digitalWrite(BuzzPin , LOW);
    delay(j / 4);
  }
  delay(500);

  // Last ready beep
  digitalWrite(BuzzPin , HIGH);
  delay(1000);
  digitalWrite(BuzzPin , LOW);
  delay(100);

}

