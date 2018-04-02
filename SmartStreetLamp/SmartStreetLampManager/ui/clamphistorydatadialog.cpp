#include "clamphistorydatadialog.h"
#include "ui_clamphistorydatadialog.h"
#include "setting/csystemsettingdlg.h"
#include "SmartStreetLampControlAPI/sslamphistorydata.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
CLampHistoryDataDialog::CLampHistoryDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLampHistoryDataDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags  flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    m_lampSerialNum = "";
    m_historyEnvDlg = Q_NULLPTR;

    m_LampHistoryServer = Q_NULLPTR;
    QString fileName = QApplication::applicationDirPath();
    fileName += "/resource/echart/historyEnvData.html";
    //qDebug()<<"html file name:"<<fileName;
    ui->webView->load(fileName);

    connect(ui->pushButton_view, SIGNAL(clicked(bool)),this, SLOT(onViewButtonClicked()));
}

CLampHistoryDataDialog::~CLampHistoryDataDialog()
{
    delete ui;
}

void CLampHistoryDataDialog::getLampSerialNumber(QString lampSerial)
{
    m_lampSerialNum = lampSerial;
}

void CLampHistoryDataDialog::onViewButtonClicked()
{
    QDateTime start = ui->dateTimeEdit_start->dateTime();
    QDateTime end = ui->dateTimeEdit_end->dateTime();

    QString envPara= ui->comboBox_weather_info->currentText();
    QString enEnvPara = convertCHLampEnvDataToEn(envPara);

    QVariantList list =  getHistoryEnvData(start, end,m_lampSerialNum, enEnvPara);

    QString historyData = prepareHistoryHourEnvData(list);

    ui->webView->runJavaScript(tr("setSerialName('%1')").arg(envPara));
    ui->webView->runJavaScript(tr("setenvdata2echarts(%1)").arg(historyData));


}

QString CLampHistoryDataDialog::prepareHistoryHourEnvData(QVariantList list)
{
    QVariantList dataList;
    for(auto it: list)
    {
        QVariantMap map = it.toMap();
        qint64 timestamp = map["timestamp"].toLongLong();
        QDateTime dateTime = QDateTime::fromTime_t(timestamp);
        QString strDateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        float value = map["value"].toFloat();
        QVariantMap tempMap;
        tempMap.insert("timestamp", strDateTime);
        tempMap.insert("value", value);
        dataList.append(tempMap);
    }
    QJsonDocument docJson = QJsonDocument::fromVariant(dataList);
    QString str = docJson.toJson();

    qDebug()<<"str:"<<str;
    return str;

}

QVariantList CLampHistoryDataDialog::getHistoryEnvData(QDateTime start, QDateTime end,QString lampID, QString strEnvPara)
{
    QVariantList list;

    qDebug()<<"start time:"<<start<<"end time:"<<end<<"env is:"<<strEnvPara<<endl;
    if(m_LampHistoryServer == nullptr)
    {
       m_LampHistoryServer = new CSSLampHistorydata;
       QString host = "";
       int port = 0;
       gSysSetting->getHistoryDataServerInfo(host,port);
       m_LampHistoryServer->setHost(host);
       m_LampHistoryServer->setPort(QString("%1").arg(port));
    }
    qint64 startT = start.toMSecsSinceEpoch();
    qint64 endT = end.toMSecsSinceEpoch();
    list = m_LampHistoryServer->getHistoryData(startT, endT,lampID,strEnvPara);
    qDebug()<<"getHistoryData:"<<list.count();

    return list;

}
QString CLampHistoryDataDialog::convertCHLampEnvDataToEn(QString chEnvData)
{
    QString enData;
    if(chEnvData == QString::fromWCharArray(L"温度(°C)"))
    {
        enData = "temperature";
    }
    else if(chEnvData == QString::fromWCharArray(L"湿度(%)"))
    {
        enData = "humidity";
    }
    else if(chEnvData == QString::fromWCharArray(L"风速(km/h)"))
    {
        enData = "windspeed";
    }
    else if(chEnvData == QString::fromWCharArray(L"PM2.5"))
    {
        enData = "pm2_5";
    }
    else if(chEnvData == QString::fromWCharArray(L"PM10"))
    {
        enData = "pm10";
    }
    else if(chEnvData == QString::fromWCharArray(L"SO2"))
    {
        enData = "So2";
    }
    else if(chEnvData == QString::fromWCharArray(L"NO2"))
    {
        enData = "No2";
    }
    else if(chEnvData == QString::fromWCharArray(L"CO"))
    {
        enData = "Co";
    }
    else if(chEnvData == QString::fromWCharArray(L"O3"))
    {
        enData = "O3";
    }
    else if(chEnvData == QString::fromWCharArray(L"降水量(mm)"))
    {
        enData = "rainFull";
    }
    else if(chEnvData == QString::fromWCharArray(L"噪声(dB)"))
    {
        enData = "noise";
    }
    else if(chEnvData == QString::fromWCharArray(L"光照"))
    {
        enData = "lux";
    }
    else if(chEnvData == QString::fromWCharArray(L"红外感应值"))
    {
        enData = "infraredValue";
    }
    else
    {
        enData = "all";
    }

    return enData;
}
