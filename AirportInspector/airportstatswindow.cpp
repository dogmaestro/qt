#include "airportstatswindow.h"
#include "ui_airportstatswindowui.h"
#include <QSqlQuery>
#include <QtCharts>
#include <QSqlRecord>

AirportStatsWindow::AirportStatsWindow(const QString &airportCode,
                                       const QString &airportName,
                                       Database *dbManager,
                                       QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AirportStatsWindowUI),
    m_db(dbManager),
    m_airportCode(airportCode)
{

    ui->setupUi(this);
    setWindowTitle(tr("Статистика загруженности - %1").arg(airportName));
    ui->lb_Airport->setText(airportName);

    initCharts();

    connect(m_db, &Database::sig_YearDataReceived, this, &AirportStatsWindow::HandleYearData);
    connect(m_db, &Database::sig_MonthDataReceived, this, &AirportStatsWindow::HandleMonthData);
    connect(m_db, &Database::sig_Error, this, &AirportStatsWindow::HandleError);
    connect(ui->cb_month, &QComboBox::currentIndexChanged, this, &AirportStatsWindow::OnMonthChanged);
    connect(ui->tw_stats, &QTabWidget::currentChanged, this, &AirportStatsWindow::onTabChanged);

    m_db->RequestYearAndMonthStats(airportCode);
}

AirportStatsWindow::~AirportStatsWindow()
{
    delete m_yearChartView;
    delete m_monthChartView;
    delete ui;
}

void AirportStatsWindow::onTabChanged(int index) {
    ui->cb_month->setCurrentIndex(0);
}

void AirportStatsWindow::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    ui->cb_month->setCurrentIndex(0);
}

void AirportStatsWindow::initCharts()
{
    ui->cv_year->setLayout(new QVBoxLayout());
    ui->cv_month->setLayout(new QVBoxLayout());

    m_yearChartView = new QChartView();
    m_yearChartView->setRenderHint(QPainter::Antialiasing);
    ui->cv_year->layout()->addWidget(m_yearChartView);

    m_monthChartView = new QChartView();
    m_monthChartView->setRenderHint(QPainter::Antialiasing);
    ui->cv_month->layout()->addWidget(m_monthChartView);

    QStringList months = {
        tr("Январь"), tr("Февраль"), tr("Март"), tr("Апрель"),
        tr("Май"), tr("Июнь"), tr("Июль"), tr("Август"),
        tr("Сентябрь"), tr("Октябрь"), tr("Ноябрь"), tr("Декабрь")
    };
    ui->cb_month->addItems(months);
}

void AirportStatsWindow::HandleYearData(QVector<QPair<QDate, int>> yearData)
{
    QVector<int> monthly(12, 0);
    for(auto& item : yearData) {
        int month = item.first.month() - 1;
        if(month >= 0 && month < 12) {
            monthly[month] += item.second;
        }
    }

    QBarSeries *series = new QBarSeries();
    QBarSet *barSet = new QBarSet("Рейсы");
    for(int value : monthly) {
        *barSet << value;
    }
    series->append(barSet);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Загруженность аэропорта за год");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QStringList categories;
    categories << "Янв" << "Фев" << "Мар" << "Апр"
               << "Май" << "Июн" << "Июл" << "Авг"
               << "Сен" << "Окт" << "Ноя" << "Дек";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Месяцы");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Количество рейсов");
    axisY->setLabelFormat("%d");
    axisY->setMin(0);
    axisY->applyNiceNumbers();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_yearChartView->setChart(chart);

    m_yearChartView->setRenderHint(QPainter::Antialiasing);
    m_yearChartView->setRubberBand(QChartView::RectangleRubberBand);
    m_yearChartView->setMinimumSize(800, 400);
    m_yearChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui->cv_year->layout()->setContentsMargins(0, 0, 0, 0);
    ui->cv_year->layout()->setSpacing(0);
}


void AirportStatsWindow::OnMonthChanged(int monthIndex) {
    int month = monthIndex + 1;

    if(!m_monthCache.contains(month)) {
        return;
    }

    QMap<int, int> daily;
    for(int day = 1; day <= 31; day++) daily[day] = 0;

    for(auto& item : m_monthCache[month]) {
        int day = item.first.day();
        daily[day] += item.second;
    }

    QLineSeries *series = new QLineSeries();
    for(auto it = daily.begin(); it != daily.end(); ++it) {
        series->append(it.key(), it.value());
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Количество рейсов по дням"));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText(tr("День месяца"));
    axisX->setLabelFormat("%d");
    axisX->setRange(1, 31);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(tr("Рейсы"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_monthChartView->setChart(chart);
}

void AirportStatsWindow::HandleError(QString error) {
    QMessageBox::critical(this, "Ошибка", error);
}

void AirportStatsWindow::HandleMonthData(QMap<int, QVector<QPair<QDate, int>>> monthData) {
    m_monthCache = monthData;
    OnMonthChanged(0);
}

void AirportStatsWindow::on_pb_close_clicked() {
    close();
}
