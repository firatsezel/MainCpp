#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

struct ElfObject {
    QString eMagic;
    QString eClass;
    QString eData;
    QString eVersion;
    QString eOSABI;
    QString eAbiVersion;
    QString ePad;
    QString eNident;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    sm = new Simulator(this);
    sm->show();
}

void MainWindow::on_toolButton_clicked()
{
    QString file1Name = QFileDialog::getOpenFileName(this,
             tr("Open Executable"), "/");//dosyayi cek

    ui->textEdit->setText(file1Name);//pathi yazdir

    std::string param = file1Name.toUtf8().constData();//qstring to string
    QString qstr = QString::fromStdString(::readFile(param));//string to qstring

    QString signature,Arch,exeSignature,fileType;//elf ve mac objeleri icin degiskenler
    /*Arch elf dosyari icin temp degiskeni*/
    ElfObject elfheader;//elf header icin obje

    for(int i = 0; i < 8 ; ++i){
       if(i < 4) exeSignature += qstr.at(i);//exe icin signal
       signature += qstr.at(i);//mac ve elf icin signal
    }

    ui->textBrowser->append("Signature: " + signature);

    /*Hangi dosya olduguna dair kontroller. Kosul saglandigi zaman if kontrolununun
icinden devam ediyor parse etmeye. Dosya formatlarinin degerleri icin yukarida struct tanimlayabiliriz. ancak string koydum hepsini
degerlerini tutmuyorumda kullaniciya gosterilecek kismini tutuyorum. BU DEGISTIRILEBILIR.*/

    if(!QString::compare("cffaedfe", signature, Qt::CaseInsensitive)){//Mac 64 kontrolu
        fileType = "macos";
        ui->textBrowser->append("Magic: " + qstr);
        ui->textBrowser->append("File Format: Mach-O binary (64-bit)");
    }else if(!QString::compare("cefaedfe", signature, Qt::CaseInsensitive)){//Mac 32 kontrolu
        fileType = "macos";
        ui->textBrowser->append("Magic: " + qstr);
        ui->textBrowser->append("File Format: Mach-O binary (32-bit)");
    }else if(!QString::compare("464c457f", signature, Qt::CaseInsensitive) ||
             !QString::compare("7f454c46", signature, Qt::CaseInsensitive)){//ELF kontrolu
        fileType = "elf";

        //elf header tanimlanmasi
        elfheader.eMagic = qstr;
        ui->textBrowser->append("File Magic: " + elfheader.eMagic);

        for(int i = 8; i < 10 ; ++i) Arch += qstr.at(i);

        if(!QString::compare("01", Arch, Qt::CaseInsensitive)) elfheader.eClass = "ELF32";
        else if(!QString::compare("02", Arch, Qt::CaseInsensitive))  elfheader.eClass = "ELF64";

        ui->textBrowser->append("File Class: " + elfheader.eClass);

        Arch = "";//yeni veri girmek icin sildik
        for(int i = 10; i < 12 ; ++i) Arch += qstr.at(i);//data icin veri cekimi

        if(!QString::compare("01", Arch, Qt::CaseInsensitive)) elfheader.eData = "Little Endian";
        else if(!QString::compare("02", Arch, Qt::CaseInsensitive)) elfheader.eData = "Big Endian";

        ui->textBrowser->append("File Data: " + elfheader.eData);

        /*VERSION NEDIR ARASTIR 12 - 14 ARASI PARS EDILMEDI*/

        Arch = "";

        for(int i = 14; i < 16 ; ++i) Arch += qstr.at(i);//OSABI kontrolu

        if(!QString::compare("01", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Hewlett-Packard HP-UX";
        else if(!QString::compare("02", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "NetBSD";
        else if(!QString::compare("03", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "GNU";
        else if(!QString::compare("06", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Sun Solaris";
        else if(!QString::compare("07", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "AIX";
        else if(!QString::compare("08", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "IRIX";
        else if(!QString::compare("09", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "FreeBSD";
        else if(!QString::compare("10", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Compaq TRU64 UNIX";
        else if(!QString::compare("11", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Novell Modesto";
        else if(!QString::compare("12", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Open BSD";
        else if(!QString::compare("13", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Open VMS";
        else if(!QString::compare("14", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Hewlett-Packard Non-Stop Kernel";
        else if(!QString::compare("15", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Amiga Research OS";
        else if(!QString::compare("16", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "The Fenix OS";
        else if(!QString::compare("17", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Nuxi CloudABI";
        else if(!QString::compare("18", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "Stratus Technologies OpenVOS";
        else if(!QString::compare("00", Arch, Qt::CaseInsensitive)) elfheader.eOSABI = "unspecified";
        else elfheader.eOSABI = "Architecture-specific value range";

        ui->textBrowser->append("File OSABI: " + elfheader.eOSABI);

    }else if(!QString::compare("4d5a", exeSignature, Qt::CaseInsensitive)){//exe kontrolu
        fileType = "exe";
        ui->textBrowser->append("Magic: " + qstr);
        ui->textBrowser->append("File Format: DOS MZ executable");
    }
}
