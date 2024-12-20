#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "databaseconnectiondialog2.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    DatabaseConnectionDialog2 dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString hostname = dialog.getHostname();
        QString dbName = dialog.getDatabaseName();
        QString login = dialog.getLogin();
        QString password = dialog.getPassword();
        quint16 port = dialog.getPort();

        QString info = QString("Имя хоста: %1\nИмя БД: %2\nЛогин: %3\nПароль: %4\nПорт: %5")
                           .arg(hostname)
                           .arg(dbName)
                           .arg(login)
                           .arg(password)
                           .arg(port);
        QMessageBox::information(this, "Информация по соединению", info);
    }
}

