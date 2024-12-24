#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , is_running(false)
    , lapCount(0)
{
    ui->setupUi(this);
    stopwatch = new Stopwatch(this);

    connect(stopwatch, &Stopwatch::timeUpdated, this, &MainWindow::updateTime);
    connect(stopwatch, &Stopwatch::stopped, this, &MainWindow::stop);

    ui->circleButton->setEnabled(false);
    ui->timeLabel->setText("Время: 0.0 сек");
    ui->lapTextBrowser->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startStopButton_clicked()
{
    if (!is_running) {
        stopwatch->start();
        is_running = true;
        ui->startStopButton->setText("Стоп");
        ui->circleButton->setEnabled(true);
    } else {
        stopwatch->stop();
    }
}

void MainWindow::on_circleButton_clicked()
{
    if (is_running) {
        lapCount++;
        double currentLapTime = stopwatch->lapTime();
        QString lapText = QString("Круг %1, время : %2 сек")
                              .arg(lapCount)
                              .arg(QString::number(currentLapTime, 'f', 1));
        ui->lapTextBrowser->append(lapText);
    }
}

void MainWindow::on_clearButton_clicked()
{
    stopwatch->reset();
    ui->startStopButton->setText("Старт");
    ui->circleButton->setEnabled(false);
    is_running = false;
    lapCount = 0;
    ui->lapTextBrowser->clear();
}

void MainWindow::updateTime(double time)
{
    ui->timeLabel->setText("Время: " + QString::number(time, 'f', 1)  + " сек");
}

void MainWindow::stop()
{
    ui->startStopButton->setText("Старт");
    ui->circleButton->setEnabled(false);
    is_running = false;
}
