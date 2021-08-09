#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "main.cpp"
/*
#ifdef(Bluetooth)

#endif
#ifdef(SerialPort)

#endif
*/
#include <QSerialPort>
#include <QSerialPortInfo>


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
