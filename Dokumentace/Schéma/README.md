# Schéma
V pdf je schematicky znázornene zapojeni.
* Schema zapojeni - Celkove propojeni
* Deska - Jde o plošný spoj vlastní výroby, kde je zminěná schéma.
* Podloska - Jde o konstrukci vlastní výroby, kde je umistěne podsvíceni misek a konektory pro topne tělesa... 
## Popis zapojeni soustavy
###Komponenty:
* Arduino Nano - Ridi celou elektrniku
* USB kabel, ktery spojuje Arduino s PC
* Zdroj napeti 5V
* Zdroj napeti 12V 2A
* H-mustek pro 12V, ktery ovlada MotorX
* H-mustek pro 5V, ktery ovlada MotorZ
* MotorX dc 12V motor, s proudovou spickou kolem 1,5A
* MotorZ je "CD-ROM Stepper motor"
* Senzor teploty MLX90614. Jde o infracervenu snimac. Meri jak teplotu okoli, tak i objektu ne ktery je nemiren.
* Deska vlastni vyroby. Spina kontakty na podlosce pro podsviceni a topeni.
* Podloska. Vlastni vyroby.

###GND spojeno s:
* GND Arduino a nasledne:
    * USB kabel a zdroj pocitace...
    * GND Senzor teplomeru
* Zabudovany zdroj 12V
* Zabudovany zdroj 5V
* GND H-mustek pro 12V MotorX
* GND H-mustek pro 5V MotorZ
* GND deska vlastni vyroby.

###Napajeni:
####12V:
* Vcc H-mustek pro 12V MotorX
* (+) Deska vlastni vyroby
####5V:
* Vcc H-mustek pro 5V MotorZ
####USB 5V:
* Arduino 5V
* Vcc teplomer

## Zapojeni Arduino Nano
Vytobyl jsem pomoci baru prazdných pinu konektory. Pojmenoval jsme je "Sloty". 
* Slot1 - 4 pin. od GND do D4
    * GND - zem zdroju
    * D3 pwm - Pwm kontroler motoru X
    * D4 - Kontroler motoru X
* Slot2 - 8 pinu od D5 do D12
    * D5 - Ohřev A
    * D6 - Ohřev B
    * D9 - Světlo A
    * D10 - Světlo B
    * D11 - Světlo C
* Slot3 - 4 pinu do A0 do A3
    * A3 - Kontrolér 1 pro motor Z
    * A2 - Kontrolér 2 pro motor Z
    * A1 - Kontrolér 3 pro motor Z
    * A0 - Kontrolér 4 pro motor Z
* Slot4 - 8 pinu od A4 od VIN
    * GND - zep pro senzor MLX.
    * 5V - napajeni pro senzor MLX.
    * A5 - SCL pro senzor MLX.
    * A4 - SDA pro senzor MLX.


