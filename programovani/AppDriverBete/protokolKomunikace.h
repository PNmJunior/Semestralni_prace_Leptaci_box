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
    bool sendMotZleftS();
    QString notOpen();
    void notOpenWrite();
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
    return QString("Not open serial port: ") + serP->portName();
}

void protokolKomunikace::notOpenWrite()
{
    textLabel->setText(notOpen());
}
bool protokolKomunikace::sendMotZleftS()
{
    	QString alfa = QString("%1%2\n").arg((char)modMotorX).arg((int)levoSMotorX,4);
		textLabel->setText(alfa);
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

