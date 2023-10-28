#include <Servo.h>

Servo motoHori;
int angleMotoH = 300;
int motoHoriLimitHigh = 300;
int motoHoriLimitLow = 65;

Servo motoVerti;
int angleMotoV = 45;
int motoVertiLimitHigh = 100;
int motoVertiLimitLow = 15 ;

int ldrtopR = 0;
int ldrtopL = 1;
int ldrbotR = 2;
int ldrbotL = 3;


void setup() {
  Serial.begin(9600);
  motoHori.attach(9);
  motoHori.write(angleMotoH);
  motoVerti.attach(10);
  motoVerti.write(angleMotoV);
  delay(300);

}

void loop() {

  int topR = analogRead(ldrtopR);
  //delay(10);
  int topL = analogRead(ldrtopL);
  //delay(10);
  int botR = analogRead(ldrbotR);
  //delay(10);
  int botL = analogRead(ldrbotL);

  int dtimev = 10;
  int tol = 50;

  int avt = (topL + topR) / 2; 
  int avd = (botL+ botR) / 2; 
  int avl = (topL + botL) / 2; 
  int avr = (topR+ botR) / 2; 
  int dvert = avt - avd; 
  int dhoriz = avl - avr;

  Serial.println(avt);
  Serial.println("-------avt--------");
  Serial.println(avd);
  Serial.println("-------avd--------");
  Serial.println(avl);
  Serial.println("-------avl--------");
  Serial.println(avr);
  Serial.println("-------avr--------");

  delay(100);

//  if (angleMotoV >= motoVertiLimitLow ){
//    motoVerti.write(angleMotoV);
////    Serial.println(angleMotoV);
//    Serial.println("===--==");
//    angleMotoV -=1;
//    delay(100);
//  }
//
//   if (angleMotoV <= motoVertiLimitHigh){
//    motoVerti.write(angleMotoV);
//    Serial.println("===++==");
//    Serial.println(angleMotoV);
//    angleMotoV +=1;
//    delay(100);
//  }

//  if (angleMotoH >= motoHoriLimitLow){
//    motoHori.write(angleMotoH);
//    angleMotoH -= 1;
//    delay(100);
//  }
//  if (angleMotoH <= motoHoriLimitHigh){
//    motoHori.write(angleMotoH);
//    angleMotoH += 1;
//    delay(10);
//  }



  if (-1*tol > dvert || dvert > tol){
    if (avt > avd){
      angleMotoV = ++angleMotoV;
      if (angleMotoV > motoVertiLimitHigh)
      {
        angleMotoV = motoVertiLimitHigh;
      }
    }
    else if (avt < avd){
      angleMotoV = --angleMotoV;
      if (angleMotoV < motoVertiLimitLow){
        angleMotoV = motoVertiLimitLow;
      }
    }
    motoVerti.write(angleMotoV);
  }

  if (-1*tol > dhoriz || dhoriz > tol){
    if (avl > avr){
      angleMotoH = --angleMotoH;
      if (angleMotoH < motoHoriLimitLow){
        angleMotoH = motoHoriLimitLow;  
      }
    }
    else if (avl < avr){
      angleMotoH = ++angleMotoH;
      if (angleMotoH > motoHoriLimitHigh){
        angleMotoH = motoHoriLimitHigh;
      }
    }
    else if (avl = avr){
      // nothing
    }
    motoHori.write(angleMotoH);
  }
  delay(10);
  
}
