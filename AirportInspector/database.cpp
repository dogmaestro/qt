#include "database.h"
#include <QSqlQuery>
#include <qdatetime.h>
#include <qstandarditemmodel.h>

Database::Database(QObject *parent) : QObject(parent)
{
    data_base = nullptr;
    status_connection = false;
}

Database::~Database()
{
    if(data_base) {
        data_base->close();
        delete data_base;
    }
}

void Database::AddDataBase(QString driver, QString nameDB)
{
    data_base = new QSqlDatabase(QSqlDatabase::addDatabase(driver, nameDB));
    currentDBName = nameDB;
}

QSqlError Database::GetLastError()
{
    if(data_base)
        return data_base->lastError();
    return QSqlError();
}

void Database::ConnectToDB()
{
    if(!data_base) return;

    data_base->setHostName("981757-ca08998.tmweb.ru");
    data_base->setDatabaseName("demo");
    data_base->setUserName("netology_usr_cpp");
    data_base->setPassword("CppNeto3");

    status_connection = data_base->open();
    emit sig_SendStatusConnection(status_connection);
}

void Database::DisconnectFromDataBase(QString nameDb)
{
    if(data_base) {
        data_base->close();
        QSqlDatabase::removeDatabase(nameDb.isEmpty() ? currentDBName : nameDb);
        status_connection = false;
        emit sig_SendStatusConnection(status_connection);
    }
}

void Database::GetAirports()
{
    QStandardItemModel *model = new QStandardItemModel();

    QSqlQuery query(*data_base);
    query.prepare("SELECT airport_name->>'ru' as name, airport_code as code "
                  "FROM bookings.airports_data order by 1");

    if(query.exec()) {
        while(query.next()) {
            QStandardItem *item = new QStandardItem(query.value("name").toString());
            item->setData(query.value("code").toString(), Qt::UserRole);
            model->appendRow(item);
        }
    }
    emit sig_SendAirports(model);
}

void Database::GetArrivals(const QString &airport_code, const QString &date)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(*data_base);
    query.prepare("SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' "
                  "FROM bookings.flights f "
                  "JOIN bookings.airports_data ad ON ad.airport_code = f.departure_airport "
                  "WHERE f.arrival_airport = ? AND DATE(scheduled_arrival) = ?");
    query.addBindValue(airport_code);
    query.addBindValue(date);
    query.exec();
    model->setQuery(query);
    emit sig_SendArrivals(model);
}

void Database::GetDepartures(const QString &airport_code, const QString &date)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(*data_base);
    query.prepare("SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' "
                  "FROM bookings.flights f "
                  "JOIN bookings.airports_data ad ON ad.airport_code = f.arrival_airport "
                  "WHERE f.departure_airport = ? AND DATE(scheduled_departure) = ?");
    query.addBindValue(airport_code);
    query.addBindValue(date);
    query.exec();
    model->setQuery(query);
    emit sig_SendDepartures(model);
}

void Database::RequestYearAndMonthStats(const QString& airport_code) {
    QSqlQuery query(*data_base);
    query.prepare(R"(
        SELECT
            DATE_TRUNC('day', day) as day,
            COUNT(*) as total
        FROM (
            SELECT scheduled_departure as day
            FROM bookings.flights
            WHERE departure_airport = :code
            UNION ALL
            SELECT scheduled_arrival as day
            FROM bookings.flights
            WHERE arrival_airport = :code
        ) combined
        GROUP BY DATE_TRUNC('day', day)
        ORDER BY day
    )");
    query.bindValue(":code", airport_code);

    if(!query.exec()) {
        emit sig_Error(query.lastError().text());
        return;
    }

    ProcessYearMonthQuery(query);
}

void Database::ProcessYearMonthQuery(QSqlQuery& query) {
    QVector<QPair<QDate, int>> yearData;
    QMap<int, QVector<QPair<QDate, int>>> monthData;

    while(query.next()) {

        QDate date = query.value(0).toDate();
        int count = query.value(1).toInt();

        yearData.append({date, count});
        int month = date.month();
        monthData[month].append({date, count});
    }

    emit sig_YearDataReceived(yearData);
    emit sig_MonthDataReceived(monthData);
}
