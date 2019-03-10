#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

#include "settingsdialog.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//1
    ui->setupUi(this);

    serial1 = new QSerialPort(this);
    settings = new SettingsDialog;

    ui->actionQuit_2->setEnabled(true);

    //

    initActionsConnections();               // design SLOT--> SIGNAL dependencies
    //
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionAbout,SIGNAL(triggered()),this, SLOT(about()));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui->actionQuit_2, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure,SIGNAL(triggered(bool)),settings, SLOT(show()));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About terminal"), tr("This is simple terminal enabling you to "
                                                      "get data from your usb devices. Program "
                                                      "was written using Qt 5.3"));
}

MainWindow::~MainWindow()
{
    delete ui;
	delete settings;
}




