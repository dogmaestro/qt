#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "airportstatswindow.h"
#include <QMessageBox>
#include <QTimer>
#include <qsqlrecord.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(new Database(this))
    , timer(new Timer(this))
{
    ui->setupUi(this);
    m_isWindowClosed = false;

    setupDatabase();
    setupUI();
    setupConnections();
    db->ConnectToDB();
}

MainWindow::~MainWindow()
{
    delete ui;
    db->DisconnectFromDataBase();
    delete db;
    delete statsWindow;
}

void MainWindow::setupDatabase()
{
    db->AddDataBase("QPSQL", "AirportsDB");
    db->ConnectToDB();
}

void MainWindow::setupUI()
{
    ui->pb_fetch_flights->setEnabled(false);
    ui->pb_show_stats->setEnabled(false);
    ui->rb_departure->setChecked(true);
    ui->lb_status->setStyleSheet("color: red;");
}

void MainWindow::setupConnections()
{
    connect(db, &Database::sig_SendStatusConnection,
            this, &MainWindow::handleConnectionStatus);
    connect(db, &Database::sig_SendAirports,
            this, &MainWindow::handleAirportsData);
    connect(db, &Database::sig_SendArrivals,
            this, &MainWindow::handleArrivalsData);
    connect(db, &Database::sig_SendDepartures,
            this, &MainWindow::handleDeparturesData);
    connect(db, &Database::sig_SendStatusRequest,
            this, &MainWindow::handleDBError);
    connect(timer, &Timer::sig_Reconnect,
            this, &MainWindow::ConnectToDB);
    connect(this, &MainWindow::sig_StartTimer,
            timer, &Timer::StartTimer);
}

void MainWindow::handleConnectionStatus(bool status)
{
    if(m_isWindowClosed) return;

    ui->lb_status->setText(status ? "Подключено" : "Отключено");
    ui->lb_status->setStyleSheet(status ? "color: green;" : "color: red;");

    ui->pb_fetch_flights->setEnabled(status);
    ui->pb_show_stats->setEnabled(status);

    if(status) {
        db->GetAirports();
        if (statsWindow != nullptr) {
            statsWindow = new AirportStatsWindow(ui->cb_airports->currentData().toString(), ui->cb_airports->currentText(), db, this);
        }
    } else {
        QMessageBox::critical(this, "Ошибка по", db->GetLastError().text());
        emit sig_StartTimer();
    }
}

void MainWindow::handleAirportsData(QStandardItemModel* model)
{
    ui->cb_airports->setModel(model);
    ui->cb_airports->setModelColumn(0);

    if (ui->cb_airports->count() > 0) {
        ui->cb_airports->setCurrentIndex(0);
    }
}

void MainWindow::handleArrivalsData(QSqlQueryModel* model)
{
    model->setHeaderData(0, Qt::Horizontal, "Номер рейса");
    model->setHeaderData(1, Qt::Horizontal, "Время прилета");
    model->setHeaderData(2, Qt::Horizontal, "Аэропорт отправления");
    ui->tv_flights->setModel(model);
    ui->tv_flights->resizeColumnsToContents();
}

void MainWindow::handleDeparturesData(QSqlQueryModel* model)
{
    model->setHeaderData(0, Qt::Horizontal, "Номер рейса");
    model->setHeaderData(1, Qt::Horizontal, "Время вылета");
    model->setHeaderData(2, Qt::Horizontal, "Аэропорт назначения");
    ui->tv_flights->setModel(model);
    ui->tv_flights->resizeColumnsToContents();
}

void MainWindow::handleDBError(QSqlError err)
{
    QMessageBox::critical(this, "Ошибка запроса", err.text());
}

void MainWindow::on_pb_fetch_flights_clicked()
{
    int currentRow = ui->cb_airports->currentIndex();
    QModelIndex currentIndex = ui->cb_airports->model()->index(currentRow, 0);
    QString code = currentIndex.data(Qt::UserRole).toString();
    QString date = ui->de_date->date().toString("yyyy-MM-dd");

    if (code.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выбран аэропорт");
        return;
    }

    if(ui->rb_departure->isChecked()) {
        db->GetDepartures(code, date);
    } else {
        db->GetArrivals(code, date);
    }
}

void MainWindow::on_pb_show_stats_clicked()
{
    statsWindow->setModal(true);
    statsWindow->exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_isWindowClosed = true;
    db->DisconnectFromDataBase();
    event->accept();
}

void MainWindow::ConnectToDB() {
    auto connect_to_DB = [this]{
        ui->lb_status->setText("Подключение...");
        ui->lb_status->setStyleSheet("color:black");
        db->ConnectToDB();
    };
    auto future = QtConcurrent::run(connect_to_DB);
}
