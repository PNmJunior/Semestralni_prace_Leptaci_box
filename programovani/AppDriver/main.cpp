#include <QApplication>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>

#include "definice.h"
#include "svetloBox.h"
#include "topeniBox.h"


int main(int argc, char ** argv)
{
	QApplication a(argc, argv);
	Serial
	

	QWidget w;
	QVBoxLayout vbox;
	QHBoxLayout potrSerialSetBox;//nastaveni portu
	QHBoxLayout motorXBox;//kontrola motoru X
	QHBoxLayout motorZBox;//kontrola motoru Z
	QHBoxLayout svetloABox;//kontrola osvetleni mysly A
	QHBoxLayout svetloBBox;//kontrola osvetleni mysly B
	QHBoxLayout topeniABox;//kontrola ohrevu mysly A
	QHBoxLayout topeniBBox;//kontrola ohrevu mysly B

	QLabel potrSerialSetBoxText("Vyber portu:",&w);
	QPushButton potrSerialSetBoxButAkt("Aktualizovat seznam");
	QComboBox potrSerialSetBoxCombSeznamPortu(&w);
	//Zaridit nacteni
	QPushButton potrSerialSetBoxButOk("Vyber");
	potrSerialSetBox.addWidget(&potrSerialSetBoxText);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButAkt);
	potrSerialSetBox.addWidget(&potrSerialSetBoxCombSeznamPortu);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButOk);


	QLabel motorXBoxText("Kontrola motoru X:",&w);
	QPushButton motorXBoxLeftB("<<"),motorXBoxLeftS("<"), motorXBoxStop("Stop"),motorXBoxRightS(">"),motorXBoxRightS(">>");
	motorXBox.addWidget(&motorXBoxText);
	motorXBox.addWidget(&motorXBoxLeftB);
	motorXBox.addWidget(&motorXBoxLeftS);
	motorXBox.addWidget(&motorXBoxStop);
	motorXBox.addWidget(&motorXBoxRightS);
	motorXBox.addWidget(&motorXBoxRightB);

	QLabel motorZBoxText("Kontrola motoru Z:",&w);
	QPushButton motorZBoxUp("Nahoru"), motorZBoxDown("Dolu"), motorZBoxVibration("Vibrace");
	motorZBox.addWidget(&motorXBoxText);
	motorZBox.addWidget(&motorZBoxUp);
	motorZBox.addWidget(&motorZBoxDown);
	motorZBox.addWidget(&motorZBoxVibration);

	QLabel svetloABoxText("Regulace podsviceni mysky A:",&w);
	QSlider svetloABoxSlider( Qt::Horizontal,&w);
	svetloABoxSlider.setMaximum(100);
	svetloABoxSlider.setMinimum(0);
	svetloABoxSlider.setPageStep(10);
	svetloABoxSlider.setSingleStep(10);

	svetloBBox.addWidget(&svetloABoxText);
	svetloBBox.addWidget(&svetloABoxSlider);



	QLabel svetloBBoxText("Regulace podsviceni mysky B:",&w);
	QSlider svetloBBoxSlider( Qt::Horizontal,&w);
	svetloBBoxSlider.setMaximum(100);
	svetloBBoxSlider.setMinimum(0);
	svetloBBoxSlider.setPageStep(10);
	svetloBBoxSlider.setSingleStep(10);


	svetloBBox.addWidget(&svetloBBoxText);
	svetloBBox.addWidget(&svetloBBoxSlider);





	vbox.addLayout(&potrSerialSetBox);
	vbox.addLayout(&motorXBox);
	vbox.addLayout(&motorZBox);
	vbox.addLayout(&svetloABox);
	vbox.addLayout(&svetloBBox);
	vbox.addLayout(&topeniABox);
	vbox.addLayout(&topeniBBox);
	
	w.setLayout(&vbox);
	w.show();

	return a.exec();
}
