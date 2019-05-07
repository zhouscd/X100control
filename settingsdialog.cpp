#include "settingsdialog.h"
#include "ui_settingsdialog.h"

//#include <QVariant>
#include <QMessageBox>
#include <QDebug>

#include <QtSerialPort/QSerialPortInfo>
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

    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->baudRateBox,SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomBaudRatePolicy(int)));

    connect(ui->applyButton,SIGNAL(clicked(bool)),this,SLOT(apply()));

    connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));

    fillPortsParameters();
    fillPortsInfo();
    updateSettings();

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}


SettingsDialog::Settings SettingsDialog::settings() const
{
    return currentSettings;
}


void SettingsDialog::fillPortsParameters()
{
    //Filling QCombo Box for choosing BaudRate
    ui->baudRateBox->addItem("115200",QSerialPort::Baud115200);
    ui->baudRateBox->addItem("9600",QSerialPort::Baud9600);
    ui->baudRateBox->addItem("38400",QSerialPort::Baud38400);
    ui->baudRateBox->addItem("57600",QSerialPort::Baud57600);
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
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem("1.5",QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem("2",QSerialPort::TwoStop);

    //Filling QCombo Box for choosing Flow control
    ui->flowControlBox->addItem("None",QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem("RTS/CTS",QSerialPort::HardwareControl);
    ui->flowControlBox->addItem("XON/XOFF",QSerialPort::SoftwareControl);
}

void SettingsDialog::updateSettings()
{
    currentSettings.name = ui->serialPortInfoListBox->currentText();

    if (ui->baudRateBox->currentIndex() == 4) {
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();

    currentSettings.localEchoEnabled = ui->localEchoCheckBox->isChecked();
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

void SettingsDialog::showPortInfo(int idx)
{
    if (idx != -1) {
        QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
        ui->DescriptionLabel->setText(tr("Description: %1").arg(list.at(1)));
        ui->ManufacturerLabel->setText(tr("Manufacturer: %1").arg(list.at(2)));
        ui->SerialNumberLabel->setText(tr("Serial number: %1").arg(list.at(3)));
        ui->LocationLabel->setText(tr("Location: %1").arg(list.at(4)));
        ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.at(5)));
        ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.at(6)));
    }
}

void SettingsDialog::fillPortsInfo() {

    ui->serialPortInfoListBox->clear();
    static const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    QString serialNumber;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }
}
