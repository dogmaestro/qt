#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    QString getHostname() const;
    QString getDatabaseName() const;
    QString getLogin() const;
    QString getPassword() const;
    uint getPort() const;

private slots:
    void on_connectButton_clicked();

    void on_cancel_clicked();

private:
    Ui::MainWindow *ui;
};
#endif
