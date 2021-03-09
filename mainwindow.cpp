#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_box_available_ports_activated(int index)
{
    foreach (QSerialPortInfo serial_info, QSerialPortInfo::availablePorts()) {
        ui->box_available_ports->addItem(serial_info.portName(), serial_info.portName());
    }
}

void MainWindow::on_baudRateCb_activated(int index)
{
    ui->baudRateCb->addItem("115200", "115200");
    ui->baudRateCb->addItem("128000", "128000");
    ui->baudRateCb->addItem("256000", "256000");
    ui->baudRateCb->addItem("460800", "460800");
    ui->baudRateCb->addItem("1500000", "1500000");
    ui->baudRateCb->addItem("custom", "");
}
