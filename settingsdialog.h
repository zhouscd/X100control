#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSerialPort>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class SettingsDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
        Settings settings() const;

private slots:
    void checkCustomBaudRatePolicy(int index);
    void apply();
    void showPortInfo(int idx);


    void on_pushButton_clicked();

private:
    void fillPortsParameters();
    void updateSettings();
    void fillPortsInfo();

private:
    Ui::SettingsDialog *ui;
    Settings currentSettings;
    QIntValidator *intValidator;
};

#endif // SETTINGSDIALOG_H
