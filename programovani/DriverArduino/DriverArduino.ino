/*
Program pro Arduino Nano rev3.0



*/
#include <Arduino.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "definice.h"

//Seznam zapojenych pinu:
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
//pini, ktere jsou nastaveny jinak nez pres pinMode(...)
#define pinTepSCL 19
#define pinTepSDA 18
#define pinSerTX 1
#define pinSerRX 0

// inicializace teploměru MLX z knihovny
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


//Promene dostupne globalne
char mode;//instrukce
// default String
String value = String(PrazdnePole);//Pouzivan jako predavac informace o datech ukrite v instrukci.
String dotaz = String(Dotaz);
// inicializace globalnich promenych
dtOhrev ohrevA;
dtOhrev ohrevB;
dtSvetlo svetloA;
dtSvetlo svetloB;
dtSvetlo svetloC;
dtMotorX motorX;
dtMotorZ motorZ;

//promene motoruZ
int motorZscit;//urcuje smer pohibu pri vibracich
byte motorZkrok;//Krok motoru Z
const bool motorZMetrix[5][4]= {{1,0,1,0},{0,1,1,0},{0,1,0,1},{1,0,0,1},{0,0,0,0}};//vystupy pinu vuci krokum
long long motorZtime;//cas dalsiho kroku

void setup() 
{
  Serial.begin(9600);// inicializace UART
  mlx.begin(); // inicializace teploměru MLX z knihovny

  //Nastaveni pinu
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

  //Inicializace promenych
  //Ohrev: Aktivni vyuziti, rozsah 0-255
  ohrevA = 0;
  ohrevB = 0;
  //Svetlo: Aktivni vyuziti, rozsah 0-255
  svetloA = 0;
  svetloB =0;
  svetloC =0;
  //MotorX: default:0
  motorX = 0;
  //Svetlo: Aktivni vyuziti, rozsah mezi 0 a 4
  motorZ = 0;
  //Systemove promene pro motor X
  motorZkrok=0;
  motorZtime=0;
}


void loop() {
  if(Serial.available() >= 6)//detekce instrukce
  {
    mode = Serial.read();//Nacteni modu
    //nacteni dat
    value.setCharAt(0,Serial.read());
    value.setCharAt(1,Serial.read());
    value.setCharAt(2,Serial.read());
    value.setCharAt(3,Serial.read());
    //nacteni koncoveho zaku
    Serial.read();

    //Vykonani prikazu
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
  MotorZrun();//Rutyna vytvarejici vibrace u motoru Z
}


void Ohrev(char mod,dtOhrev *data, int pin)//Metoda pro zpracovani instukce Ohrev
{
  if(notSend(mod,*data))
  {
  *data = value.toInt();
   analogWrite(pin, *data);
  }
}


void Svetlo(char mod,dtSvetlo *data, int pin)//Metoda pro zpracovani instukce Svetlo
{
  if(notSend(mod,*data))
  {
  *data = value.toInt();
   analogWrite(pin, *data);
  }
}


//Metoda pro zpracovani instukce MotorX.
/*
PWM rizeni nezvladal zdroj 12V a H-mustkem. 
Proto jsme pristoupil na binarni rizeni, ktere je ovsem velmi rychle a silne.
Kvuli moznosti poskozeni o stenu jsem urcil casovy limit pri posunech.
Funkce vyckava, dokud cas nevyprsi. 
Prikaz stopMotorX nema zadnou funkci
Omezuje to motor Z pri vybracich, co nicemu nevadi.
Omezeje zpracovavani novych instukci na max 200ms, co nevadi.
*/
void MotorXB()
{
  if(notSend(modMotorX,motorX))
  {
    dtMotorX novi = value.toInt();
    if(novi == levoSMotorX )
    {
      digitalWrite(pinMotorXdig, 0);
      digitalWrite(pinMotorXpwm, 1);
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
    //vypnuti motoru
    digitalWrite(pinMotorXdig, 0);
    digitalWrite(pinMotorXpwm, 0);
  }
}


void TepOkoliB()//Posle udaj o teplote okoli teplometu.
{
  notSend(modTepOkoli,TepControl(mlx.readAmbientTempC()));
}


void TepNadrzB()//Posle udaj o teplote nadrzi teplometu.
{
  notSend(modTepNadrz,TepControl(mlx.readObjectTempC()));
}


int TepControl(dtTep a)//kontrola chyb teplomeru a uprava formatu
{
  int u=(double)(a*tepNas);
  if(u<minTep*tepNas) {u = minTep*tepNas;}
  if(u>maxTep*tepNas) {u = maxTep*tepNas;}
  return u;
}


/*
Metoda MotorZC() ma dva rozdilne zamereni
-Posunuje o oredem difinovany pocet kroku motor Z. Po tuto dobu zaastaveno zpracovani novych instrukci.
--Nefunkguje prikaz stopMotorZ
-Nastavy MotorZrun() na generator vibraci (vibMotorZOn) nebo vibrace vipne (stopMotorZ).
*/
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


void MotorZrun()//Tento kod je spousten v loop(). Pokud jsou splneny podminy, vytvari vybrace
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


void MotorZindex(int index)//Nastavy u motoru Z piny.
{
    digitalWrite(pinMotorZ1, motorZMetrix[index][0]);
    digitalWrite(pinMotorZ2, motorZMetrix[index][1]);
    digitalWrite(pinMotorZ3, motorZMetrix[index][2]);
    digitalWrite(pinMotorZ4, motorZMetrix[index][3]);
}


//Return je:
//True:  Nenalezeno "????" a pokracuje se dal v ukonech jako zapsani dat.
//False: Nalezeno "????" a zpracovano. Zakaz delat dalsi ukony
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
