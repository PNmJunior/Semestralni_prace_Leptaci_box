/*
Program pro Arduino Nano.
Arduino je uvnitr Leptaciho boxu, kde zastava funkci microkontroleru.


*/
#include <Arduino.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "definice.h"//Spolecny soubor pro Aplikaci

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
//piny, ktere jsou nastaveny jinak nez pres pinMode(...)
#define pinTepSCL 19
#define pinTepSDA 18
#define pinSerTX 1
#define pinSerRX 0

// inicializace teploměru MLX z knihovny
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


//Globalni promenne
char mode;//instrukce
// default String
String value = String(PrazdnePole);//Pouzivan jako predavac informace o datech v instrukci.
// inicializace globalnich promennych
dtOhrev ohrevA;
dtOhrev ohrevB;
dtSvetlo svetloA;
dtSvetlo svetloB;
dtSvetlo svetloC;
dtMotorX motorX;
dtMotorZ motorZ;

//promenne motoruZ
int motorZscit;//urcuje smer pohybu pri vibracich
byte motorZkrok;//Krok motoru Z
const bool motorZMetrix[5][4]= {{1,0,1,0},{0,1,1,0},{0,1,0,1},{1,0,0,1},{0,0,0,0}};//vystupy pinu vuci krokum
long long motorZtime;//cas dalsiho kroku

void setup() 
{
  Serial.begin(9600);// inicializace UART
  Serial.flush();
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

  //Inicializace promennych
  //Ohrev: Aktivni vyuziti, rozsah 0-255
  ohrevA = 0;
  ohrevB = 0;
  //Svetlo: Aktivni vyuziti, rozsah 0-255
  svetloA = 0;
  svetloB =0;
  svetloC =0;
  //MotorX: default:0
  motorX = 0;
  //Svetlo: Aktivni vyuziti, prepinani mezi 0 (stopMotorZ) a 3 (vibMotorZOn)
  motorZ = 0;
  //Systemove promenne pro motor X
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
    //nacteni koncoveho znaku
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
      MotorZ()  ;
      break; 
      case modTepNadrz:
      TepNadrzB();
      break;
      case modTepOkoli:
      TepOkoliB();
      break;
      case modIdentifikace:
      Identifikace();
      break;
      default:
      Serial.print(Problem);
      break;
    }
  }
  MotorZrun();//Rutina vytvarejici vibrace u motoru Z
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
PWM rizeni nezvladal zdroj 12V nebo H-mustkem. 
Proto jsem pristoupil na binarni rizeni, ktere je ovsem velmi rychle a silne.
Kvuli moznosti poskozeni o stenu jsem urcil casovy limit pri posunech.
Funkce vyckava, dokud cas nevyprsi. 
Prikaz stopMotorX nema zadnou funkci
Omezuje to motor Z pri vibracich, co nicemu nevadi.
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


void TepOkoliB()//Posle udaj o teplote v Leptacim boxu.
{
  notSend(modTepOkoli,TepControl(mlx.readAmbientTempC()));
}


void TepNadrzB()//Posle udaj o teplote miskyA.
{
  notSend(modTepNadrz,TepControl(mlx.readObjectTempC()));
}

void Identifikace()//Odpoved na dotaz identifikace
{
  notSend(modIdentifikace,numIdentifikace);
}

int TepControl(dtTep a)//kontrola chyb teplomeru a uprava formatu
{
  int u=(double)(a*tepNas);
  if(u<minTep*tepNas) {u = minTep*tepNas;}
  if(u>maxTep*tepNas) {u = maxTep*tepNas;}
  return u;
}


/*
Metoda MotorZ() ma dva rozdilne zamereni
-Posunuje o predem difinovany pocet kroku motor Z. Po tuto dobu zastaveno zpracovani novych instrukci.
--Nefunkguje prikaz stopMotorZ
-Nastavy MotorZrun() na generator vibraci (vibMotorZOn) nebo vibrace vypne (stopMotorZ).
*/
void MotorZ()
{
  if(notSend(modMotorZ,motorZ))
  {
    dtMotorZ novi = value.toInt();
    if(novi == stopMotorZ )
    {
       motorZkrok = 4;
       motorZ = stopMotorZ;
       MotorZindex(4);
    digitalWrite(pinMotorXdig, 0);
    digitalWrite(pinMotorXpwm, 0);
    }
    else if(novi == nahorumotorZ)
    {
      for(int i = 0; i <= 32;  i++)
      {
        MotorZindex(i%4);
        delay(7);
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
       //motorZscit = 1;
    }
    MotorZindex(4);
  }
}


void MotorZrun()//Tento kod je spousten v loop(). Pokud jsou splneny podminky, vitvari vibrace
{
  if(motorZ == vibMotorZOn && motorZtime < millis())
  {
    
    /*
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
    */
     motorZtime = millis() + 60;
    switch (motorZkrok)
    {
    case 0:
          digitalWrite(pinMotorXdig, 1);
    digitalWrite(pinMotorXpwm, 0);  
      break;
      case 1:
      case 3:
          digitalWrite(pinMotorXdig, 0);
    digitalWrite(pinMotorXpwm, 0);
    break;
    case 2:
        digitalWrite(pinMotorXdig, 0);
    digitalWrite(pinMotorXpwm, 1);
    break;
    
    default:
      break;
    }
  }
}


void MotorZindex(int index)//Nastaveni pinu u motoru Z.
{
    digitalWrite(pinMotorZ1, motorZMetrix[index][0]);
    digitalWrite(pinMotorZ2, motorZMetrix[index][1]);
    digitalWrite(pinMotorZ3, motorZMetrix[index][2]);
    digitalWrite(pinMotorZ4, motorZMetrix[index][3]);
}


//Return je:
//True:  Nenalezeno "????" a pokracuje se dal v ukonech jako zapsani dat.
//False: Nalezeno "????" a zpracovano. Zakaz delat dalsi ukony.
bool notSend(char typ, int data)
{
  //Za predpokladu, ze se ve value nachazi bud ctyrmistne cislo nebo "????", neni potreba pro '?' hldedat v kazdem ze ctyr pozic.
  if(value.charAt(2) == '?')
  {
  char c[6];
  sprintf(c, "%c%4d\n", typ,data);
  Serial.print(c);
  return false;
  } 
  return true;
}
