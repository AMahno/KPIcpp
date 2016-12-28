/****************************************************************************
** Spectroscope application
** This is a modified Qt's Serial Monitor example created by:
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** Modified by Aleksander Mahnyov (C) 2016
** cropemail@gmail.com
** NTUU "KPI"
**
** Created as a final C++ project at KEOA, FEL
***/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"
#include "qcustomplot.h"
#include "spectrum.h"
#include <math.h>

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>

QVector<double> coords(1000);

//! [0]
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //timer, controls and progress bar setup
    timer = new QTimer;
    timer->setInterval(1000);
    ui->lineEdit->setText("1000");
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(1000);
    ui->startbutton->setEnabled(false);
    ui->stopbutton->setEnabled(false);

    autoscale = false;

    //create console instance
    console = new Console;
    console->setEnabled(false);

    //create spectrum storage instance
    spectrum = new Spectrum;
    spectrum->accumulation = false;

    //create bar chart instance
    wGraphic = new QCustomPlot();
    bars = new QCPBars(wGraphic->xAxis, wGraphic->yAxis);

    //fill x vector
    for(int i = 0; i < 1000; i++) coords[i] = i;

    //add widgets to UI
    ui->verticalLayout->addWidget(wGraphic);
    ui->verticalLayout->addWidget(console);
    console->setFixedHeight(70);

    //set chart range
    wGraphic->xAxis->setRange(0,1000);
    wGraphic->yAxis->setRange(0,10000);

    //set chart style
    bars->setWidthType(QCPBars::wtPlotCoords);
    bars->setWidth(0.001);
    bars->setAntialiased(false);//makes them occupy one px

    xRange = 10000;

    //cursors
    QVector<double> x(2) , y(2);
            x[0] = 0;
            y[0] = 0;
            x[1] = 0;
            y[1] = xRange;
    leftLine = new QCPCurve(wGraphic->xAxis, wGraphic->yAxis);
    rightLine = new QCPCurve(wGraphic->xAxis, wGraphic->yAxis);
    wGraphic->addPlottable(leftLine);
    wGraphic->addPlottable(rightLine);
    leftLine->setData(x, y);
    rightLine->setData(x, y);
    leftLine->setPen(QPen(Qt::red));
    leftLine->setAntialiased(false);
    rightLine->setPen(QPen(Qt::green));
    leftLine->setAntialiased(false);

    wGraphic->replot();

    //create serial instance
    serial = new QSerialPort(this);

    settings = new SettingsDialog;

    //activate actions
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);
    ui->actionAutoscale->setEnabled(true);
    ui->actionScaleDown->setEnabled(true);
    ui->actionScaleUp->setEnabled(true);
    ui->moveLineLeft->setEnabled(true);
    ui->moveLineRight->setEnabled(true);

    status = new QLabel;
    ui->statusBar->addWidget(status);

    initActionsConnections();
}


MainWindow::~MainWindow(){
    delete settings;
    delete ui;
}


void MainWindow::openSerialPort(){
    SettingsDialog::Settings p = settings->settings();
    qDebug() << p.name << p.baudRate << p.dataBits << p.parity << p.stopBits << p.flowControl;
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        //console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
        ui->startbutton->setEnabled(true);
    } else {
        //QMessageBox::critical(this, tr("Error"), serial->errorString());
        console->putData("Serial Error! " + serial->errorString() + '\n');
        showStatusMessage(tr("Open error"));
    }
}



void MainWindow::closeSerialPort(){
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
    on_stopbutton_clicked();
    ui->startbutton->setEnabled(false);
}

void MainWindow::about(){
    QMessageBox::about(this, tr("Spectroscope about"),
                       tr("The <b>Spectroscope app</b> is a deeply modified Qt's "
                          "Terminal example "
                          "done by Mahnyov Aleksander, NTUU KPI. "
                          "contact: cropemail@gmail.com"));
}


void MainWindow::writeData(const QByteArray &data){
    serial->write(data);
}


void MainWindow::readData(){
    console->putData("Bytes at port: ");
    QString data = QString::number(serial->bytesAvailable());
    console->putData(data);
    console->putData("\n");

    if(autoscale){
        xRange = spectrum->getMax()*1.1;
        wGraphic->yAxis->setRangeUpper(xRange);
    }
    bars->setData(coords, spectrum->getStorage());
    updateLines();
    wGraphic->replot();
}


void MainWindow::handleError(QSerialPort::SerialPortError error){
    if (error == QSerialPort::ResourceError) {
        //QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        console->putData("Serial Error! " + serial->errorString());
        on_stopbutton_clicked();
        closeSerialPort();
    }
}

