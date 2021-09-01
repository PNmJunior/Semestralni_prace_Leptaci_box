#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QByteArray>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
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



public:
QSerialPort *serialovyPort;//ukazatel na seriovy port
QSerialPortInfo *serialPortInfo;
transportDat(QSerialPort *SerPort, QSerialPortInfo *SerPortInf);
QList<QSerialPortInfo> seznam;
~transportDat();
    bool isOpen();
    qint64 write(const QByteArray &data);
    bool flush();
    void close();
    bool timeOut();
    qint64 bytesAvailable();
    QByteArray read(qint64 delka);
    bool begin(const QSerialPortInfo &b);
    QString portName();

    
    
};

transportDat::transportDat(QSerialPort *SerPort, QSerialPortInfo *SerPortInf)
{
    serialovyPort = SerPort;
    serialPortInfo = SerPortInf;
    seznam = QSerialPortInfo::availablePorts();
    
}

transportDat::~transportDat()
{
}


bool transportDat::isOpen(){return serialovyPort->isOpen();}
qint64 transportDat::write(const QByteArray &data){return serialovyPort->write(data);}
bool transportDat::flush(){return serialovyPort->flush();}
void transportDat::close(){serialovyPort->close();}
bool transportDat::timeOut()
{
        serialovyPort->waitForBytesWritten();//cekani
        serialovyPort->waitForReadyRead();//cekani
        return true;
}
qint64 transportDat::bytesAvailable(){return serialovyPort->bytesAvailable();}
QByteArray transportDat::read(qint64 delka){return serialovyPort->read(delka);}
bool transportDat::begin(const QSerialPortInfo &b)
{
    serialovyPort->setPort(b);//Nastavit port podle portInfo
    serialovyPort->open(QIODevice::ReadWrite);//druh komunikace
    serialovyPort->setBaudRate(QSerialPort::Baud9600);//rychlost
    //Dalsi parametry potrebne pro spravne fungovani serioveho portu
    serialovyPort->setDataBits(QSerialPort::Data8);
    serialovyPort->setParity(QSerialPort::NoParity);
    serialovyPort->setStopBits(QSerialPort::OneStop);
    serialovyPort->setFlowControl(QSerialPort::NoFlowControl);
    while (!serialovyPort->isOpen())//Otevreni portu
    {
        serialovyPort->open(QIODevice::ReadWrite);
    }
    if((serialovyPort->isWritable()&&serialovyPort->isOpen()))//Kontrola otevreni portu
    {
        return true;
    }
    return false;
}
QString transportDat::portName()
{
    return serialovyPort->portName();
}