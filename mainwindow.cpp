
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include "qbytearray.h"

#include "settingsdialog.h"
#include "console.h"
#include <QtSerialPort/QSerialPort>

#define UART_MSG_LEN_MIN    8
#define UART_MSG_LEN_MAX    500
#define PKG_HEAD_CHAR       0xAA
#define PKG_TAIL_CHAR       0xA5
#define PKG_CHECKSUM_MARK   0xE0



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);

    ui->consoleLayout->addWidget(console);

    serial = new QSerialPort(this);
    settings = new SettingsDialog;

    ui->actionQuit_2->setEnabled(true);

    //

    initActionsConnections();               // design SLOT--> SIGNAL dependencies
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    //connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
}

void MainWindow::initActionsConnections()
{
    ui->actionDisconnect->setEnabled(false);
    ui->actionLED_ON->setEnabled(false);
    ui->actionLED_OFF->setEnabled(false);
    ui->actionStepperMotor0_up->setEnabled(false);
    ui->actionStepperMotor2_up->setEnabled(false);
    ui->actionStepperMotor0_down->setEnabled(false);
    ui->actionStepperMotor2_down->setEnabled(false);
    ui->actionCapture->setEnabled(false);
    ui->actionStop->setEnabled(false);

    connect(ui->actionAbout,SIGNAL(triggered()),this, SLOT(about()));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui->actionQuit_2, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure,SIGNAL(triggered(bool)),settings, SLOT(show()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionLED_ON, SIGNAL(triggered()), this, SLOT(led_on()));
    connect(ui->actionLED_OFF, SIGNAL(triggered()), this, SLOT(led_off()));
    connect(ui->actionStepperMotor0_up, SIGNAL(triggered()), this, SLOT(StepperMotor0_up()));
    connect(ui->actionStepperMotor2_up, SIGNAL(triggered()), this, SLOT(StepperMotor2_up()));
    connect(ui->actionStepperMotor0_down, SIGNAL(triggered()), this, SLOT(StepperMotor0_down()));
    connect(ui->actionStepperMotor2_down, SIGNAL(triggered()), this, SLOT(StepperMotor2_down()));
    connect(ui->actionCapture, SIGNAL(triggered()), this, SLOT(Capture()));
    connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(Stop()));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About terminal"), tr("This is simple terminal enabling you to "
                                                      "get data from your usb devices. Program "
                                                      "was written using Qt 5.3"));
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        g_isSerialOpen = true;
        //console->setEnabled(true);
        //console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        ui->actionLED_ON->setEnabled(true);
        ui->actionLED_OFF->setEnabled(true);
        ui->actionStepperMotor0_up->setEnabled(true);
        ui->actionStepperMotor2_up->setEnabled(true);
        ui->actionStepperMotor0_down->setEnabled(true);
        ui->actionStepperMotor2_down->setEnabled(true);
        ui->actionCapture->setEnabled(true);
        ui->actionStop->setEnabled(true);
        ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                   .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                   .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort() {
    g_isSerialOpen = false;
    serial->close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->actionLED_ON->setEnabled(false);
    ui->actionLED_OFF->setEnabled(false);
    ui->actionStepperMotor0_up->setEnabled(false);
    ui->actionStepperMotor2_up->setEnabled(false);
    ui->actionStepperMotor0_down->setEnabled(false);
    ui->actionStepperMotor2_down->setEnabled(false);
    ui->actionCapture->setEnabled(false);
    ui->actionStop->setEnabled(false);
    ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();

    console->putData(data);
    console->putData("\n");
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
	delete settings;
}

void MainWindow::msg_send(QByteArray &data)
{
    char checksum = 0;
    char mark;
    for (int i = 0; i < data.size(); i++) {
            checksum += data.at(i);
    }
    data.prepend(PKG_HEAD_CHAR & 0xff);
    mark = ((checksum & 0xff) >> 4) | 0xE0;
    data.append(mark);
    mark = ((checksum & 0x0f) | 0xE0);
    data.append(mark);
    data.append(PKG_TAIL_CHAR & 0xff);
    serial->write(data);
}

void MainWindow::led_on()
{
    qDebug("led_on.\n");
    QByteArray data("{\"cmd\":1,\"ctl\":1,\"on\":1}");
    console->putData("msg_send:");
    console->putData(data);
    console->putData("\n");
    msg_send(data);
}

void MainWindow::led_off()
{
    qDebug("led_off.\n");
    QByteArray data("{\"cmd\":1,\"ctl\":1,\"on\":0}");
    console->putData("msg_send:");
    console->putData(data);
    console->putData("\n");
    msg_send(data);
}

void MainWindow::StepperMotor0_up()
{
    QByteArray data("{\"cmd\":2,\"id\":0,\"step\":50,\"dir\":1,\"freq\":300}");
    console->putData("msg_send:");
    console->putData(data);
    console->putData("\n");
    msg_send(data);
}
void MainWindow::StepperMotor0_down()
{
    QByteArray data("{\"cmd\":2,\"id\":0,\"step\":50,\"dir\":0,\"freq\":300}");
    console->putData("msg_send:");
    console->putData(data);
    console->putData("\n");
    msg_send(data);
}

void MainWindow::StepperMotor2_up()
{
    QByteArray data("{\"cmd\":2,\"id\":2,\"step\":50,\"dir\":1,\"freq\":300}");
    console->putData("msg_send:");
    console->putData(data);
    console->putData("\n");
    msg_send(data);
}
void MainWindow::StepperMotor2_down()
{
    QByteArray data("{\"cmd\":2,\"id\":2,\"step\":50,\"dir\":0,\"freq\":300}");
    console->putData("msg_send:");
    console->putData(data);
    console->putData("\n");
    msg_send(data);
}
void MainWindow::Capture()
{
    qDebug("Capture.\n");
    QByteArray data("{\"cmd\":5,\"oid\":0,\"cid\":0,\"imgs\":[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20],\"num\":21,\"loop\":-1,\"setp\":-1}");
    console->putData("msg_send:");
    console->putData(data);
    console->putData("\n");
    msg_send(data);
}
void MainWindow::Stop()
{
    qDebug("Stop.\n");
    QByteArray data("{\"cmd\":5,\"loop\":0}");
    console->putData("msg_send:");
    console->putData(data);
    console->putData("\n");
    msg_send(data);
}

void MainWindow::on_Capture_cmd_clicked()
{
    qDebug("on_Capture_cmd_clicked.\n");
    if(g_isSerialOpen != true)
        return;
    Stop();
    QByteArray data("{\"cmd\":5,\"oid\":0,\"cid\":0,\"imgs\":[");
    data.append(ui->imgs_Edit->text());
    data.append("],\"num\":");
    data.append(ui->num_spinBox->text());
    data.append(",\"loop\":");
    data.append(ui->loop_spinBox->text());
    data.append(",\"step\":");
    data.append(ui->step_spinBox->text());
    data.append("}");
    console->putData("msg_send:");
    console->putData(data);
    console->putData("\n");
    msg_send(data);
}
