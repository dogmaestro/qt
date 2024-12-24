#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);

    void start();
    void stop();
    void reset();

    double elapsedTime() const;
    double lapTime();

signals:
    void timeUpdated(double time);
    void stopped();

private slots:
    void onTimeout();

private:
    QTimer *timer;
    int totalEelapsedTime;
    int lastLapTime;
};
#endif // STOPWATCH_H
