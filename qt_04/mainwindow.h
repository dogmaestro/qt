#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>
#include <QProgressBar>

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

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QRadioButton *rb_1;
    QRadioButton *rb_2;
    QComboBox *comboBox;
    QPushButton *button;
    QProgressBar *progressBar;

    int progressValue;
 };
#endif // MAINWINDOW_H
