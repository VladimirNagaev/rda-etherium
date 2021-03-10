#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->baudRateCb->addItem("1500000", "1500000");
    ui->baudRateCb->addItem("115200", "115200");
    ui->baudRateCb->addItem("460800", "460800");
    ui->baudRateCb->addItem("custom", "");
    baudRate = 1500000;
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
void MainWindow::comRenewer()
{
    int temp1 = ui->box_available_ports->count();
    int temp2 = QSerialPortInfo::availablePorts().size();
    int timeMsec = QTime::currentTime().msec() % 100;

    if (QTime::currentTime().msec() % 100 == 0
        && ui->box_available_ports->count() != QSerialPortInfo::availablePorts().size()) {
        foreach (QSerialPortInfo serial_info, QSerialPortInfo::availablePorts()) {
            ui->box_available_ports->addItem(serial_info.portName(), serial_info.portName());
        }
    }
}
void MainWindow::on_box_available_ports_activated(int index)
{
    int temp1 = ui->box_available_ports->count();
    int temp2 = QSerialPortInfo::availablePorts().size();
    int timeMsec = QTime::currentTime().msec() % 100;
    foreach (QSerialPortInfo serial_info, QSerialPortInfo::availablePorts()) {
        ui->box_available_ports->addItem(serial_info.portName(), serial_info.portName());
    }
    comPort = ui->box_available_ports->currentText();
    ui->stateLable->setText(temp1 + " strings   ports " + temp2);
}

QString fromBool(bool bol)
{
    if (bol) {
        return "true";
    } else {
        return "false";
    }
}
void MainWindow::on_baudRateCb_activated(int index)
{}

void MainWindow::b_cleaner()
{
    ui->baudRateCb->clear();
}

void MainWindow::on_u9_Cheskbox_stateChanged(int arg1)
{
    if (ui->u8_Cheskbox->isChecked()) {
        ui->u8_Cheskbox->setCheckState(Qt::CheckState(0));
    }
    isU8 = ui->u9_Cheskbox->isChecked();
    isU9 = ui->u8_Cheskbox->isChecked();
    QString state = "Не выбран тип преобразователя";
    if (isU8 && !isU9) {
        state = "Выбран преобразователь серии U8";
    } else if (isU9 && !isU8) {
        state = "Выбран преобразователь серии U8";
    }

    ui->stateLable->setText(state);
}

void MainWindow::on_u8_Cheskbox_stateChanged(int arg1)
{
    if (ui->u9_Cheskbox->isChecked()) {
        ui->u9_Cheskbox->setCheckState(Qt::CheckState(0));
    }
    isU9 = ui->u9_Cheskbox->isChecked();
    isU8 = ui->u8_Cheskbox->isChecked();
    QString state = "Не выбран тип преобразователя";
    if (isU8 && !isU9) {
        state = "Выбран преобразователь серии U8";
    } else if (isU9 && !isU8) {
        state = "Выбран преобразователь серии U8";
    }

    ui->stateLable->setText(state);
}

void MainWindow::on_baudRateCb_currentIndexChanged(int index)
{
    baudRate = ui->baudRateCb->currentText().toInt();

    QString state = "Baud rate ";
    state.append(ui->baudRateCb->currentText());
    ui->stateLable->setText(state);
}
void MainWindow::tryToConnect()
{
    current_serial.setPortName(ui->box_available_ports->currentText());
    if ((isU8 || isU9) && baudRate != -1) {
        if (current_serial.setBaudRate(baudRate) && current_serial.setParity(QSerialPort::EvenParity)
            && current_serial.setDataBits(QSerialPort::Data8) && current_serial.setStopBits(QSerialPort::OneStop)
            && current_serial.setFlowControl(QSerialPort::HardwareControl)) {
            if (current_serial.open(QIODevice::ReadWrite) == true) {
                if (current_serial.isOpen()) {
                    QString string = current_serial.portName() + " - port online...";
                    string.append(QTime::currentTime().toString() + " >> " + current_serial.portName() + " - opened!");
                    ui->stateLable->setText(string);

                    // read_curr_serial->start();
                } else {
                    current_serial.close();
                    QString string = QTime::currentTime().toString() + " >> " + "ERROR: " + current_serial.portName()
                                     + ": " + current_serial.errorString().toLocal8Bit();
                    ui->stateLable->setText(string);
                }
            } else {
                QString string = QTime::currentTime().toString() + " >> " + "ERROR: " + current_serial.portName() + ": "
                                 + current_serial.errorString().toLocal8Bit();
                ui->stateLable->setText(string);
            }
        }
    } else {
        ui->stateLable->setText("<font color:red>Чего-то не хватает!</font>");
    }
}

void MainWindow::on_box_available_ports_currentIndexChanged(int index)
{
    tryToConnect();
}
