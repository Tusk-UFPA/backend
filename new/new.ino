#include <Servo.h>

Servo motoHori;
int angleMotoH = 0;
int motoHoriLimitHigh = 180;
int motoHoriLimitLow = 45;

Servo motoVerti;
int angleMotoV = 15;
int motoVertiLimitHigh = 140;
int motoVertiLimitLow = 15 ;

// as cores dos cabos para os sensores
int ldrtopR = 0; //verde
int ldrtopL = 1; //vermelho
int ldrbotR = 2; //branco
int ldrbotL = 3; //preto

void setup() {
  Serial.begin(9600);
  motoHori.attach(9);
//  motoHori.write(0);
  motoVerti.attach(10);
//  motoVerti.write(0);

  int restMotoV = motoVerti.read();
  int restMotoH = motoHori.read();
  
  Serial.println(restMotoV);
  for(restMotoV > angleMotoV; --restMotoV;){
    motoVerti.write(restMotoV);
    delay(50);
  }

  for(restMotoH < angleMotoH; --restMotoH;){
    motoHori.write(restMotoH);
    delay(50);
  }
  delay(100);
}

void loop() {

  int topR = analogRead(ldrtopR);
  int topL = analogRead(ldrtopL);
  int botR = analogRead(ldrbotR);
  int botL = analogRead(ldrbotL);

//  int dtimev = 10;
  int tol = 50;

  int avt = (topL + topR) / 2; 
  int avb = (botL + botR) / 2; 
  int avl = (topL + botL) / 2; 
  int avr = (topR + botR) / 2; 
  int dverti = avt - avb; 
  int dhori = avl - avr;

  Serial.println(avt);
  Serial.println("-------avt--------");
  Serial.println(avb);
  Serial.println("-------avd--------");
  Serial.println(avl);
  Serial.println("-------avl--------");
  Serial.println(avr);
  Serial.println("-------avr--------");

  delay(10);
  
  if(-1*tol > dverti || dverti > tol){
    if(avt > avb){
      angleMotoV = ++angleMotoV;
      if(angleMotoV > motoVertiLimitHigh){
        angleMotoV = motoVertiLimitHigh;
      }
      
//      motoVerti.write(angleMotoV);
//      delay(10);
    }else if(avt < avb){
      angleMotoV = --angleMotoV;
      if(angleMotoV < motoVertiLimitLow){
        angleMotoV = motoVertiLimitLow;
      }
         
//      delay(10);
    }else if(avt = avb){
  //    motoVerti.write(angleMotoV);
  //    delay(10);
  //    faz nada
    }
    motoVerti.write(angleMotoV);
  }
  delay(10);
  
  if (-1*tol > dhori || dhori > tol){
   
    if(avl < avr){
      angleMotoH = ++angleMotoH;
     if(angleMotoH > motoHoriLimitHigh){
        angleMotoH = motoHoriLimitHigh;
      }
      
//      delay(10);
      
    }else if(avl > avr){
      angleMotoH = --angleMotoH;
       if(angleMotoH < motoHoriLimitLow){
        angleMotoH = motoHoriLimitLow;
      }
//      delay(10);
    }else if(avl = avr) {
  //    motoVerti.write(angleMotoV);
  //    delay(10);
  //      faz nada
    }
    motoHori.write(angleMotoH);
  }
  
  delay(100);
}
