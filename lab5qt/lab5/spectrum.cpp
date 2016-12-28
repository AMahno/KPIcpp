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
                if(X < 1000) GenSpectrum::storage[X] += Y;
            }
        }
    }
}

double Spectrum::getMax(){
    return *std::max_element(GenSpectrum::storage.begin(), GenSpectrum::storage.end());
}

Calibration::Calibration(){
    GenSpectrum::storage.resize(1000);
    GenSpectrum::storage.fill(0);
    //points.resize(5);
}

void Calibration::addPoint(int x, double y){
    point tmpPoint;
    tmpPoint.x = x;
    tmpPoint.y = y;
    points.push_back(tmpPoint);
    interpolate();
}

void Calibration::removeLastPoint(){
    points.pop_back();
    interpolate();
}

point Calibration::getPoint(int index){
    if(index < points.size())
        return points[index];
    else{
        point tmp;
        tmp.x = 0;
        tmp.y = 0;
        return tmp;
    }
}

void Calibration::setPoint(int index, double X, double Y){
    if(index <= points.size()){
        points[index].x = X;
        points[index].y = Y;
        interpolate();
    }
}

int Calibration::getPointsSize(){
    return points.size();
}

point Calibration::getHighest(){
    point tmpPoint;
    tmpPoint.y = points[0].y;
    for(int i = 0; i < points.size(); i++){
        if(points[i].y > tmpPoint.y) tmpPoint = points[i];
    }
    return tmpPoint;
}

void Calibration::interpolate(){
    for(int i = 0; i < points.size()-1; i++){
        for(double j = points[i].x; j < points[i+1].x; j++){
            storage[j] = points[i].y + (j-points[i].x)*(points[i+1].y-points[i].y)/(points[i+1].x-points[i].x);
        }
    }
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

