#include <Arduino.h>

#include <avr/wdt.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "definice.h"

#define pinOhrevA 5
#define pinOhrevB 6
#define pinSvetloA 9
#define pinSvetloB 10
#define pinSvetloC 11
#define pinMotorXpwm 3
#define pinMotorXdig 4
#define pinMotorZ1 17
#define pinMotorZ2 16
#define pinMotorZ3 15
#define pinMotorZ4 14
#define pinTepSCL 19
#define pinTepSDA 18
#define pinSerTX 1
#define pinSerRX 0

// inicializace teploměru MLX z knihovny
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

char mode;
String value = String(PrazdnePole);
String dotaz = String(Dotaz);
dtOhrev ohrevA;
dtOhrev ohrevB;
dtSvetlo svetloA;
dtSvetlo svetloB;
dtSvetlo svetloC;
dtMotorX motorX;
dtMotorZ motorZ;
dtTep tepOkolyD[8];
dtTep tepNadrzD[8];

char motorZscit;
byte motorZkrok;
byte motorZopak;
const bool motorZMetrix[5][4]= {{1,0,1,0},{0,1,1,0},{0,1,0,1},{1,0,0,1},{0,0,0,0}};
long motorZtime;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
mlx.begin(); 
pinMode(pinOhrevA, OUTPUT);
pinMode(pinOhrevB, OUTPUT);
pinMode(pinSvetloA , OUTPUT);
pinMode(pinSvetloB , OUTPUT);
pinMode(pinSvetloC , OUTPUT);
pinMode(pinMotorXpwm , OUTPUT);
pinMode(pinMotorXdig , OUTPUT);
pinMode(pinMotorZ1 , OUTPUT);
pinMode(pinMotorZ2 , OUTPUT);
pinMode(pinMotorZ3 , OUTPUT);
pinMode(pinMotorZ4 , OUTPUT);
pinMode(13 , OUTPUT);
ohrevA = 0;
ohrevB = 0;
svetloA = 0;
svetloB =0;
svetloC =0;
motorX = 0;
motorZ = 0;
motorZkrok=0;
motorZtime=0;
}


void loop() {
  if(Serial.available() >= 6)
  {
    mode = Serial.read();
    
    value.setCharAt(0,Serial.read());
    value.setCharAt(1,Serial.read());
    value.setCharAt(2,Serial.read());
    value.setCharAt(3,Serial.read());
    Serial.read();

    switch(mode)
    {
      case modOhrevA:
      Ohrev(mode,&ohrevA,pinOhrevA);
      break;
      case modOhrevB:
      Ohrev(mode,&ohrevB,pinOhrevB);
      break;
      case modSvetloA:
      Svetlo(mode,&svetloA,pinSvetloA)  ;
      break;
      case modSvetloB:
      Svetlo(mode,&svetloB,pinSvetloB)  ;
      break;
      case modSvetloC:
      Svetlo(mode,&svetloC,pinSvetloC)  ;
      break;    
      case modMotorX:
      MotorXB()  ;
      break; 
      case modMotorZ:
      MotorZC()  ;
      break; 
      case modTepNadrz:
      TepNadrzB();
      break;
      case modTepOkoli:
      TepOkoliB();
      break;
      default:
      Serial.print(Problem);
      break;
    }
  }
  MotorZrun();
}


void Ohrev(char mod,dtOhrev *data, int pin)
{
  if(notSend(mod,*data))
  {
  *data = value.toInt();
   analogWrite(pin, *data);
  }
}


void Svetlo(char mod,dtSvetlo *data, int pin)
{
  if(notSend(mod,*data))
  {
  *data = value.toInt();
   analogWrite(pin, *data);
  }
}


