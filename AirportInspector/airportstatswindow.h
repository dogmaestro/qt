#ifndef AIRPORTSTATSWINDOW_H
#define AIRPORTSTATSWINDOW_H

#include <QDialog>
#include <QtCharts>
#include "database.h"
#include "ui_airportstatswindowui.h"

class AirportStatsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AirportStatsWindow(const QString &airportCode,
                                const QString &airportName,
                                Database *dbManager,
                                QWidget *parent = nullptr);
    ~AirportStatsWindow();

private slots:
    void HandleError(QString error);
    void OnMonthChanged(int monthIndex);
    void HandleMonthData(QMap<int, QVector<QPair<QDate, int>>> monthData);
    void HandleYearData(QVector<QPair<QDate, int>> yearData);
    void on_pb_close_clicked();
    void onTabChanged(int index);
    void showEvent(QShowEvent *event) override;

private:
    Ui::AirportStatsWindowUI *ui;
    Database *m_db;
    QString m_airportCode;
    QChartView *m_yearChartView;
    QChartView *m_monthChartView;
    QMap<int, QVector<QPair<QDate, int>>> m_monthCache;
    void initCharts();
};

#endif // AIRPORTSTATSWINDOW_H
