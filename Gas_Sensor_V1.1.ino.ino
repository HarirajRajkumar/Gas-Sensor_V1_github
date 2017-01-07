

#include <Wire.h> 

#include <LiquidCrystal_I2C.h>

#include "DHT.h"



DHT dht;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Threshold
const byte gasThres = 40; // gas conc threshold 


//Pin Declaration
const byte GASpin = A3;
const byte BuzzPin = 11;

// Variable Declaration
byte GASval =0;
int mapGasVal = 0;
boolean BuzzState = LOW;

// Timer Variables
//unsigned long MillisTimer ;
unsigned long previousMillis ;
unsigned long setupTimer ;

// Millis Timer
 unsigned long MillisTimer; // Start Millis
  
void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

    lcd.setCursor(8,0);
    lcd.print("New Horizon");
    
    //lcd.setCursor(
  for( int i=0 ; i<6; i++)
  { 
   lcd.setCursor(6,1);
   lcd.print("Gas Sensor V1.1");
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

  dht.setup(3); // data pin 2
  pinMode(GASpin , INPUT);
  pinMode(BuzzPin , OUTPUT);
  digitalWrite(BuzzPin , HIGH);
  delay(200);
  digitalWrite(BuzzPin , LOW);
  delay(100);

  lcd.clear();
  
  
  lcd.setCursor(3,0);  

 //boolean buzzSetupstate = LOW;
  for( int j=0 ; j < 201 ; j++)
  {
    lcd.clear();
    lcd.print("! Heating Coil !");
    lcd.setCursor(8,1);
    lcd.print(j);
    digitalWrite(BuzzPin , HIGH);
    delay(j);
    digitalWrite(BuzzPin , LOW);
    delay(j/4);
  } 
  delay(500);
  digitalWrite(BuzzPin , HIGH);
  delay(1000);
  digitalWrite(BuzzPin , LOW);
  delay(100);
  
}// close setup

void loop()
{

 MillisTimer = millis();
  delay(dht.getMinimumSamplingPeriod()); // Delay to get data from dht
  
  float humidity = dht.getHumidity();  // retrive humidity data from dht
  float temperature = dht.getTemperature(); // Temperature

  Serial.print(dht.getStatusString()); // Status String to display in serial monitor
  
  Serial.print("\t");
  
  Serial.print(humidity, 1); // prints humidity with one decimal point

  Serial.print("\t \t");

  Serial.print(temperature ,1);  // ( Value , format );

  GASval = analogRead(GASpin);
  mapGasVal =map(GASval , 0 , 255 , 0 , 100);
  Serial.print("\t"); 
  Serial.print(GASval);
  Serial.print("\t");
  Serial.print(mapGasVal);
  Serial.print("%");
   
  lcd.clear();
  lcd.print(humidity,1);
  lcd.print("RH%   ");
  lcd.print(temperature,1);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Gas conc(%) = ");
  lcd.print(mapGasVal);

  if(MillisTimer - previousMillis >= 200)
  { 
  previousMillis = MillisTimer;
   Serial.print("\t");
   Serial.print(MillisTimer);
   Serial.print("ms\t");
   Serial.println(previousMillis);
   
    if(mapGasVal >= gasThres )
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
