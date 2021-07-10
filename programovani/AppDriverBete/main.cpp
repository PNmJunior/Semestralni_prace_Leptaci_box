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
#include <QString>
#include <QByteArray>
#include <QObject>

#include "definice.h"
#include "topeniBox.h"
#include "protokolKomunikace.h"


int main(int argc, char ** argv)
{
	QApplication a(argc, argv);

	QList<QSerialPortInfo> seznPort = QSerialPortInfo::availablePorts();
	QSerialPortInfo vyberPort;
	QSerialPort serPort;
	QWidget w;
	QVBoxLayout vbox;
	QHBoxLayout potrSerialSetBox;//nastaveni portu
	QHBoxLayout motorXBox;//kontrola motoru X
	QHBoxLayout motorZBox;//kontrola motoru Z
	QHBoxLayout svetloABox;//kontrola osvetleni mysly A
	QHBoxLayout svetloBBox;//kontrola osvetleni mysly B
	QHBoxLayout topeniABox;//kontrola ohrevu mysly A
	QHBoxLayout topeniBBox;//kontrola ohrevu mysly B

	QLabel potrSerialSetBoxText("Vyber portu:",&w),potrSerialSetBoxTextStatus("Stav: ",&w),potrSerialSetBoxStatus("Nebyl vybran zadny port.",&w);
	QPushButton potrSerialSetBoxButAkt("Aktualizovat seznam");
	QComboBox potrSerialSetBoxCombSeznamPortu(&w);
	//potrSerialSetBoxCombSeznamPortu.addItems(seznPort);
	auto infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos){
        potrSerialSetBoxCombSeznamPortu.addItem(info.portName());}
	QPushButton potrSerialSetBoxButOk("Vyber");
	potrSerialSetBox.addWidget(&potrSerialSetBoxText);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButAkt);
	potrSerialSetBox.addWidget(&potrSerialSetBoxCombSeznamPortu);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButOk);
	potrSerialSetBox.addWidget(&potrSerialSetBoxTextStatus);
	potrSerialSetBox.addWidget(&potrSerialSetBoxStatus);


	QLabel motorXBoxText("Kontrola motoru X:",&w);
	QPushButton motorXBoxLeftB("<<"),motorXBoxLeftS("<"), motorXBoxStop("Stop1234"),motorXBoxRightS(">"),motorXBoxRightB(">>");
	motorXBox.addWidget(&motorXBoxText);
	motorXBox.addWidget(&motorXBoxLeftB);
	motorXBox.addWidget(&motorXBoxLeftS);
	motorXBox.addWidget(&motorXBoxStop);
	motorXBox.addWidget(&motorXBoxRightS);
	motorXBox.addWidget(&motorXBoxRightB);

	QLabel motorZBoxText("Kontrola motoru Z:",&w);
	QPushButton motorZBoxUp("Nahoru"), motorZBoxDown("Dolu"), motorZBoxVibration("Vibrace"),motorZBoxStop("Stop");
	motorZBox.addWidget(&motorXBoxText);
	motorZBox.addWidget(&motorZBoxUp);
	motorZBox.addWidget(&motorZBoxDown);
	motorZBox.addWidget(&motorZBoxVibration);
	motorZBox.addWidget(&motorZBoxStop);

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

	protokolKomunikace protKom = protokolKomunikace(&serPort,&potrSerialSetBoxStatus);

	QObject::connect(&potrSerialSetBoxButAkt, QPushButton::clicked, [&](){
		potrSerialSetBoxCombSeznamPortu.clear();
		infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos)
        potrSerialSetBoxCombSeznamPortu.addItem(info.portName());
	});


	QObject::connect(&potrSerialSetBoxButOk, QPushButton::clicked, [&](){
		if(potrSerialSetBoxCombSeznamPortu.currentIndex() == -1)
		{
			potrSerialSetBoxStatus.setText("Zkontroluj pripojeni. Vyber stroj ze seznamu. Zmackni tlacitko.");
		}
		vyberPort = infos.at( potrSerialSetBoxCombSeznamPortu.currentIndex());
		serPort.setPort(vyberPort);
		serPort.open(QIODevice::ReadWrite);
		serPort.setBaudRate(QSerialPort::Baud9600);

		serPort.setDataBits(QSerialPort::Data8);
		serPort.setParity(QSerialPort::NoParity);
		serPort.setStopBits(QSerialPort::OneStop);
		serPort.setFlowControl(QSerialPort::NoFlowControl);
		while (!serPort.isOpen())
		{
			serPort.open(QIODevice::ReadWrite);
		}
		if(!(serPort.isWritable()&&serPort.isOpen()))
		{
			protKom.notOpenWrite();
		}
		else
		{
			QString beta = QString("Komunikace s portem: ")+serPort.portName();
			potrSerialSetBoxStatus.setText(beta);
		}
	});

	QObject::connect(&motorXBoxLeftS, QPushButton::clicked, [&](){
		protKom.sendMotX(levoSMotorX);
	});
	QObject::connect(&motorXBoxLeftB, QPushButton::clicked, [&](){
		protKom.sendMotX(levoBMotorX);
	});
	QObject::connect(&motorXBoxRightS, QPushButton::clicked, [&](){
		protKom.sendMotX(pravSMotorX);
	});
	QObject::connect(&motorXBoxRightB, QPushButton::clicked, [&](){
		protKom.sendMotX(pravBMotorX);
	});
	QObject::connect(&motorXBoxStop, QPushButton::clicked, [&](){
		protKom.sendMotX(stopMotorX);
	});
	QObject::connect(&motorZBoxStop, QPushButton::clicked, [&](){
		protKom.sendMotZ(stopMotorZ);
	});
	QObject::connect(&motorZBoxUp, QPushButton::clicked, [&](){
		protKom.sendMotZ(nahorumotorZ);
	});
	QObject::connect(&motorZBoxDown, QPushButton::clicked, [&](){
		protKom.sendMotZ(dolumotorZ);
	});
	QObject::connect(&motorZBoxVibration, QPushButton::clicked, [&](){
		protKom.sendMotZ(vibMotorZOn);
	});



	w.show();

	return a.exec();
}
