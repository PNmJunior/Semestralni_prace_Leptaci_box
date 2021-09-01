#include <QApplication>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <qmath.h>
#include <QThread>  
#include "definice.h"//Spojecny soubor pro Arduino


class protokolKomunikace
{
private:
    transportDat *serialPort;//ukazatel na seriovy port   transportDat
    QLabel *textLabel;//ukazatel na QLabel, kde bude informovat o stavu otevreni portu. Informovani o chybe...
    
public:
    protokolKomunikace(transportDat *SerialP, QLabel *textL);//Inicializace
    ~protokolKomunikace();
    QString notOpen();//Text s informaci, ktery port nejde otevrit
    void notOpenInformation();//Vypise text o neotevreni portu na QLabel
    void send(QByteArray sendText);//Odesle data na vstupu
    //bool: kontola vstupnich dat
    bool sendMotX(dtMotorX smer);//odeslani instrukci pro MotorX
    bool sendMotZ(dtMotorZ smer);//odeslani instrukci pro MotorZ
    bool sendSvetlo(char mod_svetlo,int val);//odeslani instrukci pro Svetlo, vstup: mode, 0-255
    bool sendSvetloProc(char mod_svetlo, int proc);//prevede podle vzorce procenta na rozsah 0-255 a posle pres sendSvetlo
    bool sendOhrev(char mod_ohrev,int val);//odeslani instrukci pro Ohrev
    
    QByteArray quest(char mod_allKom);//Odesle dotaz na vstupni mod a vystup je odpoved na otazku
    QString answer( char mod_allKom);//Zkontroluje vystup z quest a prevede data na QString
    int answerInt(char mod_allKom);//Prevede answer na INT
    double answerDouble(char typ);//Prevede answer na DOUBLE
    int answerProcenta(char mod_svetlo);//Prevede answer na svetlo ve formatu procenta -> 0-100
    bool answerIdentifikace();//True v pripade spravne odpovedi na Identifikacni otazku
};


protokolKomunikace::protokolKomunikace(transportDat *SerialP, QLabel *textL)
{
    serialPort = SerialP;
    textLabel = textL;
}


protokolKomunikace::~protokolKomunikace()
{
    
}


void protokolKomunikace::send(QByteArray sendText)
{
    try
    {   
        if(serialPort->isOpen())//Kontrola otevreni portu
        {
            serialPort->write(sendText);//Ulozeni do buffer
            serialPort->flush();//Odeslani do Leptaciho boxu
        }
        else
        {
            notOpenInformation();//Odeslani chybne hlasky
            serialPort->close();
        }
    }
    catch(...)
    {
        notOpenInformation();//Odeslani chybne hlasky
        serialPort->close();
    }
}


QString protokolKomunikace::notOpen()
{
    
    return QString("Nelze komunikovat se serialovym portem: ") + serialPort->portName();
}


void protokolKomunikace::notOpenInformation()
{
    textLabel->setText(notOpen());
}


bool protokolKomunikace::sendMotX(dtMotorX smer)
{
    //Konntrola vstupu
    if((smer != levoBMotorX && smer!=levoSMotorX) && ((smer!=pravBMotorX && smer!=pravSMotorX) && smer!=stopMotorZ)){return false;}
    //Format zpravy
    QString alfa = QString("%1%2\n").arg((char)modMotorX).arg((int)smer,4);
    send(alfa.toUtf8());//poslani dat
    return true;
}


bool protokolKomunikace::sendMotZ(dtMotorZ smer)
{
    //Kontrola vstupu
    if((smer != stopMotorZ &&  smer!= vibMotorZOn) &&( smer != dolumotorZ && smer != nahorumotorZ )){return false;}
    //Format zpravy
    QString alfa = QString("%1%2\n").arg((char)modMotorZ).arg((int)smer,4);
    send(alfa.toUtf8());//poslani dat
    return true;
}


bool protokolKomunikace::sendOhrev(char mod_ohrev,int val)
{
    //Kontrola vstupu
    if((mod_ohrev !=modOhrevA && mod_ohrev != modOhrevB) || val > 255 ){ return false;}
    //Format zpravy
    QString alfa = QString("%1%2\n").arg((char)mod_ohrev).arg((int)val,4);
    send(alfa.toUtf8());//poslani dat
    return true;
}


bool protokolKomunikace::sendSvetlo(char mod_svetlo,int val)
{
    //Kontrola vstupu
    if((mod_svetlo !=modSvetloA && (mod_svetlo!= modSvetloB && mod_svetlo != modSvetloC)) || val > maxSvetlo )
    { 
        return false;
    }
    //Format zpravy
    QString alfa = QString("%1%2\n").arg((char)mod_svetlo).arg((int)val,4);
    send(alfa.toUtf8());//poslani dat
    return true;
}


bool protokolKomunikace::sendSvetloProc(char mod_svetlo, int proc)
{
    int i;
    //Kontrola vstupu
    if(proc < 0 || proc > 100)
    {
        return false;
    }
    if(proc == 0){i=minSvetlo;}//0% = vypnute svetlo => 0
    else if (proc == 100){i = maxSvetlo;}//100% = maximum sviceni => 255
    else if (proc <= 19) {i = 1;}//od 1% do 19% = minimalni sviceni => 1
    else//od 20% do 99% = rozsah pro vzorec: y = 1.741^(x/10)
    {
        i = qPow(1.741,proc/10.0);
        if (i > maxSvetlo)i = maxSvetlo;//Kontrola preteceni
    }
    return sendSvetlo(mod_svetlo,i);//Odeslani
}


QByteArray protokolKomunikace::quest(char mod_allKom)
{
    try
    {
        if(serialPort->isOpen() == false)//Kontrola otevreni portu
        {
            notOpenInformation();
            return QString(Problem).toUtf8();
        }
        QString alfa = QString("%1%2\n").arg((char)mod_allKom).arg(Dotaz);//Formulace dotazu
        send(alfa.toUtf8());//Poslani dotazu
        serialPort->timeOut();
        for (int i = 0; i <20 && serialPort->bytesAvailable()<6; i++)
        {
            textLabel->setText("cekam");
            QThread::msleep(100);
        }
        if((serialPort->bytesAvailable()<6))
        {
            notOpenInformation();
            serialPort->close();
            return QString(Problem).toUtf8();
        }
        return serialPort->read(6);//poslani odpovedi
    }
    catch(...)
    {
        notOpenInformation();
        serialPort->close();
        return QString(Problem).toUtf8();
    }
}


QString protokolKomunikace::answer(char mod_allKom)
{
    QByteArray in = quest(mod_allKom);//Poslani otazky a ziskani odpovedi
    if (mod_allKom != in.at(0))// kontola modu
    {
        return QString("-1");
    }
    return QString(in.mid(1,4));  //prevod na QString
}


int protokolKomunikace::answerInt(char mod_allKom)
{
    return answer(mod_allKom).toInt();//prevod na INT
}


double protokolKomunikace::answerDouble(char mod_allKom)
{
    return answer(mod_allKom).toDouble() / tepNas;//Prevod na desetinne cislo pro teplomer
}


int protokolKomunikace::answerProcenta(char mod_svetlo)
{
    int hod = answerInt(mod_svetlo);//Nacteni typu
    if (hod == 0|| hod == -1)//vyple svetlo nebo chyba
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


bool protokolKomunikace::answerIdentifikace()
{
    if(answer(modIdentifikace).toInt() == numIdentifikace)
    {
        return true;
    }   
    else
    {
        return false;
    }
}