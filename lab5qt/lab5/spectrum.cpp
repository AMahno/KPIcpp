#include "spectrum.h"
#include "mainwindow.h"

Spectrum::Spectrum(){
    storage.reserve(1000);
}

void Spectrum::setChannel(int channel, long value){
    storage[channel] = value;
}

void Spectrum::recieve(){
    QSerialPort* serial = static_cast<QSerialPort*>(QObject::sender());
    QByteArray data = serial->readAll();
    QString dataAsString = QTextCodec::codecForMib(1015)->toUnicode(data);
    QStringList dataList = dataAsString.split('!');
    int x = dataList.at(0).toInt();
    int y = dataList.at(1).toInt();
    if(x <= 1000){
        storage[x] = y;
    }
}

QVector<double> Spectrum::getStorage(){
    return storage;
}

void Spectrum::clearSpectrum(){
    storage.fill(0);
}
