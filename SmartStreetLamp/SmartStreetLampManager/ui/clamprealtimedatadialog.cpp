#include "clamprealtimedatadialog.h"
#include "ui_clamprealtimedatadialog.h"
#include "SmartStreetLampControlAPI/kaasubjectmqtt.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
CLampRealTimeDataDialog::CLampRealTimeDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLampRealTimeDataDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags  flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    initRealTimeDataTable();
    m_lampSerialNum = "";
}

CLampRealTimeDataDialog::~CLampRealTimeDataDialog()
{
    delete ui;
}

void CLampRealTimeDataDialog::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    QPixmap pixmap(":/resource/lamp.png");
    ui->widget_pictrue->setAutoFillBackground(true);
    QPixmap newpix = pixmap.scaled(ui->widget_pictrue->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPalette palette = ui->widget_pictrue->palette();
    palette.setBrush(QPalette::Window, QBrush(newpix));
    ui->widget_pictrue->setPalette(palette);
}

void CLampRealTimeDataDialog::initRealTimeDataTable()
{
    ui->tableWidget_realData->setColumnCount(2);
    ui->tableWidget_realData->setRowCount(14);
    ui->tableWidget_realData->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_realData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_realData->setEditTriggers(QHeaderView::NoEditTriggers);
    ui->tableWidget_realData->horizontalHeader()->setVisible(false);
    ui->tableWidget_realData->verticalHeader()->setVisible(false);

    for(int i = 0; i < ui->tableWidget_realData->rowCount(); i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem();
        QTableWidgetItem* item2 = new QTableWidgetItem();
        ui->tableWidget_realData->setItem(i, 0, item);
        ui->tableWidget_realData->setItem(i, 1, item2);
    }

    ui->tableWidget_realData->item(0, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"路灯ID")));
    ui->tableWidget_realData->item(1, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"温度(°C)")));
    ui->tableWidget_realData->item(2, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"湿度(%)")));
    ui->tableWidget_realData->item(3, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"风速(km/h)")));
    ui->tableWidget_realData->item(4, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"PM10")));
    ui->tableWidget_realData->item(5, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"Pm2.5")));
    ui->tableWidget_realData->item(6, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"SO2")));
    ui->tableWidget_realData->item(7, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"NO2")));
    ui->tableWidget_realData->item(8, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"O3")));
    ui->tableWidget_realData->item(9, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"CO")));
    ui->tableWidget_realData->item(10, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"光照")));
    ui->tableWidget_realData->item(11, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"降水量(mm)")));
    ui->tableWidget_realData->item(12, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"噪声(dB)")));
    ui->tableWidget_realData->item(13, 0)->setText(QString("%1").arg(QString::fromWCharArray(L"红外感应")));

}

void CLampRealTimeDataDialog::subscribeRealTimeTopic(QString strLampID)
{
    QString strTopic = QString("/kaa/smartlamp/loger/%1/").arg(APPLICATION_TOKEN) +strLampID;
    qDebug()<<"strTopic:"<<strTopic;
    m_kaaRegistServer->registerObserver(this, SLOT(onGetRealTimeData(const QString&, const QString&)), strTopic);
    m_lampSerialNum = strLampID;
}

