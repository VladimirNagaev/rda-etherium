#include "comportworker.h"

#include <QSerialPortInfo>
//#include "mainwindow.h"
#include "mainwindow.ui"

#include <QTime>

COMPortWorker::COMPortWorker()
{}
QSerialPort current_serial = new QSerialPort();

void COMPortWorker::addActiveComPorts()
{
    foreach (QSerialPortInfo serial_info, QSerialPortInfo::availablePorts()) {
        box_available_ports->addItem(serial_info.portName(), serial_info.portName());
    }
}

void COMPortWorker::connectToPort(DeviceConnectInfo devicrconnectinfo)
{
    current_serial->setPortName(box_available_ports->currentText());

    if (current_serial->setBaudRate(QSerialPort::Baud9600) && current_serial->setParity(QSerialPort::NoParity)
        && current_serial->setDataBits(QSerialPort::Data8) && current_serial->setStopBits(QSerialPort::OneStop)
        && current_serial->setFlowControl(QSerialPort::HardwareControl)) {
        if (current_serial->open(QIODevice::ReadWrite) == true) {
            if (current_serial->isOpen()) {
                status_port->setText(current_serial->portName() + " - port online...");
                information_received_from_port->insertPlainText(QTime::currentTime().toString() + " >> "
                                                                + current_serial->portName() + " - opened!" + "\n");
                read_curr_serial->start();
            } else {
                current_serial->close();
                information_received_from_port->insertPlainText(QTime::currentTime().toString() + " >> "
                                                                + "ERROR: " + current_serial->portName() + ": "
                                                                + current_serial->errorString().toLocal8Bit());
            }
        } else {
            current_serial->close();
            information_received_from_port->insertPlainText(QTime::currentTime().toString() + " >> "
                                                            + "ERROR: " + current_serial->portName() + ": "
                                                            + current_serial->errorString().toLocal8Bit());
        }
    }
}

void COMPortWorker::ok_write_toPort(QByteArray command)
{
    command = command_toPort->text().toLocal8Bit();

    if (current_serial->isOpen()) {
        current_serial->write(command);
        information_received_from_port->insertPlainText(QTime::currentTime().toString() + " >> " + "WRITE: " + command
                                                        + "\n");
    } else {
        information_received_from_port->insertPlainText(QTime::currentTime().toString() + " >> "
                                                        + "ERROR: " + current_serial->errorString().toLocal8Bit());
    }
}

// crc

uint16_t crc(uint8_t *data, uint8_t size)
{
    uint16_t pos;
    uint8_t i;
    uint16_t crc = 0xFFFF;

    for (pos = 0; pos < size; pos++) {
        crc ^= (uint16_t)data[pos];

        for (uint8_t i = 8; i != 0; i--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else
                crc >>= 1;
        }
    }
    return crc;
}

void convert(const char *src, uint8_t **dst, size_t *dst_size)
{
    size_t i;
    *dst_size = strlen(src) / 2;
    *dst = (uint8_t *)malloc(*dst_size);

    for (i = 0; i < *dst_size; i++) {
        sscanf(src + i * 2, "%02x", *dst + i);
    }
}

void print_crc(const char *data)
{
    uint8_t *value;
    size_t size;
    size_t i;
    uint16_t val_crc;

    convert(data, &value, &size);

    for (i = 0; i < size; i++)
        printf("%02X", value[i]);
    printf("\n");

    val_crc = crc(value, size);
    printf("0x%04X\n", val_crc);

    free(value);
}
