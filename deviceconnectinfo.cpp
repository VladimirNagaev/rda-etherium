#include "deviceconnectinfo.h"

DeviceConnectInfo::DeviceConnectInfo(int serverAddress, const QString &serialPort, qint32 baudRate,
                                     QSerialPort::Parity parity, QSerialPort::DataBits dataBits,
                                     QSerialPort::StopBits stopBits)
    : m_serverAddress(serverAddress), m_serialPort(serialPort), m_baudRate(baudRate), m_parity(parity),
      m_dataBits(dataBits), m_stopBits(stopBits)
{}

QSharedPointer<DeviceConnectInfo> DeviceConnectInfo::create(int serverAddress, const QString &serialPort, qint32 baudRate,
                                                            QSerialPort::Parity parity, QSerialPort::DataBits dataBits,
                                                            QSerialPort::StopBits stopBits)
{
    QSharedPointer<DeviceConnectInfo> deviceInfo(
        new DeviceConnectInfo(serverAddress, serialPort, baudRate, parity, dataBits, stopBits));
    return deviceInfo;
}

int DeviceConnectInfo::serverAddress() const
{
    return m_serverAddress;
}

QString DeviceConnectInfo::serialPort() const
{
    return m_serialPort;
}

qint32 DeviceConnectInfo::baudRate() const
{
    return m_baudRate;
}

QSerialPort::Parity DeviceConnectInfo::parity() const
{
    return m_parity;
}

QSerialPort::DataBits DeviceConnectInfo::dataBits() const
{
    return m_dataBits;
}

QSerialPort::StopBits DeviceConnectInfo::stopBits() const
{
    return m_stopBits;
}

void DeviceConnectInfo::setServerAddress(int serverAddress)
{
    m_serverAddress = serverAddress;
}

void DeviceConnectInfo::setSerialPort(const QString &serialPort)
{
    m_serialPort = serialPort;
}

void DeviceConnectInfo::setBaudRate(qint32 baudRate)
{
    m_baudRate = baudRate;
}

void DeviceConnectInfo::setParity(const QSerialPort::Parity &parity)
{
    m_parity = parity;
}

void DeviceConnectInfo::setDataBits(const QSerialPort::DataBits &dataBits)
{
    m_dataBits = dataBits;
}

void DeviceConnectInfo::setStopBits(const QSerialPort::StopBits &stopBits)
{
    m_stopBits = stopBits;
}
