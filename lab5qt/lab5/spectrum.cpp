#include "spectrum.h"
#include "mainwindow.h"

Spectrum::Spectrum(){
    storage.resize(1000);
    storage.fill(0);
}

void Spectrum::setChannel(int channel, double value){
    storage[channel] = value;
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

QVector<double> Spectrum::getStorage(){
    return storage;
}

void Spectrum::clearSpectrum(){
    storage.fill(0);
}

double Spectrum::getMax(){
    return *std::max_element(storage.begin(), storage.end());
}

double Spectrum::getChannel(int channel){
    return storage[channel];
}
