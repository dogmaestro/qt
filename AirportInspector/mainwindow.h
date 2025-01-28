#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QtConcurrent>
#include "airportstatswindow.h"
#include "database.h"
#include "timer.h"

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

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pb_fetch_flights_clicked();
    void on_pb_show_stats_clicked();

    void handleConnectionStatus(bool status);
    void handleAirportsData(QStandardItemModel* model);
    void handleArrivalsData(QSqlQueryModel* model);
    void handleDeparturesData(QSqlQueryModel* model);
    void handleDBError(QSqlError err);

signals:
    void sig_StartTimer();

private:
    Ui::MainWindow *ui;
    Database *db;
    QString currentAirportCode;
    bool m_isWindowClosed = false;
    AirportStatsWindow* statsWindow;
    Timer* timer;

    void setupDatabase();
    void setupUI();
    void setupConnections();
    void ConnectToDB();
};

#endif // MAINWINDOW_H
