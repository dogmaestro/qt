#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , progressValue(0)
{
    ui->setupUi(this);

    rb_1 = findChild<QRadioButton*>("rb_1");
    rb_2 = findChild<QRadioButton*>("rb_2");
    comboBox = findChild<QComboBox*>("comboBox");
    button = findChild<QPushButton*>("pushButton");
    progressBar = findChild<QProgressBar*>("progressBar");

    if (rb_1 && rb_2) {
        rb_1->setText("Вариант 1");
        rb_2->setText("Вариант 2");
    }

    if (comboBox) {
        QStringList items = {"Пункт 1", "Пункт 2", "Пункт 3", "Пункт 4", "Пункт 5"};
        comboBox->addItems(items);
    }

    if (button) {
        button->setText("Начало работы");
        button->setCheckable(true);
    }

    if (progressBar) {
        progressBar->setMinimum(0);
        progressBar->setMaximum(10);
        progressBar->setValue(progressValue);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    progressValue += 1;

    if (progressValue > 10) {
        progressValue = 0;
    }

    if (progressBar) {
        progressBar->setValue(progressValue);
    }
}

