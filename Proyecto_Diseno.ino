#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define BitsContra 8 
int lock1 = 2;
int lock2=3;
//definicion de las contrasenas
char Data[BitsContra]; 
char Master[BitsContra] = "123A456"; 


byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

//definicion del keypad
const byte R = 4;
const byte C = 4;
char hexaKeys[R][C] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[R] = {11, 10, 9, 8};
byte colPins[C] = {7, 6, 5, 4};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, R, C);

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup(){
  lcd.begin();
  lcd.backlight();
  pinMode(lock1, OUTPUT);
    pinMode(lock2, OUTPUT);

}

void loop(){

  lcd.setCursor(0,0);
  lcd.print("INGRESE LOCK # ");

  customKey = customKeypad.getKey();
  if (customKey){
    Data[data_count] = customKey; 
    lcd.setCursor(data_count,1); 
    lcd.print(Data[data_count]); 
    data_count++; 
    }

  if(data_count == BitsContra-1){
    lcd.clear();

    if(!strcmp(Data, Master)){
      lcd.print("Correct");
      digitalWrite(lock1, HIGH); 
      delay(5000);
      digitalWrite(lock1, LOW);
      }
    else{
      lcd.print("Incorrect");
      delay(1000);
      }
    
    lcd.clear();
    clearData();  
  }
}

void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0; 
  }
  return;
}
