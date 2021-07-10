#include <QApplication>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include "definice.h"

class protokolKomunikace
{
private:
    QSerialPort *serP;
    QLabel *textLabel;
    
public:
    protokolKomunikace(QSerialPort *Ser_p, QLabel *textL);
    ~protokolKomunikace();
    QString notOpen();
    void notOpenWrite();
    void sendMotX(dtMotorX a);
    void sendMotZ(dtMotorZ a);
};

protokolKomunikace::protokolKomunikace(QSerialPort *Ser_p, QLabel *textL)
{
    serP = Ser_p;
    textLabel = textL;
}

protokolKomunikace::~protokolKomunikace()
{
}
QString protokolKomunikace::notOpen()
{
    //return QString("Not open serial port: ") + serP->portName();

    return QString("Nelze komunikovat s serialovym portem: ") + serP->portName();
}

void protokolKomunikace::notOpenWrite()
{
    textLabel->setText(notOpen());
}

void protokolKomunikace::sendMotX(dtMotorX a)
{
    	QString alfa = QString("%1%2\n").arg((char)modMotorX).arg((int)a,4);
		//textLabel->setText(alfa);
		QByteArray j = alfa.toUtf8();
		if(serP->isOpen())
		{
		serP->write(j);
		serP->flush();
		}
		else
		{
			notOpenWrite();
		}
}


void protokolKomunikace::sendMotZ(dtMotorZ a)
{
    	QString alfa = QString("%1%2\n").arg((char)modMotorZ).arg((int)a,4);
		//textLabel->setText(alfa);
		QByteArray j = alfa.toUtf8();
		if(serP->isOpen())
		{
		serP->write(j);
		serP->flush();
		}
		else
		{
			notOpenWrite();
		}
}
