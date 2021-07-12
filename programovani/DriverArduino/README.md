# DriverArduino

# Popis zapojeni soustavy
Komponenty:
-Arduino Nano - Ridi celou elektrniku
-USB kabel, ktery spojuje Arduino s PC
-Zdroj napeti 5V
-Zdroj napeti 12V 2A
-H-mustek pro 12V, ktery ovlada MotorX
-H-mustek pro 5V, ktery ovlada MotorZ
-MotorX dc 12V motor, s proudovou spickou kolem 1,5A
-MotorZ je "CD-ROM Stepper motor"
-Senzor teploty MLX90614. Jde o infracervenu snimac. Meri jak teplotu okoli, tak i objektu ne ktery je nemiren.
-Deska vlastni vyroby. Spina kontakty na podlosce pro podsviceni a topeni.
-Podloska. Vlastni vyroby.

GND spojeno s:
-GND Arduino a nasledne:
--USB kabel a zdroj pocitace...
--GND Senzor teplomeru
-Zabudovany zdroj 12V
-Zabudovany zdroj 5V
-GND H-mustek pro 12V MotorX
-GND H-mustek pro 5V MotorZ
-GND deska vlastni vyroby.

Napajeni:
12V:
-Vcc H-mustek pro 12V MotorX
-(+) Deska vlastni vyroby
5V:
-Vcc H-mustek pro 5V MotorZ
USB 5V:
-Arduino 5V
-Vcc teplomer




