/*
#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "main.cpp"


class transportDat
{
private:
QSerialPort *serialPort;//ukazatel na seriovy port


public:

transportDat(QSerialPort *SerPort);
~transportDat();
    bool isOpen();
    qint64 write(const QByteArray &data);
    bool flush();
    void close();
    bool timeOut();
    qint64 available();
    QByteArray read(qint64 delka);
    bool begin();
    
    
};

transportDat::transportDat(QSerialPort *SerPort);
{
    serialPort = SerPort;
}

transportDat::~transportDat()
{
}


bool transportDat::isOpen(){}
qint64 transportDat::write(const QByteArray &data){}
bool transportDat::flush(){}
void transportDat::close(){}
bool transportDat::timeOut(){}
qint64 transportDat::available(){}
QByteArray transportDat::read(qint64 delka){}
bool transportDat::begin(){}
*/
#include <QApplication>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <qmath.h>
#include <QThread>  

class prepravaDat
{
private:
    
public:
    prepravaDat(/* args */);
    ~prepravaDat();
bool isOpen();
};

prepravaDat::prepravaDat(/* args */)
{
}

prepravaDat::~prepravaDat()
{
}

bool prepravaDat::isOpen()
{
    return true;
}