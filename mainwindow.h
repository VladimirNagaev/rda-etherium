#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QDebug>
#include <QMainWindow>
#include <QModbusDataUnit>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>

class QModbusClient;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isU8;
    bool isU9;
    int baudRate = -1;
    QString comPort;
    QSerialPort current_serial;
    Ui::MainWindow *ui;

    uint16_t crc(uint8_t *data, uint8_t size);
    uint16_t arrayCRC(QByteArray data);

    int getAdressFromReply(QByteArray vector);
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

public slots:
    void on_box_available_ports_activated(int index);

    void on_baudRateCb_activated(int index);

    void on_u9_Cheskbox_stateChanged(int arg1);

    void on_u8_Cheskbox_stateChanged(int arg1);
    void b_cleaner();
    void tryToConnect();
    void comRenewer();
    QByteArray searchSequence(quint8 firstByte);
    QByteArray setAddresSequence(quint8 adress);
    QByteArray setNetMaskSequence(quint8 netMask);
    //uint16_t crc(uint8_t *data, uint8_t size);
    // uint16_t arrayCRC(QVector<uint8_t> data);

private slots:

    void on_baudRateCb_currentIndexChanged(int index);

    void on_box_available_ports_currentIndexChanged(int index);

    void on_checkParamButton_clicked();

    void on_NetMaskSetButton_clicked();

    void on_AdressSetButton_clicked();

    void onStateChanged(int state);
    void readReady();

private:
    QModbusClient *modbusDevice;
    void init();

    const int SERVER_ADDRESS = 1;
    const int START_ADDRESS = 0;
    const int AMOUNT = 10;
    const int WRITE_SIZE = 1;

    const int BAUDS = 1500000;
    const int STOP_BITS = 1;
    const int DATA_BITS = 8;
    const QString PORT = "COM16";
    const int PARITY = QSerialPort::EvenParity;
    const int RESPONSE_TIME = 100;
};
#endif // MAINWINDOW_H
