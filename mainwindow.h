#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

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

public slots:
    void on_box_available_ports_activated(int index);

    void on_baudRateCb_activated(int index);

    void on_u9_Cheskbox_stateChanged(int arg1);

    void on_u8_Cheskbox_stateChanged(int arg1);
    void b_cleaner();
    void tryToConnect();
    void comRenewer();

private slots:

    void on_baudRateCb_currentIndexChanged(int index);

    void on_box_available_ports_currentIndexChanged(int index);

private:
};
#endif // MAINWINDOW_H