void MotorXB()
{
  if(notSend(modMotorX,motorX))
  {
    dtMotorX novi = value.toInt();
    if(novi == levoSMotorX )
    {
    digitalWrite(pinMotorXdig, 0);
    digitalWrite(pinMotorXpwm, 1);
    digitalWrite(13, 1);
    delay(50);
    }
    else if(novi == levoBMotorX)
        {
    digitalWrite(pinMotorXdig, 0);
    digitalWrite(pinMotorXpwm, 1);
    delay(200);
    }
    else if(novi == pravSMotorX)
        {
    digitalWrite(pinMotorXdig, 1);
    digitalWrite(pinMotorXpwm, 0);
    delay(50);
    }
    else if(novi == pravBMotorX)
        {
    digitalWrite(pinMotorXdig, 1);
    digitalWrite(pinMotorXpwm, 0);
    delay(200);
    }
    digitalWrite(pinMotorXdig, 0);
    digitalWrite(pinMotorXpwm, 0);
    delay(5000);
    digitalWrite(13, 0);
  }
}

/*
void MotorX()
{
  if(value.charAt(2) == '?')
  {
  char c[5];
  sprintf(c, "%c%4d", modMotorX,motorX);
  Serial.print(c);
  }
  else
  {
    dtMotorX novi = value.toInt();
    if(novi >= minMotorX && novi <= maxMotorX )
    {
      motorX = novi;
      if(motorX == stopMotorX)
      {
        digitalWrite(pinMotorXdig, 0);
        analogWrite(pinMotorXpwm, 0);
      }
      else if(motorX > 0)
      {
        digitalWrite(pinMotorXdig, 0);
        analogWrite(pinMotorXpwm, motorX);
      }
      else
      {
        digitalWrite(pinMotorXdig, 1);
        analogWrite(pinMotorXpwm, ~(-motorX));
      }
    }
  }
}*/


void TepOkoliB()
{
  /*char c[5];
  sprintf(c, "%c%4d", modTepOkoli,TepControl(mlx.readAmbientTempC()));
  Serial.print(c);*/
  notSend(modTepNadrz,TepControl(mlx.readAmbientTempC()));
}


void TepNadrzB()
{
  /*char c[5];
  sprintf(c, "%c%4d", modTepNadrz,TepControl(mlx.readObjectTempC()));
  Serial.print(c);*/
  notSend(modTepNadrz,TepControl(mlx.readObjectTempC()));
}


int TepControl(dtTep a)
{
  int u=(double)(a*tepNas);
  if(u<minTep*tepNas) {u = minTep*tepNas;}
  if(u>maxTep*tepNas) {u = maxTep*tepNas;}
  return u;
}


void MotorZC()
{
  if(notSend(modMotorZ,motorZ))
  {
    dtMotorZ novi = value.toInt();
    if(novi == stopMotorZ )
    {
       motorZkrok = 4;
       motorZ = stopMotorZ;
       MotorZindex(4);
    }
    else if(novi == nahorumotorZ)
    {
      for(int i = 0; i <= 32;  i++)
      {
        MotorZindex(i%4);
        delay(10);
      }
    } 
    else if(novi == dolumotorZ)
    {
      for(int i = 32; i >=0;  i--)
      {
        MotorZindex(i%4);
        delay(10);
      }
    }  
    else if(novi == vibMotorZOn)
    {
       motorZkrok = 0;
       motorZ = vibMotorZOn;
       motorZtime = millis();
       motorZscit = 1;
    }
    MotorZindex(4);
  }
}


void MotorZrun()
{
  if(motorZ == vibMotorZOn && motorZtime < millis())
  {
    MotorZindex(motorZkrok);
    motorZtime = millis() + 10;
    if(motorZkrok == 3)
    {
      motorZscit = -1;
    }
    else if(motorZkrok == 0)
    {
      motorZscit = 1;
    }
    motorZkrok += motorZscit;
    
  }
}


void MotorZindex(int index)
{
    digitalWrite(pinMotorZ1, motorZMetrix[index][0]);
    digitalWrite(pinMotorZ2, motorZMetrix[index][1]);
    digitalWrite(pinMotorZ3, motorZMetrix[index][2]);
    digitalWrite(pinMotorZ4, motorZMetrix[index][3]);
}


bool notSend(char typ, int data)
{
  if(value.charAt(2) == '?')
  {
  char c[6];
  sprintf(c, "%c%4d\n", typ,data);
  Serial.print(c);
  return false;
  } 
  return true;
}
