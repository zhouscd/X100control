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

    //
    connect(ui->actionConfigure,SIGNAL(triggered(bool)),
            settings, SLOT(show()));
    //
}

MainWindow::~MainWindow()
{
    delete ui;
	delete settings;
}




