/* This code runs on Qt.
It takes pulse signal from microcontroller through serial I/O.
Uses Fuzzy Logic to generate value for velocity control.

*/





#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qextserialport.h"
#include <QTimer>
#include <QMessageBox>
#include <QMenuBar>
#include <QDebug>

// Fuzzy logic Variables
float NB=  -15;
float NM = -10;
float NS = -7;
float a   =-2;
float ZE   =0;
float b  =  2 ;
float PS  = 7;
float PM  = 10;
float PB  = 15;
float NBO =-13;
float NMO =-10;
 float NSO= -5;
float ZEO= 0;
 float PSO = 10;
float PMO  =16;
 float PBO = 19;
float setp= 16;


//bit             fiftmls;
 char   erroro=0,actualvalue,p1=0,pulse,temp,check=-86,err_l,err_r,a1=0,setPointl=7,setPointr=7,send1=0,pulsel,pulser;
char right1=0,left1=0;
 float Den=0,Num=0;
unsigned char   WML=120,WMR=120;
float deloutput;

unsigned char PWM = 70,PWML,PWMR;
unsigned char WM = 120;
int pr2 = 140, val = 0;
//char error = 0;
char err[3] = {0,0,0};
unsigned char val1 = 0, val2 = 0;
char bit_flag = 1;
char flag = 0, doit = 1;
unsigned char maxPWM=139,minPWM=6;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString com;
   // qDebug("ayoo");

    PWM = ui->horizontalSlider->value();
    com = ui->comboBox->currentText();
    port = new QextSerialPort(com);

    ui->pwmL->setDisabled(0);
    ui->pwmR->setDisabled(0);
    ui->errR->setDisabled(0);
    ui->errL->setDisabled(0);
    if(port->isOpen())
    {
        port->flush();
        port->close();
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(serialupdate()));
    timer->start(2);
}

MainWindow::~MainWindow()
{
    port->close();
    delete ui;
    delete port;
}


void MainWindow::serialupdate()
{
    port->flush();
    char buff[1024];
    if(port->bytesAvailable())
    {
        int i = port->read(buff, 1);
        buff[i] = '\0';
        erroro = buff[i-1];
        check=erroro;
       //a1=!a1;


       if(i != -1)
        {
              QString str(buff);
           /*  // QByteArray data = str.toAscii().toHex();
              //ui->pwmL-> append("0x"+data);
          /    if(bit_flag == 0)
              {
                  val2 = val1;
                  val1 = buff[i-1];
                  doit = !doit;
                  if(doit == 1)
                  {
                      val = int(val2<<8) + val1;
                      qDebug()<<val;
                  }
                //  ui->errL->append(QString::number(val));    //16-bit
              }
              else if(bit_flag == 1)
              {
                  ui->errL->append(QString::number(buff[i-1]));  //8-bit
              }*/
              algo('f');
        }
        else
            qDebug("cannot open");
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->pwmL->setDisabled(0);
    ui->errL->setDisabled(0);
    ui->pwmR->setDisabled(0);
    ui->errR->setDisabled(0);
    ui->comboBox->setDisabled(1);
    ui->comboBox_2->setDisabled(1);
    port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
  //  port->setTextModeEnabled(true);
    port->setQueryMode(QextSerialPort::EventDriven);
  //  port->setTimeout(100);
}

void MainWindow::on_pushButton_2_clicked()
{
   // ui->pwmL->setDisabled(1);
   // ui->errL->setDisabled(1);
    ui->comboBox->setDisabled(0);
    ui->comboBox_2->setDisabled(0);
    port->close();
}



void MainWindow::on_pushButton_3_clicked()
{
    ui->pwmL->clear();
    ui->errL->clear();
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
        if(index == 0)
            port->setBaudRate(BAUD4800);
        if(index == 1)
            port->setBaudRate(BAUD9600);
        if(index == 2)
            port->setBaudRate(BAUD14400);
        if(index == 3)
            port->setBaudRate(BAUD19200);
 }




void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->PWMtext->setText(QString::number(value));
    PWM = abs(pr2 * value/100);
}


void MainWindow::on_pushButton_4_clicked()
{
   QByteArray b;
   unsigned char c = PWM;
   QString sdata = QString(c);
   b = sdata.toAscii();
   qDebug()<<b;
   qDebug()<<PWM;
   ui->pwm_value->setText(QString::number(PWM));
   qDebug()<<port->write(b);
   b.clear();
   port->flush();
}


void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    if(index == 0)
        bit_flag = 1;
    else if(index == 1 )
        bit_flag = 0;
}






//FUZZY LOGIC INPUT DEGREE OF MEMBERSHIP FUNCTION

float err_NB(float CRISP)
    {
   if(CRISP<=NM) return ((NM-CRISP)/(NM-NB));

    else return 0;
    }

float err_NM(float CRISP)
{
  if(CRISP<=NM)return((CRISP-NB)/(NM-NB));
 else if(CRISP<=NS) return ((NS-CRISP)/(NS-NM));
 else return 0;
}

float err_NS(float CRISP)
{
    if (CRISP<=NS&&CRISP>=NM) return ((CRISP-NM)/(NS-NM));
    else if(CRISP<=0&&CRISP>=NS) return ((0-CRISP)/(0-NS));
    else return 0;
}
float err_ZE(float CRISP)
{
  if (CRISP<=0&&CRISP>=a) return ((CRISP-a)/a);
  else if(CRISP>=0&&CRISP<=b) return ((b-CRISP)/b);
  else return 0;
}
float err_PS(float CRISP)
{
  if(CRISP<=PS&&CRISP>=0)return (CRISP/PS);
else if(CRISP<=PM&&CRISP>=PS) return ((PM-CRISP)/(PM-PS));
else return 0;
}
float err_PM(float CRISP)
{
  if(CRISP<=PM&&CRISP>=PS)return((CRISP-PS)/(PM-PS));
  else if (CRISP<=PB&&CRISP>=PM) return((PB-CRISP)/(PB-PM));
  else return 0;
}
float err_PB(float CRISP)
{
  if(CRISP>=PM) return((CRISP-PM)/(PB-PM));
  else return 0;
}



