#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);

    connect(udpWorker, &UDPworker::sig_sendStringToGUI, this, &MainWindow::DisplayUserData);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);
        outStr << DataType::TimeData << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

    connect(ui->pb_sendDatagram, &QPushButton::clicked, this, &MainWindow::on_pb_sendDatagram_clicked);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}

void MainWindow::on_pb_sendDatagram_clicked()
{
    QString text = ui->le_input->text();
    if (!text.isEmpty()) {
        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);
        outStr << DataType::UserData << text;
        udpWorker->SendDatagram(dataToSend);
        ui->le_input->clear();
    }
}

void MainWindow::DisplayUserData(const QHostAddress &sender, int dataSize/*, const QString &data*/)
{
    ui->te_result->append("Принято сообщение от " + sender.toString() +
                            ", размер сообщения(байт) " + QString::number(dataSize) + ";");
}

