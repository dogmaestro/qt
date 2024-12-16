#include "mainwindow.h"
#include "./ui_mainwindow.h"

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

QString MainWindow::getHostname() const {
    return ui->hostname->text();
}

QString MainWindow::getDatabaseName() const {
    return ui->databaseName->text();
}

QString MainWindow::getLogin() const {
    return ui->login->text();
}

QString MainWindow::getPassword() const {
    return ui->password->text();
}

uint MainWindow::getPort() const {
    return static_cast<uint>(ui->port->value());
}

void MainWindow::on_connectButton_clicked() {
    this->close();
}

void MainWindow::on_cancel_clicked()
{
    this->close();
}

