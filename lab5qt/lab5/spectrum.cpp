#include "spectrum.h"
#include "mainwindow.h"

Spectrum::Spectrum(){
    storage.resize(1000);
    storage.fill(0);
}

void Spectrum::recieve(){
    QSerialPort* serial = static_cast<QSerialPort*>(QObject::sender());
    QString data;
    while(serial->canReadLine()){ //yup, this is parser. TODO: create error handlers
        data = serial->readLine();
        if(accumulation){
            QStringList dataList = data.split('!');
            if(dataList.size() == 2){
                QString x = dataList.at(0);
                double X = x.toDouble();
                QString y = dataList.at(1);
                y.remove('\n');
                y.remove('\r');
                double Y = y.toDouble();
                if(X < 1000) storage[X] += Y;
            }
        }
    }
}

double Spectrum::getMax(){
    return *std::max_element(storage.begin(), storage.end());
}

Calibration::Calibration(){
    storage.resize(1000);
    storage.fill(0);
}

void Calibration::addPoint(int x, double y){
    point tmpPoint;
    tmpPoint.x = x;
    tmpPoint.y = y;
    points.push_back(tmpPoint);
}

GenSpectrum::GenSpectrum(){
    storage.resize(1000);
    storage.fill(0);
}

void GenSpectrum::setChannel(int channel, double value){
    storage[channel] = value;
}

QVector<double> GenSpectrum::getStorage(){
    return storage;
}

void GenSpectrum::clearSpectrum(){
    storage.fill(0);
}

double GenSpectrum::getChannel(int channel){
    return storage[channel];
}

