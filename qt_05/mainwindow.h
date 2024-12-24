#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"

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

private slots:
    void on_startStopButton_clicked();
    void on_circleButton_clicked();
    void on_clearButton_clicked();
    void updateTime(double);
    void stop();

private:
    Ui::MainWindow *ui;
    Stopwatch *stopwatch;
    bool is_running;
    int lapCount;
};
#endif // MAINWINDOW_H