void CLampRealTimeDataDialog::setRealTimeData(lampEnvInfoStru&  lampEnvInfo)
{
    if(lampEnvInfo.lampID != m_lampSerialNum)
    {
        return;
    }
    resetEnvData();
    ui->tableWidget_realData->item(0, 1)->setText(QString("%1").arg(lampEnvInfo.lampID));
    ui->tableWidget_realData->item(1, 1)->setText(QString("%1").arg(lampEnvInfo.temperature));
    ui->tableWidget_realData->item(2, 1)->setText(QString("%1").arg(lampEnvInfo.humidity));
    ui->tableWidget_realData->item(3, 1)->setText(QString("%1").arg(lampEnvInfo.windspeed));
    ui->tableWidget_realData->item(4, 1)->setText(QString("%1").arg(lampEnvInfo.pm10));
    ui->tableWidget_realData->item(5, 1)->setText(QString("%1").arg(lampEnvInfo.pm2_5));
    ui->tableWidget_realData->item(6, 1)->setText(QString("%1").arg(lampEnvInfo.so2));
    ui->tableWidget_realData->item(7, 1)->setText(QString("%1").arg(lampEnvInfo.no2));
    ui->tableWidget_realData->item(8, 1)->setText(QString("%1").arg(lampEnvInfo.co));
    ui->tableWidget_realData->item(9, 1)->setText(QString("%1").arg(lampEnvInfo.o3));
    ui->tableWidget_realData->item(10, 1)->setText(QString("%1").arg(lampEnvInfo.light));
    ui->tableWidget_realData->item(11, 1)->setText(QString("%1").arg(lampEnvInfo.rainfall));
    ui->tableWidget_realData->item(12, 1)->setText(QString("%1").arg(lampEnvInfo.noisy));
    ui->tableWidget_realData->item(13, 1)->setText(QString("%1").arg(lampEnvInfo.infrared));
}

void CLampRealTimeDataDialog::resetEnvData()
{
    for(int i = 0; i < ui->tableWidget_realData->rowCount(); i++)
    {
        ui->tableWidget_realData->item(i, 1)->setText("");
    }
}

void CLampRealTimeDataDialog::setKaaRegistServer(CkaaSubject *kaaRegistServer)
{
    m_kaaRegistServer = kaaRegistServer;
}

void CLampRealTimeDataDialog::onGetRealTimeData(const QString &strTopic, const QString &strEnvInfo)
{
    qDebug()<<"onGetRealTimeData"<<"strTopic:"<<strTopic<<"strEnvInfo"<<strEnvInfo;
    parseJsonData(strEnvInfo);

    setRealTimeData(m_realTimeEnvInfo);

}

void CLampRealTimeDataDialog::parseJsonData(QString strEnvJson)
{
    QByteArray byteArr = strEnvJson.toUtf8();
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(byteArr, &err);
    if(!document.isNull() && err.error == QJsonParseError::NoError)
    {
        if(document.isObject())
        {
            QVariantMap map = document.toVariant().toMap();

            QVariantMap baseMap = map["Lampbase"].toMap();
            m_realTimeEnvInfo.lampID = baseMap["lampGuid"].toString();
            QVariantMap envMap = map["LampEnvironment"].toMap();
            m_realTimeEnvInfo.temperature = envMap["temperature"].toFloat();
            m_realTimeEnvInfo.humidity = envMap["humidity"].toFloat();
            m_realTimeEnvInfo.windspeed = envMap["windspeed"].toFloat();
            m_realTimeEnvInfo.pm2_5 = envMap["pm2_5"].toFloat();
            m_realTimeEnvInfo.pm10 = envMap["pm10"].toFloat();
            m_realTimeEnvInfo.so2 = envMap["So2"].toFloat();
            m_realTimeEnvInfo.no2 = envMap["No2"].toFloat();
            m_realTimeEnvInfo.co = envMap["Co"].toFloat();
            m_realTimeEnvInfo.o3 = envMap["O3"].toFloat();
            m_realTimeEnvInfo.rainfall = envMap["rainFull"].toFloat();
            m_realTimeEnvInfo.noisy = envMap["noise"].toFloat();
            m_realTimeEnvInfo.light = envMap["lux"].toFloat();
            m_realTimeEnvInfo.infrared = envMap["infraredValue"].toFloat();
            QVariantMap  selfMap = map["LampSelf"].toMap();
            m_realTimeEnvInfo.lampStatus = selfMap["lampStatus"].toBool();
        }
    }

}

void CLampRealTimeDataDialog::closeEvent(QCloseEvent *event)
{
    QString strTopic = QString("/kaa/smartlamp/loger/%1/").arg(APPLICATION_TOKEN) +m_lampSerialNum;
    qDebug()<<"closeEvent:strTopic:"<<strTopic;
    m_kaaRegistServer->unregisterObserver(this, SLOT(onGetRealTimeData(const QString&, const QString&)), strTopic);
    resetEnvData();
    QDialog::closeEvent(event);
}

