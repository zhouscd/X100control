#include "settingsdialog.h"
#include "ui_settingsdialog.h"

//#include <QVariant>
#include <QMessageBox>
#include <QDebug>

#include <QIntValidator>
#include <QLineEdit>

QT_USE_NAMESPACE

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->localEchoCheckBox->setChecked(true);
    intValidator = new QIntValidator(0,4000000,this);

    connect(ui->baudRateBox,SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomBaudRatePolicy(int)));

    connect(ui->applyButton,SIGNAL(clicked(bool)),this,SLOT(apply()));

    fillPortsParameters();


}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::fillPortsParameters()
{
    //Filling QCombo Box for choosing BaudRate
    ui->baudRateBox->addItem("9600",QSerialPort::Baud9600);
    ui->baudRateBox->addItem("38400",QSerialPort::Baud38400);
    ui->baudRateBox->addItem("57600",QSerialPort::Baud57600);
    ui->baudRateBox->addItem("115200",QSerialPort::Baud115200);
    ui->baudRateBox->addItem("Custom");

    //Filling QCombo Box for choosing Data Bits
    ui->dataBitsBox->addItem("8",QSerialPort::Data8);
    ui->dataBitsBox->addItem("7",QSerialPort::Data7);
    ui->dataBitsBox->addItem("6",QSerialPort::Data6);
    ui->dataBitsBox->addItem("5",QSerialPort::Data5);

    //Filling QCombo Box for choosing Parity
    ui->parityBox->addItem("None",QSerialPort::NoParity);
    ui->parityBox->addItem("Even",QSerialPort::EvenParity);
    ui->parityBox->addItem("Odd",QSerialPort::OddParity);
    ui->parityBox->addItem("Mark",QSerialPort::MarkParity);
    ui->parityBox->addItem("Space",QSerialPort::SpaceParity);

    //Filling QCombo Box for choosing the number of stop bits
    ui->stopBitsBox->addItem("1",QSerialPort::OneStop);
    ui->stopBitsBox->addItem("1.5",QSerialPort::OneAndHalfStop);
    ui->stopBitsBox->addItem("2",QSerialPort::TwoStop);

    //Filling QCombo Box for choosing Flow control
    ui->flowControlBox->addItem("None",QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem("RTS/CTS",QSerialPort::HardwareControl);
    ui->flowControlBox->addItem("XON/XOFF",QSerialPort::SoftwareControl);
}

void SettingsDialog::updateSettings()
{
    currentSettings.name = ui->selectBox->currentText();
    currentSettings.localEchoEnabled = ui->localEchoCheckBox->isChecked();

    currentSettings.stringBaudRate = ui->baudRateBox->currentText();
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();
    currentSettings.stringParity = ui->parityBox->currentText();
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

if (ui->baudRateBox->currentIndex() != 4)
{
    currentSettings.baudRate = (QSerialPort::BaudRate)ui->baudRateBox->itemData(
               ui->baudRateBox->currentIndex()).toInt();
}
else
{
    currentSettings.stringBaudRate = ui->baudRateBox->currentText();
    currentSettings.baudRate = (QSerialPort::BaudRate)ui->baudRateBox->currentText().toInt();
}

   currentSettings.dataBits = (QSerialPort::DataBits)ui->dataBitsBox->itemData(
               ui->dataBitsBox->currentIndex()).toInt();

   currentSettings.flowControl = (QSerialPort::FlowControl)ui->flowControlBox->itemData(
               ui->flowControlBox->currentIndex()).toInt();

   currentSettings.parity = (QSerialPort::Parity)ui->parityBox->itemData(
               ui->parityBox->currentIndex()).toInt();

   currentSettings.stopBits = (QSerialPort::StopBits)ui->stopBitsBox->itemData(
               ui->stopBitsBox->currentIndex()).toInt();

    /*
    QVariant z;
    z =QVariant::fromValue(ui->baudRateBox->currentData(ui->baudRateBox->currentIndex()));
    currentSettings.baudRate = z.value<qint32>();
    */
}

void SettingsDialog::checkCustomBaudRatePolicy(int index)
{
   bool isCustomChosen = !ui->baudRateBox->itemData(index).isValid();
   if (isCustomChosen)
   {
       ui->baudRateBox->setEditable(true);
       ui->baudRateBox->clearEditText();
       QLineEdit *edit = ui->baudRateBox->lineEdit();
       edit->setValidator(intValidator);
       //QMessageBox::information(this,"Ska","Text");
   }
}

void SettingsDialog::apply()
{
    updateSettings();
    hide();
}
