#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent) : QObject(parent),
                                        totalEelapsedTime(0),
                                        lastLapTime(0)
{
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &Stopwatch::onTimeout);
}

void Stopwatch::start()
{
    if (!timer->isActive()) {
        timer->start();
    }
}

void Stopwatch::stop()
{
    if (timer->isActive()) {
        timer->stop();
        emit stopped();
    }
}

void Stopwatch::reset()
{
    timer->stop();
    totalEelapsedTime = 0;
    lastLapTime = 0;
    emit timeUpdated(0.0);
}

double Stopwatch::elapsedTime() const
{
    return totalEelapsedTime / 1000.0;
}

double Stopwatch::lapTime()
{
    double result = lastLapTime / 1000.0;
    lastLapTime = 0;
    return result;
}

void Stopwatch::onTimeout()
{
    totalEelapsedTime += 100;
    lastLapTime += 100;
    emit timeUpdated(elapsedTime());
}
