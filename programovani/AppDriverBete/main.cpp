//include knihoven
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

//include knihoven
#include "definice.h"
#include "protokolKomunikace.h"


//main
int main(int argc, char ** argv)
{
	QApplication a(argc, argv);
	QTimer tim1 ;//pravidena aktualizace dat
	QList<QSerialPortInfo> seznPort = QSerialPortInfo::availablePorts();//seznam vsech portu v PC
	QSerialPortInfo vyberPort;//Port na kterem je lepraci box
	QSerialPort serPort;//inicializace vybraneho portu
	QWidget w;
	QVBoxLayout vbox;
	//Oblasti grafiky
	QHBoxLayout potrSerialSetBox;//nastaveni portu
	QHBoxLayout motorXBox;//kontrola motoru X
	QHBoxLayout motorZBox;//kontrola motoru Z
	QHBoxLayout miskaABox;//Oblast misky A
	QHBoxLayout miskaBBox;//Oblast misky B
	QHBoxLayout svetloABox;//kontrola osvetleni mysly A
	QHBoxLayout svetloBBox;//kontrola osvetleni mysly B
	QHBoxLayout teplotaABox;//kontrola ohrevu mysly A
	QHBoxLayout ohrevABox;//kontrola teploty mysly A
	QHBoxLayout ohrevBBox;//kontrola teploty mysly B
	QHBoxLayout teplotaVBox;//teoplota nejsiho okoli
	//fond pro indikaci Ohrivani
	QFont fOhrev = QFont("Arial", 20, QFont::Bold);

	//Nastaveni portu
	QLabel potrSerialSetBoxText("Vyber portu:",&w),potrSerialSetBoxTextStatus("Stav: ",&w),potrSerialSetBoxStatus("Nebyl vybran zadny port.",&w);
	QPushButton potrSerialSetBoxButAkt("Aktualizovat seznam");
	QComboBox potrSerialSetBoxCombSeznamPortu(&w);
	auto infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos)
	{
        potrSerialSetBoxCombSeznamPortu.addItem(info.portName());//vypis portu do Combo
	}

	QPushButton potrSerialSetBoxButOk("Vyber");//Vyber portu
	QPushButton potrSerialSetBoxButAnswerAll("Aktualizovat vse");//Aktualizuje vsechny data
	potrSerialSetBox.addWidget(&potrSerialSetBoxText);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButAkt);
	potrSerialSetBox.addWidget(&potrSerialSetBoxCombSeznamPortu);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButOk);
	potrSerialSetBox.addWidget(&potrSerialSetBoxTextStatus);
	potrSerialSetBox.addWidget(&potrSerialSetBoxStatus);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButAnswerAll);


	//Rizeni Motoru X
	QLabel motorXBoxText("Rizeni motoru X:",&w);
	QPushButton motorXBoxLeftB("<<"),motorXBoxLeftS("<"), motorXBoxStop("Stop"),motorXBoxRightS(">"),motorXBoxRightB(">>");
	motorXBox.addWidget(&motorXBoxText);
	motorXBox.addWidget(&motorXBoxLeftB);
	motorXBox.addWidget(&motorXBoxLeftS);
	motorXBox.addWidget(&motorXBoxStop);
	motorXBox.addWidget(&motorXBoxRightS);
	motorXBox.addWidget(&motorXBoxRightB);

	
	//Rizeni Motoru Z
	QLabel motorZBoxText("Rizeni motoru Z:",&w);
	QPushButton motorZBoxUp("Nahoru"), motorZBoxDown("Dolu"), motorZBoxVibration("Vibrace"),motorZBoxStop("Stop");
	motorZBox.addWidget(&motorZBoxText);
	motorZBox.addWidget(&motorZBoxUp);
	motorZBox.addWidget(&motorZBoxDown);
	motorZBox.addWidget(&motorZBoxVibration);
	motorZBox.addWidget(&motorZBoxStop);


	//Myska A
	QLabel miskaABoxText("Mysky A:",&w);
	QPushButton miskaABoxAnswer("Aktualizace");
	miskaABox.addWidget(&miskaABoxText);
	miskaABox.addWidget(&miskaABoxAnswer);


	//Regulace podsviceni
	QLabel svetloABoxText("Regulace podsviceni",&w);
	QSlider svetloABoxSlider( Qt::Horizontal,&w);
	svetloABoxSlider.setMaximum(100);
	svetloABoxSlider.setMinimum(0);
	svetloABoxSlider.setPageStep(1);
	svetloABoxSlider.setSingleStep(1);

	svetloABox.addWidget(&svetloABoxText);
	svetloABox.addWidget(&svetloABoxSlider);


	//Indikace teploty nadrze
	QLabel teplotaABoxName("Teplota: ",&w), teplotaABoxHodnota("00,00",&w), teplotaABoxJednotka("°C",&w); 
	teplotaABox.addWidget(&teplotaABoxName);
	teplotaABox.addWidget(&teplotaABoxHodnota);
	teplotaABox.addWidget(&teplotaABoxJednotka);


	//Regulace ohrevu mysky
	QLabel ohrevABoxText("ohrev: "), ohrevABoxStav("??????");
	ohrevABoxStav.setFont(fOhrev);
	ohrevABoxStav.setMinimumWidth(200);
	QSlider ohrevABoxSlider(Qt::Horizontal,&w);
	ohrevABoxSlider.setMaximum(255);
	ohrevABoxSlider.setMinimum(0);
	ohrevABoxSlider.setPageStep(1);
	ohrevABoxSlider.setSingleStep(1);

	ohrevABox.addWidget(&ohrevABoxText);
	ohrevABox.addWidget(&ohrevABoxSlider);
	ohrevABox.addWidget(&ohrevABoxStav);


	//Myska B
	QLabel miskaBBoxText("Mysky B:",&w);
	QPushButton miskaBBoxAnswer("Aktualizace");
	miskaBBox.addWidget(&miskaBBoxText);
	miskaBBox.addWidget(&miskaBBoxAnswer);


	//Regulace podsviceni
	QLabel svetloBBoxText("Regulace podsviceni",&w);
	QSlider svetloBBoxSlider( Qt::Horizontal,&w);	
	svetloBBoxSlider.setMaximum(100);
	svetloBBoxSlider.setMinimum(0);
	svetloBBoxSlider.setPageStep(1);
	svetloBBoxSlider.setSingleStep(1);

	svetloBBox.addWidget(&svetloBBoxText);
	svetloBBox.addWidget(&svetloBBoxSlider);


	//Indikace teploty nadrze B neni


	//Regulace ohrevu mysky
	QLabel ohrevBBoxText("ohrev: "), ohrevBBoxStav("??????");
	ohrevBBoxStav.setFont(fOhrev);
	ohrevBBoxStav.setMinimumWidth(200);
	QSlider ohrevBBoxSlider(Qt::Horizontal,&w);
	ohrevBBoxSlider.setMaximum(255);
	ohrevBBoxSlider.setMinimum(0);
	ohrevBBoxSlider.setPageStep(1);
	ohrevBBoxSlider.setSingleStep(1);

	ohrevBBox.addWidget(&ohrevBBoxText);
	ohrevBBox.addWidget(&ohrevBBoxSlider);
	ohrevBBox.addWidget(&ohrevBBoxStav);



	//Indikator Vnejsi teploty
	QLabel teplotaVBoxName("Vnejsi teplota: ",&w), teplotaVBoxHodnota("00,00",&w), teplotaVBoxJednotka("°C",&w); 
	teplotaVBox.addWidget(&teplotaVBoxName);
	teplotaVBox.addWidget(&teplotaVBoxHodnota);
	teplotaVBox.addWidget(&teplotaVBoxJednotka);


	//Pridani vsech vrstev do QVBoxLayout
	vbox.addLayout(&potrSerialSetBox);
	vbox.addLayout(&motorXBox);
	vbox.addLayout(&motorZBox);
	vbox.addLayout(&miskaABox);
	vbox.addLayout(&svetloABox);
	vbox.addLayout(&teplotaABox);
	vbox.addLayout(&ohrevABox);
	vbox.addLayout(&miskaBBox);
	vbox.addLayout(&svetloBBox);
	vbox.addLayout(&ohrevBBox);
	vbox.addLayout(&teplotaVBox);
	
	//pridani do QWidget
	w.setLayout(&vbox);

	//inicializace moji knihovny pro komunikaci
	protokolKomunikace protKom = protokolKomunikace(&serPort,&potrSerialSetBoxStatus);


	//Hlidani signalu:

	//Aktualizace ComboBoxu s porty
	QObject::connect(&potrSerialSetBoxButAkt, QPushButton::clicked, [&](){
		potrSerialSetBoxCombSeznamPortu.clear();
		infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos)
        potrSerialSetBoxCombSeznamPortu.addItem(info.portName());
	});
	//Vyber portu urceneho ComboBoxem a jeho inicializace...
	QObject::connect(&potrSerialSetBoxButOk, QPushButton::clicked, [&](){
		if(potrSerialSetBoxCombSeznamPortu.currentIndex() == -1)//Kontrola vyberu Potru
		{
			potrSerialSetBoxStatus.setText("Zkontroluj pripojeni. Vyber stroj ze seznamu. Zmackni tlacitko.");
		}
		vyberPort = infos.at( potrSerialSetBoxCombSeznamPortu.currentIndex());//Vyber Potru
		serPort.setPort(vyberPort);//Nastavit port posle portInfo
		serPort.open(QIODevice::ReadWrite);//druch komunikace
		serPort.setBaudRate(QSerialPort::Baud9600);//rychlost
		//Dalsi parametry potrebne pro spravne fungovani serioveho portu
		serPort.setDataBits(QSerialPort::Data8);
		serPort.setParity(QSerialPort::NoParity);
		serPort.setStopBits(QSerialPort::OneStop);
		serPort.setFlowControl(QSerialPort::NoFlowControl);
		while (!serPort.isOpen())//Otevreni portu
		{
			serPort.open(QIODevice::ReadWrite);
		}
		if(!(serPort.isWritable()&&serPort.isOpen()))//Kontrola otevreni portu
		{
			protKom.notOpenWrite();
		}
		else//Uspesne nastavena komunikace
		{
			QString beta = QString("Komunikace s portem: ")+serPort.portName();//Informace o navazani spojeni
			potrSerialSetBoxStatus.setText(beta);
			
			//Nastaveni casovace, ktery pravidelne bude aktualizovat data.
			tim1.start(2000);
		}
	});
	//Odeslani instrukce pro MotorX - Trochu do leva
	QObject::connect(&motorXBoxLeftS, QPushButton::clicked, [&](){
		protKom.sendMotX(levoSMotorX);
	});
	//Odeslani instrukce pro MotorX - Hodne do leva
	QObject::connect(&motorXBoxLeftB, QPushButton::clicked, [&](){
		protKom.sendMotX(levoBMotorX);
	});
	//Odeslani instrukce pro MotorX - Trochu do prava
	QObject::connect(&motorXBoxRightS, QPushButton::clicked, [&](){
		protKom.sendMotX(pravSMotorX);
	});
	//Odeslani instrukce pro MotorX - Hodne do prava
	QObject::connect(&motorXBoxRightB, QPushButton::clicked, [&](){
		protKom.sendMotX(pravBMotorX);
	});
	//Odeslani instrukce pro MotorX - Stop - Ale nema jakikoliv viznam...
	QObject::connect(&motorXBoxStop, QPushButton::clicked, [&](){
		protKom.sendMotX(stopMotorX);
	});
	//Odeslani instrukce pro MotorZ - Stop - Zastavi vibrace
	QObject::connect(&motorZBoxStop, QPushButton::clicked, [&](){
		protKom.sendMotZ(stopMotorZ);
	});
	//Odeslani instrukce pro MotorZ - Nahoru
	QObject::connect(&motorZBoxUp, QPushButton::clicked, [&](){
		protKom.sendMotZ(nahorumotorZ);
	});
	//Odeslani instrukce pro MotorZ - Dolu
	QObject::connect(&motorZBoxDown, QPushButton::clicked, [&](){
		protKom.sendMotZ(dolumotorZ);
	});
	//Odeslani instrukce pro MotorZ - Zapnout vibrace
	QObject::connect(&motorZBoxVibration, QPushButton::clicked, [&](){
		protKom.sendMotZ(vibMotorZOn);
	});
	//Odeslani instrukce pro SvetloA - nastaveni hodnoty
	//Kvuli nelinearite sviceni prevadim udaj z procen na 0-255 pomoci vzorce...
	QObject::connect(&svetloABoxSlider, QSlider::valueChanged, [&](){
		protKom.sendSvetloProc(modSvetloA,svetloABoxSlider.value());
		
	});
	//Odeslani instrukce pro SvetloA - nastaveni hodnoty
	//Kvuli nelinearite sviceni prevadim udaj z procen na 0-255 pomoci vzorce...
	QObject::connect(&svetloBBoxSlider, QSlider::valueChanged, [&](){
		protKom.sendSvetloProc(modSvetloB, svetloBBoxSlider.value());	
	});
	//Odeslani instrukce pro Ohrev Misky A - nastaveni intenzity
	QObject::connect(&ohrevABoxSlider, QSlider::valueChanged, [&](){
		protKom.sendOhrev(modOhrevA, ohrevABoxSlider.value());
	});
	//Odeslani instrukce pro Ohrev Misky B - nastaveni intenzity
	QObject::connect(&ohrevBBoxSlider, QSlider::valueChanged, [&](){
		protKom.sendOhrev(modOhrevB, ohrevBBoxSlider.value());
	});
	//Odeslani dotazu na: teplotu nadrze A, stav Ohrevu A a od toho nastaveni upozorneni. 
	QObject::connect(&miskaABoxAnswer, QPushButton::clicked, [&](){
		teplotaABoxHodnota.setText(QString::number(protKom.answerDouble(modTepNadrz)));//Zobrazeni teploty
		int ohrev = protKom.answerInt(modOhrevA);//informaci o Ohrevu
		if (ohrev == 0 || ohrev == -1)
		{
			ohrevABoxStav.setText("Chlazeni");
		}
		else
		{
			ohrevABoxStav.setText("!!!Topime!!!");
		}
		//ohrevABoxSlider.setValue(ohrev);
		
	});
	//Odeslani dotazu na: stav Ohrevu B a od toho nastaveni upozorneni.
	QObject::connect(&miskaBBoxAnswer, QPushButton::clicked, [&](){
		//teplomer neobsahuje
		int ohrev = protKom.answerInt(modOhrevB);
		if (ohrev == 0 || ohrev == -1)
		{
			ohrevBBoxStav.setText("Chlazeni");
		}
		else
		{
			ohrevBBoxStav.setText("!!!Topime!!!");
		}
		//ohrevBBoxSlider.setValue(ohrev);
	});
	//Odeslani Dotayu: svetloA, svetloB, teplota okoli, ohrevA, teplota Nadrze A, ohrev B
	QObject::connect(&potrSerialSetBoxButAnswerAll, QPushButton::clicked, [&](){
		svetloABoxSlider.setValue(protKom.answerSvetlo(modSvetloA));
		svetloBBoxSlider.setValue(protKom.answerSvetlo(modSvetloB));
		teplotaVBoxHodnota.setText(QString::number(protKom.answerDouble(modTepOkoli)));
		miskaABoxAnswer.click();
		miskaBBoxAnswer.click();
	});
	//Pravidelna aktualizace udaji: teplota okoli, teplota nadrze A, ohrevA, ohrevB
	QObject::connect(&tim1, QTimer::timeout, [&](){
		teplotaVBoxHodnota.setText(QString::number(protKom.answerDouble(modTepOkoli)));
		miskaABoxAnswer.click();
		miskaBBoxAnswer.click();
	});


	//Spusteni aplikace
	w.show();


	return a.exec();
}
