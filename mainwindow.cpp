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

uint16_t MainWindow::crc(uint8_t *data, uint8_t size)
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
uint16_t MainWindow::arrayCRC(QByteArray data)
{
    uint16_t pos;
    uint8_t i;
    uint16_t crc = 0xFFFF;

    for (pos = 0; pos < data.size(); pos++) {
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
    int timeMsec = QTime::currentTime().msec() % 100;

    if (QTime::currentTime().msec() - timeMsec > 1000) {
        ui->box_available_ports->clear();
        foreach (QSerialPortInfo serial_info, QSerialPortInfo::availablePorts()) {
            ui->box_available_ports->addItem(serial_info.portName(), serial_info.portName());
        }
    }
}
void MainWindow::on_box_available_ports_activated(int index)
{
    if (QSerialPortInfo::availablePorts().size() > ui->box_available_ports->count()) {
        ui->box_available_ports->clear();
        foreach (QSerialPortInfo serial_info, QSerialPortInfo::availablePorts()) {
            ui->box_available_ports->addItem(serial_info.portName(), serial_info.portName());
        }
    }
    comPort = ui->box_available_ports->currentText();
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

    if (QSerialPortInfo::availablePorts().size() > ui->box_available_ports->count()) {
        ui->box_available_ports->clear();
        foreach (QSerialPortInfo serial_info, QSerialPortInfo::availablePorts()) {
            ui->box_available_ports->addItem(serial_info.portName(), serial_info.portName());
        }
    }
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

    if (QSerialPortInfo::availablePorts().size() > ui->box_available_ports->count()) {
        ui->box_available_ports->clear();
        foreach (QSerialPortInfo serial_info, QSerialPortInfo::availablePorts()) {
            ui->box_available_ports->addItem(serial_info.portName(), serial_info.portName());
        }
    }
}

void MainWindow::on_baudRateCb_currentIndexChanged(int index)
{
    baudRate = ui->baudRateCb->currentText().toInt();

    QString state = "Baud rate ";
    state.append(ui->baudRateCb->currentText());
    ui->stateLable->setText(state);
}
void MainWindow::tryToConnect() //  подключаемся к порту
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
//  Вот туе начинается засылание последовательностей

QByteArray MainWindow::searchSequence(quint8 firstByte) // формирование команды поиска
{
    QByteArray searchSequence;
    quint8 loByte;
    quint8 hiByte;

    searchSequence.push_back(char(firstByte));
    searchSequence.push_back(0x03);
    searchSequence.push_back(char(0x00));
    searchSequence.push_back(0x01);
    searchSequence.push_back(char(0x00));
    searchSequence.push_back(0x01);

    uint16_t crcSS = arrayCRC(searchSequence);
    loByte = crcSS & 0xFF;
    hiByte = crcSS >> 8;

    searchSequence.push_back(loByte);
    searchSequence.push_back((unsigned char)hiByte);

    return searchSequence;
}

QByteArray MainWindow::setAddresSequence(quint8 adress) // формирование команды изменения адреса
{
    QByteArray addresSequence;

    quint8 loByte;
    quint8 hiByte;

    addresSequence.push_back(char(0x00));
    addresSequence.push_back(0x10);
    addresSequence.push_back(char(0x00));
    addresSequence.push_back(char(0x00));
    addresSequence.push_back(char(0x00));
    addresSequence.push_back(0x01);
    addresSequence.push_back(0x02);
    addresSequence.push_back(char(0x00));
    addresSequence.push_back(adress);

    uint16_t crcSS = arrayCRC(addresSequence);
    loByte = crcSS & 255;
    hiByte = crcSS >> 8;

    addresSequence.push_back((char)loByte);
    addresSequence.push_back((char)hiByte);

    return addresSequence;
}

QByteArray MainWindow::setNetMaskSequence(quint8 netMask) // формирование команды изменения маски сети
{
    QByteArray netMaskSequence;

    quint8 loByte;
    quint8 hiByte;

    netMaskSequence.push_back(char(0x00));
    netMaskSequence.push_back(0x10);
    netMaskSequence.push_back(char(0x00));
    netMaskSequence.push_back(0x03);
    netMaskSequence.push_back(char(0x00));
    netMaskSequence.push_back(0x01);
    netMaskSequence.push_back(0x02);
    netMaskSequence.push_back(char(0x00));
    netMaskSequence.push_back((char)netMask);

    uint16_t crcSS = arrayCRC(netMaskSequence);
    loByte = crcSS & 0xFF;
    hiByte = crcSS >> 8;

    netMaskSequence.push_back((char)loByte);
    netMaskSequence.push_back((char)hiByte);

    return netMaskSequence;
}

int MainWindow::getAdressFromReply(QByteArray vector)
{
    int address = vector.at(1);
    ui->spinBox->setValue(address);
    ui->stateLable->setText(&"Прибор РДА адрес "[address]);
    return address;
}
QString byteArrToStr(QByteArray vector)
{
    QString string;
    string.append("|");
    for (int i = 0; i < vector.size(); i++) {
        QString temp;
        temp.setNum(vector.at(i), 16).toUpper();
        string.append(temp);
        string.append(" |");
    }

    string.append("\n");

    return string;
}

void MainWindow::on_checkParamButton_clicked() // кнопочка проверки параметров
{
    QString string;
    string = ui->stateLable->text();
    string.append("\n");
    for (quint8 i = 1; i < 8; i++) {
        string.append(QString::number(sizeof(searchSequence(i)), 10));
        string.append(" -  ");
        string.append(byteArrToStr(searchSequence(i)));
        ui->stateLable->setText(string);
        current_serial.write(searchSequence(i), searchSequence(i).length());
        QByteArray reply = current_serial.readAll();
        current_serial.flush();
        if (reply.size() > 6) {
            getAdressFromReply(reply);
            ui->stateLable->setText(byteArrToStr(reply));
            // break;
        }
    }
    // current_serial.close();
}

void MainWindow::on_NetMaskSetButton_clicked() // изменение маски подсети
{
    quint8 addr = (quint8)ui->spinBox->value(); // забираем маску из поля для ввода
    if (addr != 0) {
        current_serial.write(setAddresSequence((quint8)addr), setNetMaskSequence((quint8)addr).length()); // кидаем посылку
        ui->stateLable->setText(byteArrToStr(setNetMaskSequence((quint8)addr))); // повторяем втроку технической информации
        current_serial.flush(); // очищаем буфер порта
    } else {
        ui->stateLable->setText("Кажется Маска (сеть) не может быть 0 ");
    }
}

void MainWindow::on_AdressSetButton_clicked() // изменение адреа прибора
{
    quint8 addr = (quint8)ui->spinBox_2->value(); // забираем адрес из поля для ввода
    if (addr != 0) {
        current_serial.write(setAddresSequence((quint8)addr), setAddresSequence((quint8)addr).length()); // кидаем посылку
        ui->stateLable->setText(byteArrToStr(setAddresSequence((quint8)addr))); // повторяем втроку технической информации
        current_serial.flush(); // очищаем буфер порта
    } else {
        ui->stateLable->setText("Кажется Адрес не может быть 0 ");
    }
}

/*
                                                                        БОЛЬШАЯ, ЗЛЮЩАЯ УБЕР_ЁБЬ  НИЖЕ
  */

void MainWindow::init()
{
    modbusDevice = new QModbusRtuSerialMaster(this);
    connect(modbusDevice, &QModbusClient::errorOccurred,
            [this](QModbusDevice::Error) { statusBar()->showMessage(modbusDevice->errorString(), 5000); });

    if (modbusDevice) {
        connect(modbusDevice, &QModbusClient::stateChanged, this, &MainWindow::onStateChanged);
    }
}

void MainWindow::onStateChanged(int state)
{
    if (state == QModbusDevice::UnconnectedState)
        ui->checkParamButton->setText(tr("Проверить параметры"));
    else if (state == QModbusDevice::ConnectedState)
        ui->checkParamButton->setText(tr("Disconnect"));
}

void MainWindow::on_readButton_clicked()
{
    if (!modbusDevice)
        return;
    // ui->readValue->clear();
    statusBar()->clearMessage();

    if (auto *lastRequest = modbusDevice->sendReadRequest(readRequest(), SERVER_ADDRESS)) {
        if (!lastRequest->isFinished())
            connect(lastRequest, &QModbusReply::finished, this, &MainWindow::readReady);
        else
            delete lastRequest; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}

void MainWindow::readReady()
{
    auto lastRequest = qobject_cast<QModbusReply *>(sender());
    if (!lastRequest)
        return;

    if (lastRequest->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = lastRequest->result();
        for (uint i = 0; i < unit.valueCount(); i++) {
            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress()).arg(QString::number(unit.value(i)));
            //    ui->readValue->addItem(entry);
        }
    } else if (lastRequest->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)")
                                     .arg(lastRequest->errorString())
                                     .arg(lastRequest->rawResult().exceptionCode(), -1, 16),
                                 5000);
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)")
                                     .arg(lastRequest->errorString())
                                     .arg(lastRequest->error(), -1, 16),
                                 5000);
    }

    lastRequest->deleteLater();
}

