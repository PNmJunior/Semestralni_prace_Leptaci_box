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

	QLabel potrSerialSetBoxText("Vyber portu:",&w);
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


	QLabel motorXBoxText("Kontrola motoru X:",&w);
	QPushButton motorXBoxLeftB("<<"),motorXBoxLeftS("<"), motorXBoxStop("Stop1234"),motorXBoxRightS(">"),motorXBoxRightB(">>");
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

	QObject::connect(&potrSerialSetBoxButAkt, QPushButton::clicked, [&](){
		potrSerialSetBoxCombSeznamPortu.clear();
		infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos)
        potrSerialSetBoxCombSeznamPortu.addItem(info.portName());
	});


	QObject::connect(&potrSerialSetBoxButOk, QPushButton::clicked, [&](){
		if(potrSerialSetBoxCombSeznamPortu.currentIndex() == -1)
		{
			//problem
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
			//problem
		}
	});

	QObject::connect(&motorXBoxLeftS, QPushButton::clicked, [&](){
		int r = levoSMotorX;
		QString alfa = QString("%1%2\n").arg((char)modMotorX).arg((int)levoSMotorX,4);
		motorXBoxText.setText(alfa);
		QByteArray j = alfa.toUtf8();
		if(serPort.isOpen())
		{
		serPort.write(j);
		serPort.flush();
		}
		else
		{
			//problem
		}
	});


	w.show();

	return a.exec();
}
