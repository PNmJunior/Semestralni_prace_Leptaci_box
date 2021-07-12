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
#include <QFont>
#include <QTimer>

#include "definice.h"
#include "topeniBox.h"
#include "protokolKomunikace.h"


int main(int argc, char ** argv)
{
	QApplication a(argc, argv);
	QTimer tim1 ;
	
	QList<QSerialPortInfo> seznPort = QSerialPortInfo::availablePorts();
	QSerialPortInfo vyberPort;
	QSerialPort serPort;
	QWidget w;
	QVBoxLayout vbox;
	QHBoxLayout potrSerialSetBox;//nastaveni portu
	QHBoxLayout motorXBox;//kontrola motoru X
	QHBoxLayout motorZBox;//kontrola motoru Z
	QHBoxLayout miskaABox;//kontrola motoru Z
	QHBoxLayout miskaBBox;//kontrola motoru Z
	QHBoxLayout svetloABox;//kontrola osvetleni mysly A
	QHBoxLayout svetloBBox;//kontrola osvetleni mysly B
	QHBoxLayout teplotaABox;//kontrola ohrevu mysly A
	QHBoxLayout topeniABox;//kontrola ohrevu mysly A
	QHBoxLayout topeniBBox;//kontrola ohrevu mysly B
	QHBoxLayout teplotaVBox;

	QFont fOhrev = QFont("Arial", 20, QFont::Bold);

	QLabel potrSerialSetBoxText("Vyber portu:",&w),potrSerialSetBoxTextStatus("Stav: ",&w),potrSerialSetBoxStatus("Nebyl vybran zadny port.",&w);
	QPushButton potrSerialSetBoxButAkt("Aktualizovat seznam");
	QComboBox potrSerialSetBoxCombSeznamPortu(&w);
	//potrSerialSetBoxCombSeznamPortu.addItems(seznPort);
	auto infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos){
        potrSerialSetBoxCombSeznamPortu.addItem(info.portName());}

	dtMotorX mX;
	dtMotorZ mZ;
	dtOhrev oA, oB;
	dtSvetlo sA, sB, sC;
	dtTep tA, tB;
	QPushButton potrSerialSetBoxButOk("Vyber");
	QPushButton potrSerialSetBoxButAnswerAll("Aktualizovat vse");
	potrSerialSetBox.addWidget(&potrSerialSetBoxText);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButAkt);
	potrSerialSetBox.addWidget(&potrSerialSetBoxCombSeznamPortu);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButOk);
	potrSerialSetBox.addWidget(&potrSerialSetBoxTextStatus);
	potrSerialSetBox.addWidget(&potrSerialSetBoxStatus);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButAnswerAll);


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
	motorZBox.addWidget(&motorZBoxText);
	motorZBox.addWidget(&motorZBoxUp);
	motorZBox.addWidget(&motorZBoxDown);
	motorZBox.addWidget(&motorZBoxVibration);
	motorZBox.addWidget(&motorZBoxStop);


	QLabel miskaABoxText("Mysky A:",&w);
	QPushButton miskaABoxAnswer("Aktualizace");
	miskaABox.addWidget(&miskaABoxText);
	miskaABox.addWidget(&miskaABoxAnswer);

	QLabel svetloABoxText("Regulace podsviceni",&w);
	QSlider svetloABoxSlider( Qt::Horizontal,&w);
	svetloABoxSlider.setMaximum(100);
	svetloABoxSlider.setMinimum(0);
	svetloABoxSlider.setPageStep(1);
	svetloABoxSlider.setSingleStep(1);

	svetloABox.addWidget(&svetloABoxText);
	svetloABox.addWidget(&svetloABoxSlider);

	QLabel teplotaABoxName("Teplota: ",&w), teplotaABoxHodnota("00,00",&w), teplotaABoxJednotka("°C",&w); 
	teplotaABox.addWidget(&teplotaABoxName);
	teplotaABox.addWidget(&teplotaABoxHodnota);
	teplotaABox.addWidget(&teplotaABoxJednotka);

	QLabel topeniABoxText("Topeni: "), topeniABoxStav("Chladne");
	topeniABoxStav.setFont(fOhrev);
	QSlider topeniABoxSlider(Qt::Horizontal,&w);
	topeniABoxSlider.setMaximum(255);
	topeniABoxSlider.setMinimum(0);
	topeniABoxSlider.setPageStep(1);
	topeniABoxSlider.setSingleStep(1);

	topeniABox.addWidget(&topeniABoxText);
	topeniABox.addWidget(&topeniABoxSlider);
	topeniABox.addWidget(&topeniABoxStav);

	QLabel miskaBBoxText("Mysky B:",&w);
	QPushButton miskaBBoxAnswer("Aktualizace");
	miskaBBox.addWidget(&miskaBBoxText);
	miskaBBox.addWidget(&miskaBBoxAnswer);

	QLabel svetloBBoxText("Regulace podsviceni",&w);
	QSlider svetloBBoxSlider( Qt::Horizontal,&w);
	
	svetloBBoxSlider.setMaximum(100);
	svetloBBoxSlider.setMinimum(0);
	svetloBBoxSlider.setPageStep(1);
	svetloBBoxSlider.setSingleStep(1);


	svetloBBox.addWidget(&svetloBBoxText);
	svetloBBox.addWidget(&svetloBBoxSlider);

	QLabel topeniBBoxText("Topeni: "), topeniBBoxStav("Chladne");
	topeniBBoxStav.setFont(fOhrev);
	QSlider topeniBBoxSlider(Qt::Horizontal,&w);
	topeniBBoxSlider.setMaximum(255);
	topeniBBoxSlider.setMinimum(0);
	topeniBBoxSlider.setPageStep(1);
	topeniBBoxSlider.setSingleStep(1);

	topeniBBox.addWidget(&topeniBBoxText);
	topeniBBox.addWidget(&topeniBBoxSlider);
	topeniBBox.addWidget(&topeniBBoxStav);

	QLabel teplotaVBoxName("Vnejsi teplota: ",&w), teplotaVBoxHodnota("00,00",&w), teplotaVBoxJednotka("°C",&w); 
	teplotaVBox.addWidget(&teplotaVBoxName);
	teplotaVBox.addWidget(&teplotaVBoxHodnota);
	teplotaVBox.addWidget(&teplotaVBoxJednotka);

	vbox.addLayout(&potrSerialSetBox);
	vbox.addLayout(&motorXBox);
	vbox.addLayout(&motorZBox);
	vbox.addLayout(&miskaABox);
	vbox.addLayout(&svetloABox);
	vbox.addLayout(&teplotaABox);
	vbox.addLayout(&topeniABox);
	vbox.addLayout(&miskaBBox);
	vbox.addLayout(&svetloBBox);
	vbox.addLayout(&topeniBBox);
	vbox.addLayout(&teplotaVBox);
	
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
			tim1.start(2000);
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
	QObject::connect(&svetloABoxSlider, QSlider::valueChanged, [&](){
		protKom.sendSvetloProc(modSvetloA,svetloABoxSlider.value());
		
	});
	QObject::connect(&svetloBBoxSlider, QSlider::valueChanged, [&](){
		protKom.sendSvetloProc(modSvetloB, svetloBBoxSlider.value());
		
	});
	QObject::connect(&topeniABoxSlider, QSlider::valueChanged, [&](){
		protKom.sendOhrev(modOhrevA, topeniABoxSlider.value());
		//miskaABoxAnswer.click();
	});
	QObject::connect(&topeniBBoxSlider, QSlider::valueChanged, [&](){
		protKom.sendOhrev(modOhrevB, topeniBBoxSlider.value());
		//miskaBBoxAnswer.click();
	});
	QObject::connect(&miskaABoxAnswer, QPushButton::clicked, [&](){
		teplotaABoxHodnota.setText(QString::number(protKom.AnswerDouble(modTepNadrz)));
		int topeni = protKom.AnswerInt(modOhrevA);
		if (topeni == 0 || topeni == -1)
		{
			topeniABoxStav.setText("Chlazeni");
		}
		else
		{
			topeniABoxStav.setText("!!!Topime!!!");
		}
		//topeniABoxSlider.setValue(topeni);
		
	});
		QObject::connect(&miskaBBoxAnswer, QPushButton::clicked, [&](){
		//teplomer neobsahuje
		int topeni = protKom.AnswerInt(modOhrevB);
		if (topeni == 0 || topeni == -1)
		{
			topeniBBoxStav.setText("Chlazeni");
		}
		else
		{
			topeniBBoxStav.setText("!!!Topime!!!");
		}
		//topeniBBoxSlider.setValue(topeni);
	});
	QObject::connect(&potrSerialSetBoxButAnswerAll, QPushButton::clicked, [&](){
		svetloABoxSlider.setValue(protKom.AnswerSvetlo(modSvetloA));
		svetloBBoxSlider.setValue(protKom.AnswerSvetlo(modSvetloB));
		teplotaVBoxHodnota.setText(QString::number(protKom.AnswerDouble(modTepOkoli)));
		miskaABoxAnswer.click();
		miskaBBoxAnswer.click();
	});
	QObject::connect(&tim1, QTimer::timeout, [&](){
		teplotaVBoxHodnota.setText(QString::number(protKom.AnswerDouble(modTepOkoli)));
		miskaABoxAnswer.click();
		miskaBBoxAnswer.click();
	});


	w.show();


	return a.exec();
}
