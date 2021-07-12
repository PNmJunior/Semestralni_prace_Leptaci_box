#include <QApplication>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QSlider>
#include <qmath.h>
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
    bool sendMotX(dtMotorX a);
    bool sendMotZ(dtMotorZ a);
    bool sendSvetlo(char typ,dtSvetlo val);
    bool sendSvetloProc(char typ, int proc);
    void send(QByteArray j);
    QByteArray quest(char typ);
    bool chechAnswer(QByteArray in, char &typ, QString &data);
};

protokolKomunikace::protokolKomunikace(QSerialPort *Ser_p, QLabel *textL)
{
    serP = Ser_p;
    textLabel = textL;
}

protokolKomunikace::~protokolKomunikace()
{
}

void protokolKomunikace::send(QByteArray j)
{
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

QString protokolKomunikace::notOpen()
{
    //return QString("Not open serial port: ") + serP->portName();

    return QString("Nelze komunikovat s serialovym portem: ") + serP->portName();
}

void protokolKomunikace::notOpenWrite()
{
    textLabel->setText(notOpen());
}



bool protokolKomunikace::sendMotX(dtMotorX a)
{
    if((a != levoBMotorX && a!=levoSMotorX) && ((a!=pravBMotorX && a!=pravSMotorX) && a!=stopMotorZ)){return false;}
    QString alfa = QString("%1%2\n").arg((char)modMotorX).arg((int)a,4);
    //textLabel->setText(alfa);
    send(alfa.toUtf8());
    return true;
}


bool protokolKomunikace::sendMotZ(dtMotorZ a)
{
    if((a != stopMotorZ &&  a!= vibMotorZOn) &&( a != dolumotorZ && a != nahorumotorZ )){return false;}
    QString alfa = QString("%1%2\n").arg((char)modMotorZ).arg((int)a,4);
    //textLabel->setText(alfa);
    //QByteArray j = alfa.toUtf8();
    send(alfa.toUtf8());
    return true;
}


bool protokolKomunikace::sendSvetlo(char typ,dtSvetlo val)
{
    if((typ !=modSvetloA && (typ!= modSvetloB && typ != modSvetloC)) || val > maxSvetlo ){ return false;}
    QString alfa = QString("%1%2\n").arg((char)typ).arg((int)val,4);
    send(alfa.toUtf8());
    return true;
}
bool protokolKomunikace::sendSvetloProc(char typ, int proc)
{
    dtSvetlo i;
    if(proc < 0 || proc > 100)
    {
        return false;
    }
    if(proc == 0){i=minSvetlo;}
    else if (proc == 100){i = maxSvetlo;}
    else if (proc <= 19) {i = 1;}
    else
    {
        i = qPow(1.741,proc/10.0);
        if (i > maxSvetlo)i = maxSvetlo;
    }
    return sendSvetlo(typ,i);
}

QByteArray protokolKomunikace::quest(char typ)
{
    //serP->readAll();
    QString alfa = QString("%1%2\n").arg((char)typ).arg(Dotaz);
    textLabel->setText("1A");
    send(alfa.toUtf8());
    textLabel->setText("A2");
    serP->waitForBytesWritten();
    textLabel->setText("A3");
    serP->waitForReadyRead();
    textLabel->setText("A4");
    while ((serP->bytesAvailable()<6))
    {
            textLabel->setText("cekam");
    }
    //textLabel->setText(QString(serP->bytesAvailable()));
    

    return serP->read(6);
}

bool protokolKomunikace::chechAnswer(QByteArray in, char &typ, QString &data)
{
    if(in.length()!=5){return false;}
    typ = in.at(0);
    if (typ<modSvetloA|| typ > modTepOkoli)
    {
        return false;
    }
    data = QString(in.mid(1,4));
    
    
    
}