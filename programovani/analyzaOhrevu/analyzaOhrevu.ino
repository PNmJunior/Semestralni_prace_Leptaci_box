#include <avr/wdt.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
// inicializace teploměru MLX z knihovny
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
unsigned long t;
int j;
bool k;
void setup() {
  // komunikace po sériové lince rychlostí 9600 baud
  Serial.begin(9600);
  // zahájení komunikace s teploměrem MLX
  mlx.begin(); 
  pinMode(3, OUTPUT); 
  k = 0;
  digitalWrite(3,k);
  j=0;
  t = millis();
  wdt_enable(WDTO_2S);
}

void loop() {
  /*
  while(t > millis())
  {
    if(Serial.available())
    {
      Serial.read();
      if(1== k)
      {
        k = 0;
          Serial.read();
      }
      else
      {
        k = 1;
        Serial.read();
      }
      
    }
    delay(1);
  }
  digitalWrite(3,k);
  Serial.print(j++);
  Serial.print(";");
  Serial.print(k);
  Serial.print(";");
  */
  Serial.print(mlx.readAmbientTempC(),4);
  Serial.print(" -> ");
  Serial.print(mlx.readObjectTempC(),4);
  Serial.println();
  delay(1000);
}
