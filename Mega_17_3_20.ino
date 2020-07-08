//Arduino code
#include "LiquidCrystal.h"
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7
#include <SoftwareSerial.h>

SoftwareSerial s(5 , 6);
//char recStr[10] = "";
const int buz=A6;

int f;
int apin0 = A0;
int apin1 = A1;
int apin2 = A2;
int apin3 = A3;
int apin4 = A4;
int apin5 = A5;
//
float c1 = 0, c2 = 0, c3 = 0, v1 = 0, v2 = 0, v3 = 0;
float pc1 = 0, pc2 = 0, pc3 = 0, pv1 = 0, pv2 = 0, pv3 = 0;
float rc1 = 0, rc2 = 0, rc3 = 0, rv1 = 0, rv2 = 0, rv3 = 0;
float tc1 = 0, tc2 = 0, tc3 = 0, tv1 = 0, tv2 = 0, tv3 = 0;

float R1 = 30000.0;
float R2 = 7500.0;

void setup() {
  
  pinMode(apin0, INPUT);
  pinMode(apin1, INPUT);
  pinMode(apin2, INPUT);
  pinMode(apin3, INPUT);
  pinMode(apin4, INPUT);
  pinMode(apin5, INPUT);
  pinMode(buz,OUTPUT);
  lcd.begin(16, 2);
  //lcd.print(" Measure > 25V  ");

  Serial.begin(9600);
  Serial1.begin(115200);

}

void loop()
{
/*.......................Read input Voltage.......................*/
  rv1 = analogRead(apin3);
  rv2 = analogRead(apin4);
  rv3 = analogRead(apin5);

/*.......................Current Calculation.......................*/
  for (int x = 0; x < 150; x++) {
    rc1 = analogRead(apin0);     
    tc1 = tc1 + rc1;  
    rc2 = analogRead(apin1);
    tc2 = tc2 + rc2;  
    rc3 = analogRead(apin2); 
    tc3 = tc3 + rc3;  
    delay(3);
  }
  pc1 = tc1 / 150.0;
  c1 = (2.5 - (pc1 * (5.0 / 1024.0)) ) / 0.066;
  pc2 = tc2 / 150.0;
  c2 = (2.5 - (pc2 * (5.0 / 1024.0)) ) / 0.066;
  pc3 = tc3 / 150.0;
  c3 = (2.5 - (pc3 * (5.0 / 1024.0)) ) / 0.066;

  //  Serial.print(rc1);
  //  delay(50);
  //  Serial.print("c1= ");
  //  Serial.print(c1);
  //  Serial.print(" c2= ");
  //  Serial.print(c2);
  //  Serial.print(" c3= ");
  //  Serial.print(c3);
  //  Serial.print(" v1=");
  //  Serial.print(v1);
  //  Serial.print(" v2=");
  //  Serial.print(v2);
  //  Serial.print(" v3=");
  //  Serial.println(v3);

/*.......................Voltage Calculation.......................*/
  tv1 = (rv1 * 5.0) / 1024.0;
  v1 = tv1 / (R2 / (R1 + R2));

  tv2 = (rv2 * 5.0) / 1024.0;
  v2 = tv2 / (R2 / (R1 + R2));

  tv3 = (rv3 * 5.0) / 1024.0;
  v3 = tv3 / (R2 / (R1 + R2));

  /*.......................Do not touch.......................*/
  if (Serial1.available() != 0)
  {
    f = Serial1.read();
    //Serial.println(f);
    delay(10);
    switch (f)
    {
      case 48: Serial1.print(c1); break;
      case 49: Serial1.print(c2); break;
      case 50: Serial1.print(c3); break;
      case 51: Serial1.print(v1); break;
      case 52: Serial1.print(v2); break;
      case 53: Serial1.print(v3); break;
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("PC_");
  lcd.setCursor(4, 0);
  lcd.print(c1);
  lcd.setCursor(8, 0);
  lcd.print(c2);
  lcd.setCursor(12, 0);
  lcd.print(c3);

  lcd.setCursor(0, 1);
  lcd.print("PV_");
  lcd.setCursor(4, 1);
  lcd.print(v1);
  lcd.setCursor(8, 1);
  lcd.print(v2);
  lcd.setCursor(12, 1);
  lcd.print(v3);

  if(c1>2){lcd.setCursor(4, 0);lcd.print("Over");digitalWrite(buz,HIGH);}
  if(c2>2){lcd.setCursor(8, 0);lcd.print("Over");digitalWrite(buz,HIGH);}
  if(c3>2){lcd.setCursor(12, 0);lcd.print("Over");digitalWrite(buz,HIGH);}
  if(v1>5){lcd.setCursor(4, 1);lcd.print("Over");digitalWrite(buz,HIGH);}
  if(v2>5){lcd.setCursor(8, 1);lcd.print("Over");digitalWrite(buz,HIGH);}
  if(v3>5){lcd.setCursor(12, 1);lcd.print("Over");digitalWrite(buz,HIGH);}
  
}
