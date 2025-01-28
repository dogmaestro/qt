#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlError>
#include <qstandarditemmodel.h>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    void AddDataBase(QString driver, QString nameDB = "");
    QSqlError GetLastError();
    void ConnectToDB();
    void DisconnectFromDataBase(QString nameDb = "");
    void GetAirports();
    void GetArrivals(const QString& airport_code, const QString& date);
    void GetDepartures(const QString& airport_code, const QString& date);
    void RequestYearAndMonthStats(const QString& airport_code);

signals:
    void sig_SendStatusConnection(bool status_connection);
    void sig_SendStatusRequest(QSqlError err);
    void sig_SendAirports(QStandardItemModel* model);
    void sig_SendArrivals(QSqlQueryModel* model);
    void sig_SendDepartures(QSqlQueryModel* model);
    void sig_Error(QString error);
    void sig_YearDataReceived(QVector<QPair<QDate, int>> yearData);
    void sig_MonthDataReceived(QMap<int, QVector<QPair<QDate, int>>> monthData);

private:
    QSqlDatabase *data_base;
    bool status_connection;
    QString currentDBName;
    void ProcessYearMonthQuery(QSqlQuery& query);
};

#endif // DATABASE_H
