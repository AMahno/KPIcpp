#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QObject>
#include <QVector>

struct point{
    double x;
    double y;
};

class GenSpectrum : public QObject {
    Q_OBJECT
protected:
    QVector<double> storage;
public:
    GenSpectrum();
    double getChannel(int);
    void setChannel(int, double);
    void clearSpectrum();
    QVector<double> getStorage();
};

class Spectrum : public GenSpectrum {
    Q_OBJECT
public:
    Spectrum();
    double getMax();
    bool accumulation;
public slots:
    void recieve();
private:
    QVector<double> storage;
};

class Calibration : public GenSpectrum {
    Q_OBJECT
public:
    Calibration();
    void addPoint(int, double);
    void setPoint(int, double, double);
    void removeLastPoint();
    point getPoint(int);
    int getPointsSize();
    point getHighest();
private:
    QVector<point> points;
    void interpolate();
};

#endif // SPECTRUM_H
