#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHT1_PIN 2     
#define DHT2_PIN 3  
#define DHTTYPE DHT11 
DHT dht1(DHT1_PIN, DHTTYPE);
DHT dht2(DHT2_PIN, DHTTYPE);

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //three columns
int lock1=A0;
int lock2=A1;
int ventilador=A2;
int enfriador=A3;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const String password_1 = "1234"; // change your password here
const String password_2 = "5642"; // change your password here
const String password_3 = "4545"; // change your password here
const String password_4 = "33112323"; // change your password here
String input_password;

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup(){ 
pinMode(13, OUTPUT);

  dht1.begin();//for first DHT module
  dht2.begin();//for 2nd DHT module  and do the same for 3rd and 4th etc.

  
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  while (!Serial);
  Serial.println("ESCANEA EL CODIGO DE BARRA");

  input_password.reserve(32); // maximum input characters is 33, change if needed
    pinMode(lock1, OUTPUT);
    pinMode(lock2, OUTPUT);
    pinMode(ventilador, OUTPUT);
    pinMode(enfriador, OUTPUT);
    digitalWrite(ventilador, LOW);
    digitalWrite(enfriador,LOW);
    lcd.setCursor(0,0);
    lcd.println("INICIANDO.......");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,1);
   lcd.println("--INGRESE LOCK#-");

}      

void loop(){

  Serial.print("Temperature 1: ");
  Serial.print(getTemp("c", 1));// get DHT1 temperature in C 
  lcd.setCursor(11,0);
  lcd.print(getTemp("c", 1));
   lcd.setCursor(4,0);
  lcd.println(getTemp("h", 1));
  Serial.print(" *C ");
  Serial.print("Temperature 2: ");
  Serial.print(getTemp("c", 2));// get DHT2 temperature in C 
  Serial.print(" *C ");

if (Serial.available())

{

int state = Serial.parseInt();

if (state == 4545)

{
digitalWrite(13, HIGH);
digitalWrite(lock1, LOW);
delay(500);
digitalWrite(lock1, HIGH);
Serial.println("LOCKER 1 ABIERTO");


}

if (state == 1234)

{

digitalWrite(lock2, LOW);
delay(500);
digitalWrite(lock2, HIGH);
Serial.println("LOCKER 2 ABIERTO");

}

}
  char key = keypad.getKey();

   digitalWrite(lock1,HIGH);
   digitalWrite(lock2,HIGH);
 

  
  if (key){
   lcd.setCursor(0,0);
   lcd.print(key);
   Serial.println(key);
  

    if(key == '*') 
    
    {
      input_password = ""; // reset the input password
    } 
    
    else if(key == '#') 
    
    {
      if(input_password == password_1 || input_password == password_2  ) {
        Serial.println("LOCKER 1 ABIERTO");
        digitalWrite(lock1,LOW);
          lcd.setCursor(0,0);
                 lcd.println("----LOCKER 1----");
                 delay(1000);
                 lcd.clear();
                 lcd.setCursor(0,1);
                lcd.println("--INGRESE LOCK#-");
        // DO YOUR WORK HERE
      }
      
      else if (input_password == password_3 || input_password == password_4)
    {
        lcd.setCursor(0,0);
        Serial.println("LOCKER 2 ABIERTO");
        digitalWrite(lock2,LOW);
        lcd.println("----LOCKER 2----");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,1);
                lcd.println("--INGRESE LOCK#-");
      }
      else{
        Serial.println("INCORRECTO");
        }

      input_password = ""; // reset the input password
    } 
    else 
    {
      input_password += key; // append new character to input password string
    }
  }
 
}

float getTemp(String req, int dhtCount)
{

if(dhtCount ==1){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h1 = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  float t1 = dht1.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f1 = dht1.readTemperature(true);

  // Compute heat index in Fahrenheit (the default)
  float hif1 = dht1.computeHeatIndex(f1, h1);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic1 = dht1.computeHeatIndex(t1, h1, false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h1) || isnan(t1) || isnan(f1)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  

  // Compute heat index in Kelvin 
  float k1 = t1 + 273.15;
  if(req =="c"){
    return t1;//return Cilsus
  }else if(req =="f"){
    return f1;// return Fahrenheit
  }else if(req =="h"){
    return h1;// return humidity
  }else if(req =="hif"){
    return hif1;// return heat index in Fahrenheit
  }else if(req =="hic"){
    return hic1;// return heat index in Cilsus
  }else if(req =="k"){
    return k1;// return temprature in Kelvin
  }else{
    return 0.000;// if no reqest found, retun 0.000
  }
}// DHT1 end


if(dhtCount ==2){
    float h1 = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  float t1 = dht1.readTemperature();
  if(t1<=20){
      digitalWrite(enfriador,HIGH);
    }
  
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f1 = dht1.readTemperature(true);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h2 = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  float t2 = dht2.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f2 = dht2.readTemperature(true);

  // Compute heat index in Fahrenheit (the default)
  float hif2 = dht2.computeHeatIndex(f1, h1);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic2 = dht2.computeHeatIndex(t2, h2, false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h2) || isnan(t2) || isnan(f2)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  

  // Compute heat index in Kelvin 
  float k2 = t2 + 273.15;
  if(req =="c"){
    return t2;//return Cilsus
  }else if(req =="f"){
    return f2;// return Fahrenheit
  }else if(req =="h"){
    return h2;// return humidity
  }else if(req =="hif"){
    return hif2;// return heat index in Fahrenheit
  }else if(req =="hic"){
    return hic2;// return heat index in Cilsus
  }else if(req =="k"){
    return k2;// return temprature in Kelvin
  }else{
    return 0.000;// if no reqest found, retun 0.000
  }
}// DHT2 end

}//getTemp end
