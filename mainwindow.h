#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QtGlobal>
#include  <QtSerialPort/QSerialPort>
#include <stdio.h>
#include <string.h>
QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE


class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void led_on();
    void led_off();
    void StepperMotor0_up();
    void StepperMotor2_up();
    void StepperMotor0_down();
    void StepperMotor2_down();
    void Capture();
    void Stop();
    void msg_send(QByteArray &data);
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);

    void on_Capture_cmd_clicked();

private:
    void initActionsConnections();

private:
    Ui::MainWindow *ui;
    Console *console;
    SettingsDialog *settings;
    QSerialPort *serial;
    bool g_isSerialOpen = false;
};

#endif // MAINWINDOW_H