void MainWindow::on_writeButton_clicked()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();
    // writeUnit.setValue(0, ui->writeBox->value());

    if (auto *lastRequest = modbusDevice->sendWriteRequest(writeUnit, SERVER_ADDRESS)) {
        if (!lastRequest->isFinished()) {
            connect(lastRequest, &QModbusReply::finished, this, [this, lastRequest]() {
                if (lastRequest->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                                 .arg(lastRequest->errorString())
                                                 .arg(lastRequest->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (lastRequest->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)")
                                                 .arg(lastRequest->errorString())
                                                 .arg(lastRequest->error(), -1, 16),
                                             5000);
                }
                lastRequest->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            lastRequest->deleteLater();
        }
    } else {
        statusBar()->showMessage(tr("Write error: ") + modbusDevice->errorString(), 5000);
    }
}

QModbusDataUnit MainWindow::readRequest() const
{
    return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, START_ADDRESS, AMOUNT);
}

QModbusDataUnit MainWindow::writeRequest() const
{
    return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, START_ADDRESS, WRITE_SIZE);
}

void MainWindow::on_connectButton_clicked()
{
    if (!modbusDevice)
        return;

    statusBar()->clearMessage();
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, PORT);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, PARITY);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, BAUDS);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, DATA_BITS);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, STOP_BITS);
        modbusDevice->setTimeout(RESPONSE_TIME);
        modbusDevice->setNumberOfRetries(1);
        if (!modbusDevice->connectDevice()) {
            statusBar()->showMessage(tr("Connect failed: ") + modbusDevice->errorString(), 5000);
        }
    } else {
        modbusDevice->disconnectDevice();
    }
}

/*
                                                                                БОЛЬШАЯ, ЗЛЮЩАЯ УБЕР_ЁБЬ  ВЫШЕ
  */
