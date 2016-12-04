/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

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

    //timer, contorls and progress bar setup
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
    console->setFixedHeight(100);

    //set chart range
    wGraphic->xAxis->setRange(0,1000);
    wGraphic->yAxis->setRange(0,10000);

    //set chart style
    bars->setWidthType(QCPBars::wtPlotCoords);
    bars->setWidth(0.001);
    QBrush barBrush;
    barBrush.setColor(Qt::blue);
    barBrush.setStyle(Qt::SolidPattern);
    bars->setBrush(barBrush);

    xRange = 10000;

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
    rightLine->setPen(QPen(Qt::green));

    wGraphic->replot();

    //create serial instance
    serial = new QSerialPort(this);

    settings = new SettingsDialog;

    //activate actions
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

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
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
        ui->startbutton->setEnabled(true);
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

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
                          "done by Mahnyov Aleksander, NTUU KPI."));
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
        wGraphic->yAxis->setRangeUpper(spectrum->getMax()*1.1);
    }
    bars->setData(coords, spectrum->getStorage());
    updateLines();
    updateChannelData();
    wGraphic->replot();
}


void MainWindow::handleError(QSerialPort::SerialPortError error){
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
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
    connect(ui->actionClear, &QAction::triggered, console, &Console::clear);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::clearChart);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(wGraphic, &QCustomPlot::mousePress, this, &MainWindow::slotMousePress);
}

void MainWindow::showStatusMessage(const QString &message){
    status->setText(message);
}

void MainWindow::on_actionClear_triggered(){

}

void MainWindow::clearChart(){
    spectrum->clearSpectrum();
    bars->setData(coords, spectrum->getStorage());
    wGraphic->replot();
    ui->progressBar->setValue(0);
}

void MainWindow::onTimer(){
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
        xRange = spectrum->getMax()*1.1;
    }else{
        autoscale = true;
    }
}

void MainWindow::slotMousePress(QMouseEvent *event){
    double clickPos = wGraphic->xAxis->pixelToCoord(event->pos().x());
    if(event->button() == Qt::LeftButton) leftLinePos = floor(clickPos);
    else if(event->button() == Qt::RightButton) rightLinePos = floor(clickPos);
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
    updateLines();
    wGraphic->replot();
}

void MainWindow::on_moveLineRight_triggered(){
    leftLinePos++;
    updateLines();
    wGraphic->replot();
}

void MainWindow::updateChannelData(){
    ui->channelData->setText("Pulses at channel "
        + QString::number(leftLinePos) + ": "
        + QString::number(spectrum->getChannel(leftLinePos)));
}

void MainWindow::on_moveLineLeft_g_triggered(){
    rightLinePos--;
    updateLines();
    wGraphic->replot();
}

void MainWindow::on_moveLineRight_g_triggered(){
    rightLinePos++;
    updateLines();
    wGraphic->replot();
}
