#include <QApplication>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <qmath.h>
#include "definice.h"


class protokolKomunikace
{
private:
    QSerialPort *serP;//ukazatel na seriovy port
    QLabel *textLabel;//ukazatel na QLabel, kde bude informovat o stavu otevreni portu. Informovani o chybe.
    
public:
    protokolKomunikace(QSerialPort *Ser_p, QLabel *textL);//Inicializace
    ~protokolKomunikace();
    QString notOpen();//Text s informaci, ktery port nejde otevrit
    void notOpenWrite();//Vypise text o neotevreni portu na QLabel
    void send(QByteArray j);//Odesle data na vstupu
    //bool: kontola vstupnich dat
    bool sendMotX(dtMotorX a);//odeslani instrukci pro MotorX
    bool sendMotZ(dtMotorZ a);//odeslani instrukci pro MotorZ
    bool sendSvetlo(char typ,int val);//odeslani instrukci pro Svetlo, vstup: mode, 0-255
    bool sendSvetloProc(char typ, int proc);//prevede podle vzorve procenta na 0-255 a pres sendSvetlo posle
    bool sendOhrev(char typ,int val);//odeslani instrukci pro Ohrev
    
    QByteArray quest(char typ);//Odesle dotas na vstupni mod a vystup je odpoved na otazku
    QString answer( char typ);//Obsahu kontrolu a Data z odpovedi prevede na QString
    int answerInt(char typ);//Prevede answer na INT
    double answerDouble(char typ);//Prevede answer na DOUBLE
    int answerSvetlo(char typ);//Prevede answer na svetlo ve formatu procenta -> 0-100
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
    if(serP->isOpen())//Kontrola otevreni portu
    {
        serP->write(j);//Ulozeni do buffer
        serP->flush();//Odeslani do Arduna
    }
    else
    {
        notOpenWrite();//Odeslani chybne hlasky
    }
}


QString protokolKomunikace::notOpen()
{
    return QString("Nelze komunikovat s serialovym portem: ") + serP->portName();
}


void protokolKomunikace::notOpenWrite()
{
    textLabel->setText(notOpen());
}


bool protokolKomunikace::sendMotX(dtMotorX a)
{
    //Konntrola vstupu
    if((a != levoBMotorX && a!=levoSMotorX) && ((a!=pravBMotorX && a!=pravSMotorX) && a!=stopMotorZ)){return false;}
    //Format zpravy
    QString alfa = QString("%1%2\n").arg((char)modMotorX).arg((int)a,4);
    send(alfa.toUtf8());//poslani dat
    return true;
}


bool protokolKomunikace::sendMotZ(dtMotorZ a)
{
    //Konntrola vstupu
    if((a != stopMotorZ &&  a!= vibMotorZOn) &&( a != dolumotorZ && a != nahorumotorZ )){return false;}
    //Format zpravy
    QString alfa = QString("%1%2\n").arg((char)modMotorZ).arg((int)a,4);
    send(alfa.toUtf8());//poslani dat
    return true;
}


bool protokolKomunikace::sendOhrev(char typ,int val)
{
    //Konntrola vstupu
    if((typ !=modOhrevA && typ != modOhrevB) || val > 255 ){ return false;}
    //Format zpravy
    QString alfa = QString("%1%2\n").arg((char)typ).arg((int)val,4);
    send(alfa.toUtf8());//poslani dat
    return true;
}


bool protokolKomunikace::sendSvetlo(char typ,int val)
{
    //Konntrola vstupu
    if((typ !=modSvetloA && (typ!= modSvetloB && typ != modSvetloC)) || val > maxSvetlo )
    { 
        return false;
    }
    //Format zpravy
    QString alfa = QString("%1%2\n").arg((char)typ).arg((int)val,4);
    send(alfa.toUtf8());//poslani dat
    return true;
}


bool protokolKomunikace::sendSvetloProc(char typ, int proc)
{
    int i;
    //Konntrola vstupu
    if(proc < 0 || proc > 100)
    {
        return false;
    }
    if(proc == 0){i=minSvetlo;}//0% = viple svetlo => 0
    else if (proc == 100){i = maxSvetlo;}//100% = maximum sviceni => 255
    else if (proc <= 19) {i = 1;}//do 1% do 19% = minimalni sviceni => 1
    else//od 20% do 99% = rozsah pro vzorec: y = 1.741^(x/10)
    {
        i = qPow(1.741,proc/10.0);
        if (i > maxSvetlo)i = maxSvetlo;//Kontrola preteceni
    }
    return sendSvetlo(typ,i);//Odeslani
}


QByteArray protokolKomunikace::quest(char typ)
{
    if(serP->isOpen() == false)//Kontrola otevreni
    {
        notOpenWrite();
        return QString(Problem).toUtf8();
    }
    QString alfa = QString("%1%2\n").arg((char)typ).arg(Dotaz);//Formulace Otasku
    send(alfa.toUtf8());//Poslani dotazu
    serP->waitForBytesWritten();//cekani
    serP->waitForReadyRead();//cekani
    while ((serP->bytesAvailable()<6))//cekani na odpoved
    {
            textLabel->setText("cekam");
    }
    return serP->read(6);//precteni odpoved
}


QString protokolKomunikace::answer(char typ)
{
    QByteArray in = quest(typ);//POslani otasky a ziskani odpovede
    if (typ != in.at(0))// kontola modu
    {
        return QString("-1");
    }
    return QString(in.mid(1,4));  //prevod na QString
}


int protokolKomunikace::answerInt(char typ)
{
    return answer(typ).toInt();//prevod na INT
}


double protokolKomunikace::answerDouble(char typ)
{
    return answer(typ).toDouble() / tepNas;//Prevod na desetine cislo pro teplomer
}


int protokolKomunikace::answerSvetlo(char typ)
{
    int hod = answerInt(typ);//Nacteni typu
    if (hod == 0|| hod == -1)//viple svetlo nebo chyba
    {
        return 0;
    }
    else if (hod == 255)//Maximum
    {
        return 100;
    }
    else if (hod == 1)//Minimalni sviceni
    {
        return 10;
    }
    else //Inverzni vzorec: y = 18.0356*log(x +1)
    {
        double vys = 18.0356*qLn(hod + 1);//vzorec
        int ret = vys;
        if (ret > 255)//kontola rozsahu
        {
            return 100;
        }
        if (ret < 0)
        {
            return 0;
        }
        return ret;
    }
    return 0;
}