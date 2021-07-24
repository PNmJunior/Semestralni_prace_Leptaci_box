# AppDriverBete

Aplikace je naprogramovaná v QT5 kvůli podpoře QSerialPort.h
##Projekt obsahuje:
* "AppDriverBete.pro" - Vytváření aplikace v release.
* "main.cpp" - Naprogramovaná aplikace.
* "definice.h" - Soubor s definicemi stejný jak pro Arduino, tak pro Aplikaci. Nutné kvůli kompaktibilitě komunikačního protokolu.
* "protokolKomunikace.h" - Obstarává komunikaci s Arduinem pro Aplikaci.
* "qcustomplot.h" a "qcustomplot.cpp" - Vytvoření grafu. Zdroj Studijní materiály.