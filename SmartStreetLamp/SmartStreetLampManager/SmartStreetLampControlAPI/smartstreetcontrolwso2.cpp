#include "smartstreetcontrolwso2.h"
#include "setting/csystemsettingdlg.h"
#include "include/lampinfo.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
CSmartStreetControlWSO2::CSmartStreetControlWSO2(QObject *parent) : APIBase(parent)
{

}


bool CSmartStreetControlWSO2::TurnOnLampByLampidList(const QStringList& lampIdList)
{
    QJsonObject opObject;
    QJsonArray  jsonarray = QJsonArray::fromStringList(lampIdList);
    opObject.insert("code", "ON");
    opObject.insert("type", "COMMAND");
    QJsonObject obj;
    obj.insert("deviceIdentifiers", jsonarray);
    obj.insert("operation", opObject);

    QByteArray byteArr = QJsonDocument(obj).toJson();
    QString operationStr = QString(byteArr);
    qDebug()<<"operationStr:"<<operationStr;
    gSysSetting->generateApplicationKey();
    gSysSetting->generateAccessToken();
    QNetworkAccessManager manager;
    QNetworkRequest  request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString headerData = "Bearer " + gSysSetting->getAccessToken();
    request.setRawHeader("Authorization", headerData.toUtf8());

    qDebug()<<headerData;
    QString host = "";
    int httpport = -1;
    int mqttport = -1;
    gSysSetting->getWSO2ServerHostAndPort(host, httpport, mqttport);
    QString strUrl = QString("http://%1:%2/api/device-mgt/v1.0/devices/%3/operations")
            .arg(host).arg(httpport).arg(STREET_LAMP_TYPE);
    request.setUrl(QUrl(strUrl));
    QString strParam;
    strParam = operationStr;
    qDebug()<<"TurnOnLampByLampidList strUrl:"<<strUrl;
    //qDebug()<<"TurnOnLampByLampidList strParam:"<<strParam;
    QNetworkReply* reply = manager.post(request, strParam.toUtf8());
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"Lighting on success.";
        return true;
    }
    qDebug()<<"Lighting on failed."<<reply->errorString();
    return false;

}

bool CSmartStreetControlWSO2::TurnOffLampByLampidList(const QStringList& lampIdList)
{
    QJsonObject opObject;
    QJsonArray  jsonarray = QJsonArray::fromStringList(lampIdList);
    opObject.insert("code", "OFF");
    opObject.insert("type", "COMMAND");
    QJsonObject obj;
    obj.insert("deviceIdentifiers", jsonarray);
    obj.insert("operation", opObject);

    QByteArray byteArr = QJsonDocument(obj).toJson();
    QString operationStr = QString(byteArr);
   // qDebug()<<"operationStr:"<<operationStr;
    gSysSetting->generateApplicationKey();
    gSysSetting->generateAccessToken();
    QNetworkAccessManager manager;
    QNetworkRequest  request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString headerData = "Bearer " + gSysSetting->getAccessToken();
    request.setRawHeader("Authorization", headerData.toUtf8());
    QString host = "";
    int httpport = -1;
    int mqttport = -1;
    gSysSetting->getWSO2ServerHostAndPort(host, httpport, mqttport);
    QString strUrl = QString("http://%1:%2/api/device-mgt/v1.0/devices/%3/operations")
            .arg(host).arg(httpport).arg(STREET_LAMP_TYPE);
    request.setUrl(QUrl(strUrl));
    QString strParam;
    strParam = operationStr;
    qDebug()<<"TurnOffLampByLampidList strUrl:"<<strUrl;
    //qDebug()<<"TurnOffLampByLampidList strParam:"<<strParam;
    QNetworkReply* reply = manager.post(request, strParam.toUtf8());
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"Lighting off success.";
        return true;
    }
    qDebug()<<"Lighting off falied."<<reply->errorString();
    return false;

}
