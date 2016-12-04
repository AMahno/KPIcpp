#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QObject>
#include <QVector>


class Spectrum : public QObject {
    Q_OBJECT
public:
    Spectrum();
    double getChannel(int);
    void setChannel(int, double);
    void clearSpectrum();
    double getMax();
    QVector<double> getStorage();
    bool accumulation;
public slots:
    void recieve();
private:
    QVector<double> storage;
};

#endif // SPECTRUM_H