//CALULATING CONSEQUENCE FOR FUZZY LOGIC
void NBout(float DOM)
{
  Den+=DOM;
  Num+=(DOM*NBO);
}
void NMout(float DOM)
{
  Den+=DOM;
  Num+=(DOM*NMO);
}

void NSout(float DOM)
{
  Den+=DOM;
  Num+=(DOM*NSO);
}
void ZEout(float DOM)
{
  Den+=DOM;
  Num+=(DOM*ZEO);
}
void PSout(float DOM)
{
  Den+=DOM;
  Num+=(DOM*PSO);
}
void PMout(float DOM)
{
  Den+=DOM;
  Num+=(DOM*PMO);
}
void PBout(float DOM)
{
  Den+=DOM;
  Num+=(DOM*PBO);
}

//calculating "deloutput"
void del_output(void)
{
   if(Den==0)deloutput=0;
   else deloutput= Num/Den;

}

//FUZZY ERROR CONTROL FUNCTION
void fuzzyerrorl(void)
{
 NBout(err_NB(err_l));   //error==NB then deloutput=NB
//NMout(err_NM(error));   //error==NM then deloutput=NM
NSout(err_NS(err_l));   //error==NS then deloutput=NS
ZEout(err_ZE(err_l));   //error==ZE then deloutput=ZE
PSout(err_PS(err_l));   //error==PS then deloutput=PS
//PMout(err_PM(error));   //error==PM then deloutput=PM
PBout(err_PB(err_l));   //error==PB then deloutput=PB
del_output();


}
void fuzzyerrorr(void)
{
 NBout(err_NB(err_r));   //error==NB then deloutput=NB
//NMout(err_NM(error));   //error==NM then deloutput=NM
NSout(err_NS(err_r));   //error==NS then deloutput=NS
ZEout(err_ZE(err_r));   //error==ZE then deloutput=ZE
PSout(err_PS(err_r));   //error==PS then deloutput=PS
//PMout(err_PM(error));   //error==PM then deloutput=PM
PBout(err_PB(err_r));   //error==PB then deloutput=PB
del_output();


}

void MainWindow::algo(char direction)
{
    qDebug("character value");
    qDebug()<<direction;

    if(direction=='f'){setPointl=7;setPointr=7;}
    else if(direction=='l'){setPointl=-7;setPointr=7;}
    else if(direction=='r'){setPointl=7;setPointr=-7;}
    else if(direction=='s'){setPointl=0;setPointr=0;}
    else if(direction=='b'){setPointl=-7;setPointr=-7;}


    if(check!=-86 && right1==1)
   {
     pulser=check;
     pulser=pulser/2;
      err_r=pulser;//setPointr-pulser;
     right1=0;
     check=0;
     send1=1;
    ui->errR->append(QString::number(err_r));
    //ui->errR->append(QString::number(err_r));
    }
   else if(check!=-86 && left1==1)
    {
      pulsel=check;
    err_l=pulsel;//setPointl-pulsel;
      left1=0;
      right1=1;
     ui->errL->append(QString::number(err_l));
     //ui->errL->append(QString::number(err_l));
    }


    else if(check==-86)
    {
        left1=1;
        //qDebug("lamo");
    }

    if(send1==1)
    {
        send1=0;
   // err[2] = error;

    //WM += 1*(err[2] - 2*err[1] + err[0]) + 2*(err[2] - err[1]) + 1*err[2];
    //err[0] = err[1];
   // err[1] = err[2];
//   signed char err_ll=err_l;
//    signed char err_rr=err_r;
    fuzzyerrorl();
        WML=WML+deloutput;
        Num=0;
        Den=0;
        deloutput=0;
  fuzzyerrorr();
        WMR=WMR+deloutput;
        Num=0;
        Den=0;
        deloutput=0;
        if(WML > maxPWM)
        WML = PWML = maxPWM;
    else if(WML < minPWM)
        WML = PWML = minPWM;
    else
        PWML = WML;

    if(WMR > maxPWM)
    WMR = PWMR = maxPWM;
   else if(WMR < minPWM)
    WMR = PWMR = minPWM;
    else
    PWMR = WMR;
//PWML = WML=50;
//PWMR = WMR=50;
    QByteArray l,r;
    QString ldata = QString(PWML);
    l = ldata.toAscii();
    qDebug()<<WML;
    //ui->pwmL->setText(QString::number(WML));
    ui->pwmL->append(QString::number(WML));

    QString rdata = QString(PWMR);
    r = rdata.toAscii();
    qDebug()<<WMR;
   // ui->pwmR->setText(QString::number(WMR));
    ui->pwmR->append(QString::number(WMR));

    char f =2;
    QByteArray f1;
    QString fdata = QString(f);
    f1 = fdata.toAscii();
    qDebug()<<port->write(f1);
    // port->flush();
  // for(int i=0;i<=10000;i++);
    qDebug()<<port->write(l);
    // port->flush();
 //for(int i=0;i<=10000;i++);
    qDebug()<<port->write(r);
 //for(int i=0;i<=10000;i++);

    l.clear();
    r.clear();
    port->flush();

}
}


