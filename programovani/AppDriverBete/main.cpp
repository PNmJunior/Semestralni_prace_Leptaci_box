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
#include <QGridLayout>
#include <QGamepad>
#include <QLine>
#include <QFrame>
#include <QLCDNumber>

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
	QGridLayout joystick;
	QHBoxLayout aktVseBox;//Oblast misky A
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
	QFrame oddelPort(&w);
	oddelPort.setFrameStyle( QFrame::HLine | QFrame::Plain);
	QFrame oddelMotor(&w);
	oddelMotor.setFrameStyle( QFrame::HLine | QFrame::Plain);
	QFrame oddelMiskaA(&w);
	oddelMiskaA.setFrameStyle( QFrame::HLine | QFrame::Plain);
	QFrame oddelMiskaB(&w);
	oddelMiskaB.setFrameStyle( QFrame::HLine | QFrame::Plain);



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
	
	potrSerialSetBox.addWidget(&potrSerialSetBoxText);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButAkt);
	potrSerialSetBox.addWidget(&potrSerialSetBoxCombSeznamPortu);
	potrSerialSetBox.addWidget(&potrSerialSetBoxButOk);
	potrSerialSetBox.addWidget(&potrSerialSetBoxTextStatus);
	potrSerialSetBox.addWidget(&potrSerialSetBoxStatus);
	//potrSerialSetBox.addWidget(&potrSerialSetBoxButAnswerAll);


	QPushButton BoxButAnswerAll("Aktualizovat vsechny data");//Aktualizuje vsechny data
	aktVseBox.addWidget(&BoxButAnswerAll);

	//Text k motorum
	QLabel motorBoxText("Joysticky pro ovladani ramene:",&w);

	//Rizeni Motoru X
	QPushButton motorXBoxLeftB("<<"),motorXBoxLeftS("<"),motorXBoxRightS(">"),motorXBoxRightB(">>");

	
	//Rizeni Motoru Z
	QPushButton motorZBoxUp("Nahoru"), motorZBoxDown("Dolu"), motorZBoxStop_Vibration("Vibrace");
	dtMotorZ mZ;
	QString motorZBoxtextVibrace = QString("Vibrace");
    QString motorZBoxtextStop = QString("Stop");

	
	joystick.addWidget(&motorZBoxUp, 0,2);
	joystick.addWidget(&motorZBoxStop_Vibration, 1,2);
	joystick.addWidget(&motorZBoxDown, 2,2);
	joystick.addWidget(&motorXBoxLeftB, 1,0);
	joystick.addWidget(&motorXBoxLeftS, 1,1);
	joystick.addWidget(&motorXBoxRightS, 1,3);
	joystick.addWidget(&motorXBoxRightB, 1,4);


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
	QLCDNumber indikNadrze(7,&w);
	indikNadrze.setSegmentStyle(QLCDNumber::Flat);
	indikNadrze.setMinimumHeight(100);
	indikNadrze.setMinimumWidth(500);
	QLabel teplotaABoxName("Teplota: ",&w), teplotaABoxHodnota("00,00",&w), teplotaABoxJednotka("°C",&w); 
	teplotaABox.addWidget(&teplotaABoxName);
	//teplotaABox.addWidget(&teplotaABoxHodnota);
	//teplotaABox.addWidget(&teplotaABoxJednotka);
	teplotaABox.addWidget(&indikNadrze);
	


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
	//teplotaVBox.addWidget(&teplotaVBoxHodnota);
	//teplotaVBox.addWidget(&teplotaVBoxJednotka);
	QLCDNumber indikVnejTep(7,&w);
	indikVnejTep.setSegmentStyle(QLCDNumber::Flat);
	indikVnejTep.setMinimumHeight(100);
	indikVnejTep.setMinimumWidth(500);
	teplotaVBox.addWidget(&indikVnejTep);


	//Pridani vsech vrstev do QVBoxLayout
	
	vbox.addLayout(&potrSerialSetBox);
	vbox.addLayout(&aktVseBox);
	vbox.addWidget(&oddelPort);
	vbox.addWidget(&motorBoxText);
	vbox.addLayout(&joystick);
	vbox.addWidget(&oddelMotor);
	vbox.addLayout(&miskaABox);
	vbox.addLayout(&svetloABox);
	vbox.addLayout(&teplotaABox);
	vbox.addLayout(&ohrevABox);
	vbox.addWidget(&oddelMiskaA);
	vbox.addLayout(&miskaBBox);
	vbox.addLayout(&svetloBBox);
	vbox.addLayout(&ohrevBBox);
	vbox.addWidget(&oddelMiskaB);
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
	//Odeslani instrukce pro MotorZ - Stop - Zastavi vibrace
	QObject::connect(&motorZBoxStop_Vibration, QPushButton::clicked, [&](){
		if (mZ  == vibMotorZOn)
		{
			protKom.sendMotZ(stopMotorZ);
			motorZBoxStop_Vibration.setText(motorZBoxtextVibrace);
			mZ = stopMotorZ;
		}
		else
		{
			protKom.sendMotZ(vibMotorZOn);
			motorZBoxStop_Vibration.setText(motorZBoxtextStop);
			mZ = vibMotorZOn;
		}
		
		
	});
	//Odeslani instrukce pro MotorZ - Nahoru
	QObject::connect(&motorZBoxUp, QPushButton::clicked, [&](){
		protKom.sendMotZ(nahorumotorZ);
	});
	//Odeslani instrukce pro MotorZ - Dolu
	QObject::connect(&motorZBoxDown, QPushButton::clicked, [&](){
		protKom.sendMotZ(dolumotorZ);
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
		//teplotaABoxHodnota.setText(QString::number(protKom.answerDouble(modTepNadrz)));//Zobrazeni teploty
		indikNadrze.display(QString("%1 C").arg(protKom.answerDouble(modTepNadrz)));
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
	QObject::connect(&BoxButAnswerAll, QPushButton::clicked, [&](){
		svetloABoxSlider.setValue(protKom.answerSvetlo(modSvetloA));
		svetloBBoxSlider.setValue(protKom.answerSvetlo(modSvetloB));
		teplotaVBoxHodnota.setText(QString::number(protKom.answerDouble(modTepOkoli)));
		miskaABoxAnswer.click();
		miskaBBoxAnswer.click();
	});
	//Pravidelna aktualizace udaji: teplota okoli, teplota nadrze A, ohrevA, ohrevB
	QObject::connect(&tim1, QTimer::timeout, [&](){
		//teplotaVBoxHodnota.setText(QString::number(protKom.answerDouble(modTepOkoli)));
		indikVnejTep.display(QString("%1 C").arg(protKom.answerDouble(modTepOkoli)));
		miskaABoxAnswer.click();
		miskaBBoxAnswer.click();
	});


	//Spusteni aplikace
	w.show();


	return a.exec();
}
