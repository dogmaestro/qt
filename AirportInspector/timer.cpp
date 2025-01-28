#include "timer.h"

Timer::Timer(QObject *parent) : QObject{parent} {
    timer = new QTimer(this);
    timer->setInterval(INTERVAL);
    connect(timer, &QTimer::timeout, this, &Timer::TimeToReconnect);
}

Timer::~Timer() {
    delete timer;
}

void Timer::TimeToReconnect() {
    timer->stop();
    emit sig_Reconnect();
}

void Timer::StartTimer() {
    timer->start();
}
