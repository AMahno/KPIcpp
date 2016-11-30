#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QObject>
#include <QVector>


class Spectrum : public QObject {
    Q_OBJECT
public:
    Spectrum();
    long getChannel(int);
    void setChannel(int, double);
    void clearSpectrum();
    QVector<double> getStorage();
public slots:
    void recieve();
private:
    QVector<double> storage;
};

#endif // SPECTRUM_H
