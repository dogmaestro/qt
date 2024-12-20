#include "databaseconnectiondialog2.h"
#include "ui_databaseconnectiondialog2.h"

DatabaseConnectionDialog2::DatabaseConnectionDialog2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DatabaseConnectionDialog2)
{
    ui->setupUi(this);
}

DatabaseConnectionDialog2::~DatabaseConnectionDialog2()
{
    delete ui;
}

void DatabaseConnectionDialog2::on_connectButton_clicked()
{
    this->close();
}

QString DatabaseConnectionDialog2::getHostname() const {
    return ui->hostname->text();
}

QString DatabaseConnectionDialog2::getDatabaseName() const {
    return ui->databaseName->text();
}

QString DatabaseConnectionDialog2::getLogin() const {
    return ui->login->text();
}

QString DatabaseConnectionDialog2::getPassword() const {
    return ui->password->text();
}

uint DatabaseConnectionDialog2::getPort() const {
    return static_cast<uint>(ui->port->value());
}

void DatabaseConnectionDialog2::on_cancel_clicked()
{
     this->close();
}

