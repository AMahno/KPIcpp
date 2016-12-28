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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <qcustomplot.h>

#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>
#include <QSettings>

QT_USE_NAMESPACE

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    m_sSettingsFile = QApplication::applicationDirPath().left(1) + ":/settings.ini";
    //m_sSettingsFile = "C:/settings.ini";

    calibration = new Calibration;
    currentSettings.curCalibration = calibration;
/*
    for(int i = 0; i<5; i++){
        calibration->addPoint(i*100, i*1.5);
    }
*/
    calibPlot = new QCustomPlot();
    calibBars = new QCPCurve(calibPlot->xAxis, calibPlot->yAxis);
    ui->plot_place->addWidget(calibPlot);
    calibPlot->setMinimumHeight(200);
    calibPlot->xAxis->setRange(0, 1000);
    calibPlot->yAxis->setRange(0, 5);
    calibPlot->addGraph(calibPlot->xAxis, calibPlot->yAxis);

    table = new QTableWidget(0, 2, this);
    ui->verticalLayout->addWidget(table);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setMinimumHeight(200);
    table->setMinimumWidth(100);

    intValidator = new QIntValidator(0, 4000000, this);

    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->applyButton, &QPushButton::clicked,
            this, &SettingsDialog::apply);
    connect(ui->serialPortInfoListBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::showPortInfo);
    connect(ui->baudRateBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::checkCustomBaudRatePolicy);
    connect(ui->serialPortInfoListBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::checkCustomDevicePathPolicy);

    fillPortsParameters();
    fillPortsInfo();
    loadSettings();
    updateSettings();
    updateGraph();
    connect(table->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsDialog::Settings SettingsDialog::settings() const
{
    return currentSettings;
}

void SettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
    ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SettingsDialog::apply()
{
    saveSettings();
    updateSettings();
    hide();
}

void SettingsDialog::checkCustomBaudRatePolicy(int idx){
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate && !loaded) {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void SettingsDialog::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !ui->serialPortInfoListBox->itemData(idx).isValid();
    ui->serialPortInfoListBox->setEditable(isCustomPath);
    if (isCustomPath)
        ui->serialPortInfoListBox->clearEditText();
}

void SettingsDialog::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }

    ui->serialPortInfoListBox->addItem(tr("Custom"));
}

void SettingsDialog::updateSettings(){
    currentSettings.name = ui->serialPortInfoListBox->currentText();

    if (ui->baudRateBox->currentIndex() == 4) {
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();
    //currentSettings.localEchoEnabled = ui->localEchoCheckBox->isChecked();
}

void SettingsDialog::onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight){
    qDebug() << "Data changed";
    if(table->rowCount() == calibration->getPointsSize()){
        for(int i = 0; i<table->rowCount(); i++){
            QTableWidgetItem *leftCol = table->item(i, 0);
            QTableWidgetItem *rightCol = table->item(i, 1);
            if(leftCol != NULL && rightCol != NULL){
                //qDebug() << leftCol->text() << " " << rightCol->text();
                calibration->setPoint(i, leftCol->text().toDouble(), rightCol->text().toDouble());
            }
        }
        updateGraph();
    }
}

void SettingsDialog::loadSettings(){
    qDebug() << "Loading settings";
    QSettings settings(m_sSettingsFile, QSettings::IniFormat);

    QString portName = settings.value("PORT", "").toString();
    int baudRate = settings.value("BAUD", "").toInt();
    if(portName != "" && baudRate != 0){
        ui->serialPortInfoListBox->setCurrentText(portName);
        if (ui->baudRateBox->currentIndex() == 4) {
            ui->baudRateBox->setCurrentText(QString::number(baudRate));
        } else {
            ui->baudRateBox->setItemData(ui->baudRateBox->currentIndex(),
                                         static_cast<QSerialPort::BaudRate>(baudRate));
        }
        loaded = true;
    }else loaded = false;

    int numOfPoints = settings.value("NumOfPoints", "").toInt();
    for(int i = 0; i < numOfPoints; i++){
        int X = settings.value("Px" + QString::number(i)).toInt();
        double Y = settings.value("Py" + QString::number(i)).toDouble();
        calibration->addPoint(X, Y);
        qDebug() << X << Y << calibration->getPointsSize();
        table->insertRow(table->rowCount());
    }
    updateTable();
    qDebug() << portName << baudRate;
}

void SettingsDialog::saveSettings(){
    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
    settings.setValue("PORT", currentSettings.name);
    int baudRate;
    if (ui->baudRateBox->currentIndex() == 4) {
        baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    settings.setValue("BAUD", baudRate);
    settings.setValue("NumOfPoints", calibration->getPointsSize());
    for(int i = 0; i < calibration->getPointsSize(); i++){
        settings.setValue(("Px" + QString::number(i)), calibration->getPoint(i).x);
        settings.setValue(("Py" + QString::number(i)), calibration->getPoint(i).y);
    }
}

void SettingsDialog::changedByUser(){
    loaded = false;
}

void SettingsDialog::on_minusButton_clicked(){
    if(table->rowCount() != 0){
        table->removeRow(table->rowCount()-1);
        calibration->removeLastPoint();
        updateTable();
    }
}

void SettingsDialog::on_plusButton_clicked(){
    qDebug() << "triggered, row count: " << table->rowCount();
    table->insertRow(table->rowCount());
    calibration->addPoint(0, 0);
    updateTable();
}

void SettingsDialog::updateTable(){
    qDebug() << "updating, row count: " << table->rowCount();
    for(int row = 0; row <= table->rowCount(); ++row){
        QTableWidgetItem *leftCol = new QTableWidgetItem(tr("%1").arg(calibration->getPoint(row).x));
        QTableWidgetItem *rightCol = new QTableWidgetItem(tr("%1").arg(calibration->getPoint(row).y));
        table->setItem(row, 0, leftCol);
        table->setItem(row, 1, rightCol);
    }
}

void SettingsDialog::updateGraph(){
    xArray.resize(calibration->getPointsSize());
    yArray.resize(calibration->getPointsSize());

    for(int i = 0; i < calibration->getPointsSize(); i++){
        xArray[i] = calibration->getPoint(i).x;
        yArray[i] = calibration->getPoint(i).y;
    }
    calibPlot->graph(0)->setData(xArray, yArray);
    calibPlot->yAxis->setRangeUpper(calibration->getHighest().y);
    calibPlot->replot();
}