void MainWindow::initActionsConnections(){
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimer);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(console, &Console::getData, this, &MainWindow::writeData);
    connect(serial, &QSerialPort::readyRead, spectrum, &Spectrum::recieve);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfigure, &QAction::triggered, settings, &MainWindow::show);
    connect(ui->actionConfigure, &QAction::triggered, settings, &SettingsDialog::changedByUser);
    connect(ui->actionClear, &QAction::triggered, console, &Console::clear);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::clearChart);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(wGraphic, &QCustomPlot::mousePress, this, &MainWindow::slotMousePress);
}

void MainWindow::showStatusMessage(const QString &message){
    status->setText(message);
}


void MainWindow::clearChart(){
    spectrum->clearSpectrum();
    bars->setData(coords, spectrum->getStorage());
    wGraphic->replot();
    ui->progressBar->setValue(0);
}

void MainWindow::onTimer(){ //timer value is stored via progress bar (sic)
    int timeout = ui->lineEdit->text().toInt();
    if(ui->progressBar->value() >= timeout){
        timer->stop();
        ui->progressBar->setValue(0);
        spectrum->accumulation = false;
    }
    else ui->progressBar->setValue(ui->progressBar->value()+1);
}

void MainWindow::on_startbutton_clicked(){
    ui->progressBar->setMaximum(ui->lineEdit->text().toInt());
    timer->start();
    spectrum->accumulation = true;
    ui->startbutton->setEnabled(false);
    ui->stopbutton->setEnabled(true);
}

void MainWindow::on_stopbutton_clicked(){
    timer->stop();
    spectrum->accumulation = false;
    ui->stopbutton->setEnabled(false);
    ui->startbutton->setEnabled(true);
}

void MainWindow::on_lineEdit_returnPressed(){
    ui->progressBar->setMaximum(ui->lineEdit->text().toInt());
}

void MainWindow::on_actionScaleUp_triggered(){
    xRange*=2;
    wGraphic->yAxis->setRangeUpper(xRange);
    updateLines();
    wGraphic->replot();
}

void MainWindow::on_actionScaleDown_triggered(){
    xRange/=2;
    wGraphic->yAxis->setRangeUpper(xRange);
    updateLines();
    wGraphic->replot();
}

void MainWindow::on_actionAutoscale_triggered(){
    if(autoscale){
        autoscale = false;
        if(spectrum->getMax()!=0)
            xRange = spectrum->getMax()*1.1;
    }else{
        autoscale = true;
    }
}

void MainWindow::slotMousePress(QMouseEvent *event){
    double clickPos = wGraphic->xAxis->pixelToCoord(event->pos().x());
    if(clickPos > 0){
        if(event->button() == Qt::LeftButton) leftLinePos = floor(clickPos);
        else if(event->button() == Qt::RightButton) rightLinePos = floor(clickPos);
    }
    updateLines();
    wGraphic->replot();
}

void MainWindow::updateLines(){
    QVector<double> x(2), y(2);
    x[0] = leftLinePos;
    y[0] = 0;
    x[1] = leftLinePos;
    y[1] = xRange;
    leftLine->setData(x, y);
    x[0] = rightLinePos;
    x[1] = rightLinePos;
    rightLine->setData(x, y);
    updateChannelData();
}

void MainWindow::on_moveLineLeft_triggered(){
    leftLinePos--;
    if(leftLinePos < 0) leftLinePos = 0;
    updateLines();
    wGraphic->replot();
}

void MainWindow::on_moveLineRight_triggered(){
    leftLinePos++;
    updateLines();
    wGraphic->replot();
}

void MainWindow::updateChannelData(){
    SettingsDialog::Settings p = settings->settings();
    ui->channelData->setText("Pulses at channel "
        + QString::number(leftLinePos) + ": "
        + QString::number(spectrum->getChannel(leftLinePos)) + ". "
        + "Channel's energy: "
        + QString::number(p.curCalibration->getStorage().at(leftLinePos)) + " MeV.");
}

void MainWindow::on_moveLineLeft_g_triggered(){
    rightLinePos--;
    if(rightLinePos < 0) leftLinePos = 0;
    updateLines();
    wGraphic->replot();
}

void MainWindow::on_moveLineRight_g_triggered(){
    rightLinePos++;
    updateLines();
    wGraphic->replot();
}

void MainWindow::on_actionClear_triggered(){
    xRange = 10000;
    updateLines();
}
