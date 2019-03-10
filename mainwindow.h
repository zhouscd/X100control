#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QtGlobal>
#include  <QtSerialPort/QSerialPort>

//SUKA!

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE



class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:



private:
    Ui::MainWindow *ui;

    //del?
    SettingsDialog *settings;
    QSerialPort *serial1;
    //
};

#endif // MAINWINDOW_H
