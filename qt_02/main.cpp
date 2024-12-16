#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtNetwork/QNetworkAccessManager>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QNetworkAccessManager manager;

    return a.exec();
}
