// Version 1.4

/******* Coding done by R.Hariraj-Electronics&Communication-4th Semester-New Horizon College** 
 ******* Jan 8th 2017 *******  
 *  Buzzes at different time interval indicating more LPG ppm is present near gas sensor.
 *  User defined ranges.
 *
 * 
 *  Sensitive in reading Gas concentration.
  */

#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include "DHT.h"


DHT dht;  // CLASS object

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Thresholds
const byte gasThres = 8; // gas conc threshold

//Pin Declaration
const byte GASpin = A3;
const byte BuzzPin = 11;

// Variable Declaration
int GASval = 0;
byte mapGASval = 0;
boolean BuzzState = LOW;

// Timer Variables
//unsigned long MillisTimer ;
unsigned long previousMillis ;
unsigned long setupTimer ;
long LPGtimer = 0;

// Millis Timer
unsigned long MillisTimer; // Start Millis

// Funcn declaration

/* This function changes buzzer delay interval, with respective to
    Gas concentration range(in %) .
    ie for example : If the input value conc is in 20 to 25 range , then buzzer delay interval becomes 1000

    It is mainly written to indicate the user that LPG ppm is more near Gas_Sensor (UPDATED in V1.2)
*/

byte lowRange_1 = 8;
byte lowRange_2 = 30;
byte midRange_2 = 45;
byte highRange_2 = 50;

int beepTimerfunc(byte mapGASval , byte lowRange_1 , byte lowRange_2 , byte midRange_2 , byte highRange_2 );

/*
   Funcn Name : heatCoil
   Description :
    To wait for coil to heat up.
   Waiting Time : 200 == 20 seconds
   Variable to change the waiting time is
    -> heatCoilTimer;
*/
byte heatCoilTimer = 200; // timer variable

void heatCoil(byte heatCoilTimer); // funcn

/* Function name : lcdDataPrint.
 *  Prints Temperature , Humidity and Gas concentration. 
 */

void lcdDataPrint(byte mapGASval_2, float humidity_2 , float temperature_2); // funcn declaration

void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  // Print lines and scroll left
  lcd.setCursor(8, 0);
  lcd.print("New Horizon");
  
  for ( int i = 0 ; i < 6; i++)  // Prints string and scrolls towards left
  {
    lcd.setCursor(6, 1);
    lcd.print("Gas Sensor V1.4");
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

  // Heat coil for 20 seconds and beeps at last
  heatCoil(heatCoilTimer);
  
}// close setup

void loop()
{
   MillisTimer = millis(); // start millis timer

 // dht time constents
  const long dhtTimer = 2000; 
  long previousDhtTimer =0;

 // dht values
 float humidity ;
 float temperature;
 
  if(MillisTimer - previousDhtTimer >= dhtTimer) // Read DHT sensor values for every 2 seconds
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
  
  GASval = analogRead(GASpin)/10;
  GASval = GASval / 1.02;
  // To get MQ-6 reading value in percentage
  
  mapGASval = map(GASval , 0 , 65 , 0 , 100);
  
  Serial.print("\t");
  Serial.print(GASval);
  Serial.print("(int)\t");
  Serial.print(mapGASval);
  Serial.println("%");

// Prints these datas only if the conc is below gasThres.
 lcdDataPrint(mapGASval , humidity , temperature) ;
 
  // print data in LCD over

long buzzTimer = 1200;
long previousBuzzTimer =0;

/*
 * Function Name : beepTimerfunc (mapGASval , lowRange_1 , lowRange_2 , midRange_2 , highRange_2 ); 
 * Return type : int
 * 
 * Used to get buzzer on off interval time , ie for mapped gas val (mapGASval)
 * 
 */
  int beepTimer =  beepTimerfunc( mapGASval , lowRange_1 , lowRange_2 , midRange_2 , highRange_2 ); 
  
  if( mapGASval > gasThres) 
  {
     lcd.clear();
      if ( MillisTimer - LPGtimer >= 400)
       {
        LPGtimer = MillisTimer;
        lcd.print("! LPG DETECTED !"); 
       }
      lcd.setCursor(0, 1);
      lcd.print("Gas conc(%) =");
      lcd.print(mapGASval); 
      delay(200);
  }
 
  if (MillisTimer - previousMillis >= beepTimer)
  {
    previousMillis = MillisTimer;
    
    if (mapGASval > gasThres)  
    {  // version 1.4 update
      lcd.clear();    
      lcd.setCursor(0, 1);
      lcd.print("Gas conc(%) =");
      lcd.print(mapGASval);     
    if (BuzzState == LOW)
      {
        BuzzState = HIGH;
      }
   else
        BuzzState = LOW;
    }// close Gas compare
  }// close Timer if

// Makes Buzzer LOW in case the reading value changes from 8 to 7 instantenly ,keeping the buzzer HIGH
 if((BuzzState == HIGH) && (mapGASval <8) && (MillisTimer - previousBuzzTimer >= buzzTimer ))  
 {
  previousBuzzTimer = MillisTimer;
  BuzzState = LOW;
 }

digitalWrite(BuzzPin , BuzzState);

}// close loop


long buzzTimer_2 = 0; // a globel variable

    /* USER DEFINED FUNCTIONS */

int beepTimerfunc(byte mapGASval_2 , byte lowRange_1 , byte lowRange_2 , byte midRange_2 , byte highRange_2 )
{
  if (  (mapGASval_2 > lowRange_1) && (mapGASval_2 < lowRange_2) )
    return buzzTimer_2 = 1600;
  else if (  (mapGASval_2 > lowRange_2) || (mapGASval_2 < midRange_2) )
    return buzzTimer_2 = 800;
  else if (  (mapGASval_2 >midRange_2) || (mapGASval_2 < highRange_2) )
    return buzzTimer_2 = 400;
  else if (  (mapGASval_2 >highRange_2) )// || (mapGASval_2 < 40) )
    return  buzzTimer_2 = 210;
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

void lcdDataPrint(byte mapGASval_2, float humidity_2 , float temperature_2) // funcn 3
{
  if(mapGASval_2 <= gasThres) 
  {
  lcd.clear();
  lcd.print(humidity_2, 1);
  lcd.print("RH%   ");
  lcd.print(temperature_2, 1);
  lcd.print((char)223); // to display  ° in LCD ( ASCII )
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Gas conc(%) =");
  lcd.print(mapGASval_2);  //mapGASval
  delay(200);
  } 
}

