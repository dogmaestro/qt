#ifndef DATABASECONNECTIONDIALOG2_H
#define DATABASECONNECTIONDIALOG2_H

#include <QDialog>

namespace Ui {
class DatabaseConnectionDialog2;
}

class DatabaseConnectionDialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseConnectionDialog2(QWidget *parent = nullptr);
    ~DatabaseConnectionDialog2();

    QString getHostname() const;
    QString getDatabaseName() const;
    QString getLogin() const;
    QString getPassword() const;
    uint getPort() const;

private slots:
    void on_connectButton_clicked();

    void on_cancel_clicked();

private:
    Ui::DatabaseConnectionDialog2 *ui;
};

#endif // DATABASECONNECTIONDIALOG2_H
