#include <Arduino.h>

#include <avr/wdt.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>



#define modOhrevA 'A'
#define modOhrevB 'B'
#define modSvetloA 'C'
#define modSvetloB 'D'
#define modSvetloC 'E'
#define modMotorX 'F'
#define modMotorZ 'H'
#define modTepNadrz 'I'
#define modTepOkoli 'K'

#define dtOhrev uint8_t
#define dtSvetlo uint8_t
#define dtMotorX int16_t
#define dtMotorZ int8_t
#define dtTep float

#define maxTep 99.0
#define maxMotorX 255
#define maxSvetlo 255
#define maxOhrev 255
#define minTep 20.0
#define minMotorX -255
#define minSvetlo 0
#define minOhrev 0

#define vibMotorZOn 3
#define stopMotorZ 0
#define stopMotorX 0

#define nahorumotorZ 2
#define dolumotorZ 1

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
#define Dotaz "????"

// inicializace teploměru MLX z knihovny
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

char mode;
String value = String("0000");
String dotaz = String(Dotaz);
dtOhrev ohrevA;
dtOhrev ohrevB;
dtSvetlo svetloA;
dtSvetlo svetloB;
dtSvetlo svetloC;
dtMotorX motorX;
dtMotorZ motorZ;
char motorZnas;
byte motorZkrok;
byte motorZopak;
const bool motorZMetrix[5][4]= {{1,0,1,0},{0,1,1,0},{0,1,0,1},{1,0,0,1},{0,0,0,0}};
long motorZtimeKrok = 10;
long motorZtime;

double tepOkolyD[8];
double tepNadrzD[8];
byte tepkrok;
long tepTime;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("Start1");
mlx.begin(); 
Serial.println("Start2");
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

ohrevA = 0;
ohrevB = 0;
svetloA = 0;
svetloB =0;
svetloC =0;
motorX = 0;
motorZ = 0;
motorZkrok=0;
motorZtime=0;
tepkrok = 0;
tepTime = millis();
Serial.println("Start3");



/*
        digitalWrite(pinMotorXdig, 0);
        digitalWrite(pinMotorXpwm, 1);
        delay(10000);

                digitalWrite(pinMotorXdig, 1);
        digitalWrite(pinMotorXpwm, 0);
        delay(10000);
                digitalWrite(pinMotorXdig, 0);
        digitalWrite(pinMotorXpwm, 1);
        delay(10000);

                digitalWrite(pinMotorXdig, 1);
        digitalWrite(pinMotorXpwm, 0);
        delay(10000);
        Serial.println("Start4");*/
}



void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(Serial.available());
  if(Serial.available() >= 6)
  {
    mode = Serial.read();
    
    value.setCharAt(0,Serial.read());
    value.setCharAt(1,Serial.read());
    value.setCharAt(2,Serial.read());
    value.setCharAt(3,Serial.read());
    Serial.read();
    Serial.println(mode);
    Serial.println(value);
    //delay(1000);
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
      MotorX()  ;
      break; 
      case modMotorZ:
      MotorZC()  ;
      break; 
      case modTepNadrz:
      //TepNadrz();
      TepNadrzB();
      break;
      case modTepOkoli:
      //TepOkoli();
      TepOkoliB();
      break;
      default:
      Serial.print("eeeee");
      break;
    }
  }
  //MotorZrun();
}

void Ohrev(char mod,dtOhrev *data, int pin)
{
  Serial.println("a");
  if(value.charAt(2) == '?')
  {
    Serial.println("b");
  char c[5];
  sprintf(c, "%c%4d", mod,*data);
  Serial.print(c);
  }
  else {
    Serial.println("c");Serial.println(value);
  *data = value.toInt();
   analogWrite(pin, *data);
  }
}

void Svetlo(char mod,dtSvetlo *data, int pin)
{
  if(value.charAt(2) == '?')
  {
  char c[5];
  sprintf(c, "%c%4d", mod,*data);
  Serial.print(c);
  }
  else {
  *data = value.toInt();
   analogWrite(pin, *data);
   //if(value.toInt() == 255){digitalWrite(pin,1);Serial.println("Skdnlkwnlkcn");}
   //else
   //{
   // digitalWrite(pin,0);Serial.println("Skdnlkwnlkcn0");
   //}
  }
}

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
        Serial.println("st");
      }
      else if(motorX > 0)
      {
        digitalWrite(pinMotorXdig, 0);
        analogWrite(pinMotorXpwm, motorX);
        Serial.println("AA");
      }
      else
      {
        digitalWrite(pinMotorXdig, 1);
        analogWrite(pinMotorXpwm, ~(-motorX));
        Serial.println("BB");
      }
    }
  }
}

void MotorXB()
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
    if(novi ==0)
    {
              digitalWrite(pinMotorXdig, 0);
        digitalWrite(pinMotorXpwm, 0);
    }
    if(novi == minMotorX)
    {
              digitalWrite(pinMotorXdig, 1);
        digitalWrite(pinMotorXpwm, 0);
    }
    if(novi == maxMotorX)
    {
              digitalWrite(pinMotorXdig, 0);
        digitalWrite(pinMotorXpwm, 1);
    }
  }
}



void TepOkoliB()
{
  char c[5];
  int u=(double)(mlx.readAmbientTempC()*100.0);
  if(u<minTep*100) {u = minTep*100;}
  if(u>maxTep*100) {u = maxTep*100;}
  sprintf(c, "%c%4d", modTepOkoli,u);
  Serial.print(c);
}


