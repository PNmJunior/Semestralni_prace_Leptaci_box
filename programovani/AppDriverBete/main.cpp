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
#include <QFrame>
#include <QLCDNumber>
#include <QThread> 
#include <QDoubleSpinBox>

#define Bluetooth
#define SerialPort

//inicializace souboru...
#include "definice.h"//Spolecny soubor pro Arduino
#include "protokolKomunikace.h"//komunikacni protokol
#include "qcustomplot.h"//graf
#include "transportDat.h"


//main
int main(int argc, char ** argv)
{
	QApplication a(argc, argv);
	QTimer timerAkt1 ;//pravidelna aktualizace dat
	QList<QSerialPortInfo> seznamPort = QSerialPortInfo::availablePorts();//seznam vsech portu v PC
	QSerialPortInfo vybranyPort;//Port na kterem je Leptaci box
	QSerialPort serialovyPort;//inicializace vybraneho portu
	QWidget w;
	QVBoxLayout vbox;
	//Oblasti grafiky
	QHBoxLayout potrSerialSetBox;//nastaveni portu
	QHBoxLayout potrSerialSetBox2;//nastaveni portu text
	QGridLayout joystick;//ovladani motoru X a Z
	QHBoxLayout aktVseBox;//Tlacitko Aktualizovat vse
	QHBoxLayout miskaABox;//Oblast Misky A
	QHBoxLayout miskaBBox;//Oblast Misky B
	QHBoxLayout svetloABox;//kontrola osvetleni misky A
	QHBoxLayout svetloBBox;//kontrola osvetleni misky B
	QHBoxLayout teplotaABox;//kontrola ohrevu misky A
	QHBoxLayout teplotaAGrafBox;//Texty k grafu
	QVBoxLayout teplotaAGrafBox2;//grafu
	QHBoxLayout ohrevABox;//kontrola teploty misky A
	QHBoxLayout ohrevBBox;//kontrola teploty misky B
	QHBoxLayout vnijsiTeplotaBox;//teplota v Leptacim boxu

	
	//font pro indikaci Ohrivani
	QFont fOhrev = QFont("Arial", 20, QFont::Bold);
	//font pro Nadpisy
	QFont fNadpis = QFont("Arial", 15, QFont::Bold);
	//Horizontalni cary
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
	potrSerialSetBoxText.setFont(fNadpis);
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
	potrSerialSetBox2.addWidget(&potrSerialSetBoxTextStatus);
	potrSerialSetBox2.addWidget(&potrSerialSetBoxStatus);
	//potrSerialSetBox.addWidget(&potrSerialSetBoxButAnswerAll);


	QPushButton BoxButAnswerAll("Aktualizovat vsechny data");//Aktualizuje vsechny data
	aktVseBox.addWidget(&BoxButAnswerAll);

	//Text k motorum
	QLabel motorBoxText("Joysticky pro ovladani ramene:",&w);
	motorBoxText.setFont(fNadpis);

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


	//miska A s nastavenim regulace ohrevu misky A
	QLabel miskaABoxText("Miska A:",&w),miskaABoxMaxTepText("Nastav maximalni teplotu:",&w);
	QDoubleSpinBox miskaABoxMaxSpin(&w);
	miskaABoxMaxSpin.setRange(minTep,maxTep);
	miskaABoxMaxSpin.setStyleSheet("background-color: white;");
	miskaABoxMaxSpin.setValue(40.0);
	miskaABoxText.setFont(fNadpis);
	QPushButton miskaABoxAnswer("Aktualizace");
	miskaABox.addWidget(&miskaABoxText);
	miskaABox.addWidget(&miskaABoxAnswer);
	miskaABox.addWidget(&miskaABoxMaxTepText);
	miskaABox.addWidget(&miskaABoxMaxSpin);


	//Regulace podsviceni
	QLabel svetloABoxText("Regulace podsviceni",&w);
	QSlider svetloABoxSlider( Qt::Horizontal,&w);
	svetloABoxSlider.setMaximum(100);
	svetloABoxSlider.setMinimum(0);
	svetloABoxSlider.setPageStep(1);
	svetloABoxSlider.setSingleStep(1);

	svetloABox.addWidget(&svetloABoxText);
	svetloABox.addWidget(&svetloABoxSlider);


	//Indikace teploty misky
	QLCDNumber teplotaABoxIndikatorTeploty(7,&w);
	teplotaABoxIndikatorTeploty.setSegmentStyle(QLCDNumber::Flat);
	teplotaABoxIndikatorTeploty.setMinimumHeight(100);
	teplotaABoxIndikatorTeploty.setFixedWidth(400);
	QLabel teplotaABoxName("Teplota: ",&w); 
	teplotaABox.addWidget(&teplotaABoxName);
	teplotaABox.addWidget(&teplotaABoxIndikatorTeploty);
	


	//Regulace ohrevu misky
	QLabel ohrevABoxText("Ohrev: "), ohrevABoxStav("??????");
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


	//miska B
	QLabel miskaBBoxText("Miska B:",&w);
	miskaBBoxText.setFont(fNadpis);
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


	//Regulace ohrevu misky
	QLabel ohrevBBoxText("Ohrev: "), ohrevBBoxStav("??????");
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
	QLabel vnijsiTeplotaBoxName("Teplota uvnitr boxu: ",&w), vnijsiTeplotaBoxEmpty(" ",&w); 
	vnijsiTeplotaBoxName.setFont(fNadpis);
	vnijsiTeplotaBox.addWidget(&vnijsiTeplotaBoxEmpty);
	QLCDNumber vnijsiTeplotaBoxIndikator(7,&w);
	vnijsiTeplotaBoxIndikator.setSegmentStyle(QLCDNumber::Flat);
	vnijsiTeplotaBoxIndikator.setMinimumHeight(100);
	vnijsiTeplotaBoxIndikator.setFixedWidth(400);
	vnijsiTeplotaBox.addWidget(&vnijsiTeplotaBoxIndikator);

	//graf vyvoje a predikce teploty v misce A
	//potrebne promenne Eulerovy metody
	int ohrevMiskyA;//promenna pro ulozeni intenzity ohrevu misky
	long gTimeSecund = 0;//Casova osa grafu
	double gTimeAddSec = 2;//cas jednoho kroku
	double gPredTeplotaZtrata_s = 0.005;//velikost ubytku teploty 
	double gPredTeplotaMaxOhrev_s = (0.1)/255;//velikost ohrevu misky pri maximalnimu ohrevu
	dtTep gTeplotaMiskyA;//hodnota pro ulozeni teploty misky
	dtTep gTeplotaMiskyAOld = minTep;//predchozi udaj o teplote misky
	int gTimePredikce_s = 60;//Cas zobrazeni predikce v grafu
	int gTimeDataZobrazeni_s = 60;//cas zobrazeni poslednich dat za ...
	int gPocetKrokuGrafu = gTimeDataZobrazeni_s/gTimeAddSec;
	//Data pro graf
	QVector<double> gZaznamTeploty;
	QVector<double> gZaznamTeplotyCas;
	QVector<double> gPredikceVivojeTep;
	QVector<double> gPredikceVivojeTepCas;
	//inicializace grafu
	QCustomPlot *plot = new QCustomPlot(&w);
	plot->addGraph();
	plot->addGraph();
	plot->graph(0)->setPen(QPen(Qt::blue));
	plot->graph(0)->setName("Teplota");
	plot->graph(1)->setPen(QPen(Qt::red));
	plot->graph(1)->setName("Predikce");
	plot->setMinimumSize(50,150);
	plot->replot();

	QLabel teplotaAGrafBoxNadpis("Teplota v grafu",&w), teplotaAGrafBoxTeplota("Namerena teplota",&w), teplotaAGrafBoxPredikce("Predikovana teplota",&w);
	teplotaAGrafBoxNadpis.setFont(fNadpis);
	teplotaAGrafBoxTeplota.setStyleSheet("QLabel {  color : blue; }");
	teplotaAGrafBoxPredikce.setStyleSheet("QLabel {  color : red; }");


	teplotaAGrafBox.addWidget(&teplotaAGrafBoxNadpis);
	teplotaAGrafBox.addWidget(&teplotaAGrafBoxTeplota);
	teplotaAGrafBox.addWidget(&teplotaAGrafBoxPredikce);


	//Pridani vsech vrstev do QVBoxLayout
	vbox.addLayout(&potrSerialSetBox);
	vbox.addLayout(&potrSerialSetBox2);
	vbox.addLayout(&aktVseBox);
	vbox.addWidget(&oddelPort);
	vbox.addWidget(&motorBoxText);
	vbox.addLayout(&joystick);
	vbox.addWidget(&oddelMotor);
	vbox.addLayout(&miskaABox);
	vbox.addLayout(&svetloABox);
	vbox.addLayout(&teplotaABox);
	vbox.addLayout(&teplotaAGrafBox);
	vbox.addLayout(&teplotaAGrafBox2);
	vbox.addWidget(plot);
	vbox.addLayout(&ohrevABox);
	vbox.addWidget(&oddelMiskaA);
	vbox.addLayout(&miskaBBox);
	vbox.addLayout(&svetloBBox);
	vbox.addLayout(&ohrevBBox);
	vbox.addWidget(&oddelMiskaB);
	vbox.addWidget(&vnijsiTeplotaBoxName);
	vbox.addLayout(&vnijsiTeplotaBox);
	
	//pridani do QWidget
	w.setLayout(&vbox);

	//inicializace moji knihovny pro komunikaci
	protokolKomunikace protKom = protokolKomunikace(&serialovyPort,&potrSerialSetBoxStatus);


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
		//Kontrola vyberu Potru. Pokud velikosti jsou rozdilne tak je problem... Duvodem je neimplementovany operator== pro QSerialPortInfo
		if(potrSerialSetBoxCombSeznamPortu.currentIndex() == -1 || infos.size() != QSerialPortInfo::availablePorts().size())
		{
			potrSerialSetBoxStatus.setText("Zkontroluj pripojeni. Zmackni tlacitko Aktualizovat seznam. Vyber port ze seznamu.");
			potrSerialSetBoxButAkt.click();
		}
		else
		{
			try
			{
				vybranyPort = infos.at( potrSerialSetBoxCombSeznamPortu.currentIndex());//Vyber Portu
				serialovyPort.setPort(vybranyPort);//Nastavit port podle portInfo
				serialovyPort.open(QIODevice::ReadWrite);//druh komunikace
				serialovyPort.setBaudRate(QSerialPort::Baud9600);//rychlost
				//Dalsi parametry potrebne pro spravne fungovani serioveho portu
				serialovyPort.setDataBits(QSerialPort::Data8);
				serialovyPort.setParity(QSerialPort::NoParity);
				serialovyPort.setStopBits(QSerialPort::OneStop);
				serialovyPort.setFlowControl(QSerialPort::NoFlowControl);
				while (!serialovyPort.isOpen())//Otevreni portu
				{
					serialovyPort.open(QIODevice::ReadWrite);
				}
				if(!(serialovyPort.isWritable()&&serialovyPort.isOpen()))//Kontrola otevreni portu
				{
					protKom.notOpenInformation();
				}
				else//Uspesne nastavena komunikace
				{
					QString beta = QString("Komunikace s portem: ")+serialovyPort.portName();//Informace o navazani spojeni
					potrSerialSetBoxStatus.setText(beta);
					QThread::sleep(2);//Pro Navazani spojeni se zarizenim
					if(protKom.answerIdentifikace())//protKom.answerIdentifikace()
					{
						beta = QString("Leptaci box pripojen na portu: ")+serialovyPort.portName();
						timerAkt1.start(gTimeAddSec * 1000);//Nastaveni casovace, ktery pravidelne bude aktualizovat data.
					}
					else
					{
						beta = QString("Na Portu %1 neni Leptaci box").arg(serialovyPort.portName());
						serialovyPort.close();
					}
					potrSerialSetBoxStatus.setText(beta);
				}
			}
			catch(...)
			{
				protKom.notOpenInformation();
				serialovyPort.close();
			}
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
	//Odeslani instrukce pro MotorZ - Stop / Zapne vibrace
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
	//Kvuli nelinearite sviceni prevadim udaj z procent na rozsah 0-255 pomoci vzorce...
	QObject::connect(&svetloABoxSlider, QSlider::valueChanged, [&](){
		protKom.sendSvetloProc(modSvetloA,svetloABoxSlider.value());
		
	});
	//Odeslani instrukce pro SvetloA - nastaveni hodnoty
	//Kvuli nelinearite sviceni prevadim udaj z procent na rozsah 0-255 pomoci vzorce...
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
	//Odeslani dotazu na: teplotu misky A; stav Ohrevu A a od toho nastaveni upozorneni. 
	QObject::connect(&miskaABoxAnswer, QPushButton::clicked, [&](){
		//teplotaABoxHodnota.setText(QString::number(protKom.answerDouble(modTepNadrz)));//Zobrazeni teploty
		//teplotaABoxIndikatorTeploty.display(QString("%1 C").arg(protKom.answerDouble(modTepNadrz)));
		ohrevMiskyA = protKom.answerInt(modOhrevA);//informaci o Ohrevu
		if(ohrevMiskyA == -1)
		{
			ohrevABoxStav.setText("??????");
		}
		if (ohrevMiskyA == 0)
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
		if(ohrev == -1)
		{
			ohrevBBoxStav.setText("??????");
		}
		if (ohrev == 0)
		{
			ohrevBBoxStav.setText("Chlazeni");
		}
		else
		{
			ohrevBBoxStav.setText("!!!Topime!!!");
		}
		//ohrevBBoxSlider.setValue(ohrev);
	});
	//Odeslani Dotazu: svetloA, svetloB, teplota okoli, ohrevA, teplota misky A, ohrev B
	QObject::connect(&BoxButAnswerAll, QPushButton::clicked, [&](){
		svetloABoxSlider.setValue(protKom.answerProcenta(modSvetloA));
		svetloBBoxSlider.setValue(protKom.answerProcenta(modSvetloB));
		//teplotaVBoxHodnota.setText(QString::number(protKom.answerDouble(modTepOkoli)));
		vnijsiTeplotaBoxIndikator.display(QString("%1 C").arg(protKom.answerDouble(modTepOkoli)));
		miskaABoxAnswer.click();
		miskaBBoxAnswer.click();
	});
	//Pravidelna aktualizace udaju: teplota okoli, teplota misky A, ohrevA, ohrevB
	QObject::connect(&timerAkt1, QTimer::timeout, [&](){
		//teplotaVBoxHodnota.setText(QString::number(protKom.answerDouble(modTepOkoli)));
		double tepA = protKom.answerDouble(modTepOkoli);
		vnijsiTeplotaBoxIndikator.display(QString("%1 C").arg(tepA));
		//Pokud je teplota vyssi nez nastaveno, tak vypne ohrevA a zcervena SpinBox.
		if (tepA >= miskaABoxMaxSpin.value())
		{
			ohrevABoxSlider.setValue(0);
			ohrevABoxSlider.valueChanged(0),
			miskaABoxMaxSpin.setStyleSheet("background-color: red;");
		}
		else
		{
			miskaABoxMaxSpin.setStyleSheet("background-color: white;");
		}
		
		miskaABoxAnswer.click();
		miskaBBoxAnswer.click();
		gTeplotaMiskyA = protKom.answerDouble(modTepNadrz);
		teplotaABoxIndikatorTeploty.display(QString("%1 C").arg(gTeplotaMiskyA));
		
		
		//Graf
		//pridani novych udaju o teplote
		gZaznamTeploty.push_back(gTeplotaMiskyA);
		gZaznamTeplotyCas.push_back(gTimeSecund);
		
		if (gZaznamTeplotyCas.size()>gPocetKrokuGrafu)//Vymaze nadbytecna data o teplote
		{
			gZaznamTeplotyCas.remove(0);
			gZaznamTeploty.remove(0);
		}
		//Vykresleni grafu
		plot->graph(0)->setData(gZaznamTeplotyCas, gZaznamTeploty);
		plot->rescaleAxes();
		plot->replot();

		//Vymazani predikce
		gPredikceVivojeTep.clear();
		gPredikceVivojeTepCas.clear();

		double vektTep = (gTeplotaMiskyA - gTeplotaMiskyAOld)/gTimeAddSec;//pocatecny vektor teplty
		dtTep predN =  gTeplotaMiskyA;//pocatecni predikovana hodnota
		//pridani pocatecnich hodnot predikce do grafu
		gPredikceVivojeTepCas.push_back(gTimeSecund );
		gPredikceVivojeTep.push_back(predN);
		//Vypocet
		for (int i = gTimeAddSec ; i < gTimePredikce_s; i+= gTimeAddSec)
		{
			//Eulerova metoda
			double aA = ohrevMiskyA;
			gPredikceVivojeTepCas.push_back(gTimeSecund +  i);
			vektTep += gTimeAddSec*(gPredTeplotaMaxOhrev_s*aA - gPredTeplotaZtrata_s);
			predN += vektTep*gTimeAddSec;
			gPredikceVivojeTep.push_back(predN);
		}
		//vykresleni predikce
		plot->graph(1)->setData(gPredikceVivojeTepCas, gPredikceVivojeTep);
		plot->rescaleAxes();
		plot->replot();
		//Nastaveni promennych do dalsiho kroku.
		gTimeSecund += gTimeAddSec;
		gTeplotaMiskyAOld = gTeplotaMiskyA;

	});


	//Spusteni aplikace
	w.show();


	return a.exec();
}
