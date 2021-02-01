#ifndef DEVICECONNECTINFO_H
#define DEVICECONNECTINFO_H

//#include <QSerialPort>
#include <QSharedPointer>
#include <QString>
#include <QtSerialPort/QSerialPort>

class DeviceConnectInfo
{
public:
    static QSharedPointer<DeviceConnectInfo> create(int serverAddress, const QString &serialPort,
                                                    qint32 baudRate = QSerialPort::Baud115200,
                                                    QSerialPort::Parity parity = QSerialPort::EvenParity,
                                                    QSerialPort::DataBits dataBits = QSerialPort::Data8,
                                                    QSerialPort::StopBits stopBits = QSerialPort::OneStop);

    int serverAddress() const;
    QString serialPort() const;
    qint32 baudRate() const;
    QSerialPort::Parity parity() const;
    QSerialPort::DataBits dataBits() const;
    QSerialPort::StopBits stopBits() const;

    void setServerAddress(int serverAddress);
    void setSerialPort(const QString &serialPort);
    void setBaudRate(qint32 baudRate);
    void setParity(const QSerialPort::Parity &parity);
    void setDataBits(const QSerialPort::DataBits &dataBits);
    void setStopBits(const QSerialPort::StopBits &stopBits);

private:
    DeviceConnectInfo(int serverAddress, const QString &serialPort, qint32 baudRate, QSerialPort::Parity parity,
                      QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits);

    int m_serverAddress = 1;

    QString m_serialPort;
    qint32 m_baudRate = QSerialPort::Baud115200;
    QSerialPort::Parity m_parity = QSerialPort::EvenParity;
    QSerialPort::DataBits m_dataBits = QSerialPort::Data8;
    QSerialPort::StopBits m_stopBits = QSerialPort::OneStop;
};

#endif // DEVICECONNECTINFO_H
