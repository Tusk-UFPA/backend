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

float tensaoEntrada = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DE TENSÃO DE ENTRADA DO SENSOR
float tensaoMedida = 0.0; //VARIÁVEL PARA ARMAZENAR O VALOR DA TENSÃO MEDIDA PELO SENSOR
 
float valorR1 = 30000.0; //VALOR DO RESISTOR 1 DO DIVISOR DE TENSÃO
float valorR2 = 7500.0; // VALOR DO RESISTOR 2 DO DIVISOR DE TENSÃO
int leituraSensor = 0; //VARIÁVEL PARA ARMAZENAR A LEITURA DO PINO ANALÓGICO
 
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

  pinMode(pinoSensor, INPUT); //DEFINE O PINO COMO ENTRADA
  
  delay(100);
}

void loop() {
  
  leituraSensor = analogRead(pinoSensor); //FAZ A LEITURA DO PINO ANALÓGICO E ARMAZENA NA VARIÁVEL O VALOR LIDO
  tensaoEntrada = (leituraSensor * 5.0) / 1024.0; //VARIÁVEL RECEBE O RESULTADO DO CÁLCULO
  tensaoMedida = tensaoEntrada / (valorR2/(valorR1+valorR2)); //VARIÁVEL RECEBE O VALOR DE TENSÃO DC MEDIDA PELO SENSOR
  
  Serial.print("Tensão DC medida: "); //IMPRIME O TEXTO NA SERIAL
  Serial.print(tensaoMedida,2); //IMPRIME NA SERIAL O VALOR DE TENSÃO DC MEDIDA E LIMITA O VALOR A 2 CASAS DECIMAIS
  Serial.println("V"); //IMPRIME O TEXTO NA SERIAL
  delay(100); //INTERVALO DE 500 MILISSEGUNDOS

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

//  Serial.println(avt);
//  Serial.println("-------avt--------");
//  Serial.println(avb);
//  Serial.println("-------avd--------");
//  Serial.println(avl);
//  Serial.println("-------avl--------");
//  Serial.println(avr);
//  Serial.println("-------avr--------");

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