void TepNadrzB()
{
  char c[5];
  //  sprintf(c, "%c%4d", modTepNadrz,(int)((double)(mlx.readObjectTempC()*100.0)));
    int u=(double)(mlx.readObjectTempC()*100.0);
  if(u<minTep*100) {u = minTep*100;}
  if(u>maxTep*100) {u = maxTep*100;}
  sprintf(c, "%c%4d", modTepOkoli,u);
  Serial.print(c);
}

void MotorZC()
{
if(value.charAt(2) == '?')
  {
  char c[5];
  sprintf(c, "%c%4d", modMotorZ,motorZ);
  Serial.print(c);
  }
  else 
  {
  dtMotorZ novi = value.toInt();
  Serial.println(novi);
   if(novi == stopMotorZ )
   {
    digitalWrite(pinMotorZ1,0);
    digitalWrite(pinMotorZ2, 0);
    digitalWrite(pinMotorZ3,0);
    digitalWrite(pinMotorZ4, 0);
     
   }
  else if(novi == nahorumotorZ)
  {
    Serial.print("6265gyuu");
         for(int i = 0; i <= 32;  i++)
     {

    digitalWrite(pinMotorZ1, motorZMetrix[i%4][0]);
    digitalWrite(pinMotorZ2, motorZMetrix[i%4][1]);
    digitalWrite(pinMotorZ3, motorZMetrix[i%4][2]);
    digitalWrite(pinMotorZ4, motorZMetrix[i%4][3]);
    //Serial.print("i");
    delay(10);

     }
     Serial.print("p");
  } 
  else if(novi == dolumotorZ)
  {
           for(int i = 32; i >=0;  i--)
     {

    digitalWrite(pinMotorZ1, motorZMetrix[i%4][0]);
    digitalWrite(pinMotorZ2, motorZMetrix[i%4][1]);
    digitalWrite(pinMotorZ3, motorZMetrix[i%4][2]);
    digitalWrite(pinMotorZ4, motorZMetrix[i%4][3]);
    //Serial.print("6");
    delay(10);

     }
  }  
   else if(novi == vibMotorZOn)
   {
     motorZkrok = 1;
     motorZ = vibMotorZOn;
     motorZtime = millis();
     motorZnas = 1;
   }
       digitalWrite(pinMotorZ1, motorZMetrix[4][0]);
    digitalWrite(pinMotorZ2, motorZMetrix[4][1]);
    digitalWrite(pinMotorZ3, motorZMetrix[4][2]);
    digitalWrite(pinMotorZ4, motorZMetrix[4][3]);
  }
}

void MotorZ()
{
if(value.charAt(2) == '?')
  {
  char c[5];
  sprintf(c, "%c%4d", modMotorZ,motorZ);
  Serial.print(c);
  }
  else 
  {
  dtMotorZ novi = value.toInt();
  Serial.println(novi);
   if(novi == stopMotorZ )
   {
     motorZ = stopMotorZ;
     MotorZstop();
     motorZkrok = 4;
     
   }
  else if(novi == nahorumotorZ)
  {
    motorZ = nahorumotorZ;
    motorZtime = millis();
    motorZkrok = 0;
    motorZopak = 100;
  } 
  else if(novi == dolumotorZ)
  {
    motorZ = dolumotorZ;
    motorZtime = millis();
    motorZkrok = 3;
    Serial.println("jo");
    Serial.println(motorZkrok);
    motorZopak = 100;
  }  
   else if(novi == vibMotorZOn)
   {
     motorZkrok = 0;
     motorZ = vibMotorZOn;
     motorZtime = millis();
     motorZnas = 1;
   }
  }
}


void MotorZrun()
{

  if(motorZ != 0 && motorZtime < millis())
  {
    digitalWrite(pinMotorZ1, motorZMetrix[motorZkrok][0]);
    digitalWrite(pinMotorZ2, motorZMetrix[motorZkrok][1]);
    digitalWrite(pinMotorZ3, motorZMetrix[motorZkrok][2]);
    digitalWrite(pinMotorZ4, motorZMetrix[motorZkrok][3]);

    //Serial.print( motorZMetrix[motorZkrok][0]);
    //Serial.print( motorZMetrix[motorZkrok][1]);
    //Serial.print( motorZMetrix[motorZkrok][2]);
    //Serial.println( motorZMetrix[motorZkrok][3]);
    //Serial.println( motorZkrok);
    motorZtime = millis() + 10;
    if(motorZ == nahorumotorZ)
    {
      motorZkrok++;
      if(motorZkrok == 3)
      {
        if(motorZopak != 0)
        {
        motorZkrok = 0;
        motorZopak --;
        }
        else{
        motorZkrok ++;
        motorZ = 0;
        }
      }
    }
    else if(motorZ == dolumotorZ)
    {
      if(motorZopak != 0)
      {
        if(motorZkrok == 1 )
        {
          motorZopak --;
          motorZkrok = 4;
        }
        else
        {
          motorZkrok--;
        }
      }
      else 
      {
        motorZ = 0;
      }

    }
    else if(motorZ == vibMotorZOn)
    {
      if(motorZkrok == 4)
      {
        motorZnas = -1;
      }
      else if(motorZkrok == 1)
      {
        motorZnas = 1;
      }
      motorZkrok += motorZnas;
    }
    else
    {
      motorZkrok = 0;
    }
  }
}





void MotorZstop()
{
    digitalWrite(pinMotorZ1, motorZMetrix[4][0]);
    digitalWrite(pinMotorZ2, motorZMetrix[4][1]);
    digitalWrite(pinMotorZ3, motorZMetrix[4][2]);
    digitalWrite(pinMotorZ4, motorZMetrix[4][3]);
}
