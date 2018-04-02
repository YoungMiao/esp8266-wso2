#include "kaasubjectmqtt.h"
#include <QJsonDocument>
#include <QVariant>
#include <QEventLoop>
#include <qDebug>
CKaaSubjectMqtt::CKaaSubjectMqtt(QObject *parent) : CkaaSubject(parent)
{

}
//设置发布者
bool CKaaSubjectMqtt::registerPublisher(const QVariantMap& publisherInfoMap)
{
    QString strHost = publisherInfoMap["ip"].toString();
    int mqttPort = publisherInfoMap["port"].toInt();

    if(strHost.isEmpty() || mqttPort==0)
        return false;
    client = new QMQTT::Client(QHostAddress(strHost), mqttPort);
    if(client != nullptr)
    {
        connect(client, SIGNAL(error(const QMQTT::ClientError)),
                this, SLOT(slot_error(const QMQTT::ClientError)));
        connect(client, SIGNAL(disconnected( )),
                this, SLOT(slot_disconnected( )));
        connect(client, SIGNAL(received(const QMQTT::Message&)),
                this, SLOT(slot_received(const QMQTT::Message&)));

        connect(client, SIGNAL(published(const QMQTT::Message&, quint16)),
                this, SLOT(slot_published(const QMQTT::Message&, quint16)));
        client->setHostName(strHost);
        client->setPort(mqttPort);
        //client->setClientId(strClientId);
        //client->setUsername(strUsername);
        //client->setPassword(strPwd.toUtf8());
        client->setKeepAlive(60);
        client->connectToHost();

        QEventLoop eventLoop;
        connect(client, &QMQTT::Client::connected, &eventLoop, &QEventLoop::quit);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
        if(client->isConnectedToHost())
        {
            qDebug()<<"11111111111111111111111111";
        }else{
            qDebug()<<"22222222222222222222222222";
        }
    }
    return true;
}
void CKaaSubjectMqtt::publishMSG(const QString& strTopic, const QString& strMsg)
{
    QMQTT::Message msg;
    msg.setTopic(strTopic);
    msg.setPayload(strMsg.toUtf8());
    msg.setQos(2);
    qDebug()<<"publish : "<<strTopic<<" - "<<strMsg;
    qDebug()<<client->publish(msg);
}
void CKaaSubjectMqtt::topicChanged(int type, const QString& strTopic)
{
    if(client->isConnectedToHost())
    {
        if(type == 0)
        {
            client->unsubscribe(strTopic);
        }
        if(type == 1)
        {
            client->subscribe(strTopic);
        }
    }
}
void CKaaSubjectMqtt::slot_timeout()
{

}
void CKaaSubjectMqtt::slot_error(const QMQTT::ClientError)
{

}
void CKaaSubjectMqtt::slot_disconnected()
{
    qDebug()<<"CKaaSubjectMqtt::slot_disconnected";
    exit(-1);
}

void CKaaSubjectMqtt::slot_received(const QMQTT::Message& msg)
{
    QString strTopic = msg.topic();
    QVariantList obList = mTopicMap[strTopic].toList();
    for(auto ob : obList)
    {
        QVariantMap varMap = ob.toMap();
        qlonglong ob = varMap[OBSERVER_OBJECT].toLongLong();
        qlonglong observer = varMap[OBSERVER_SLOT].toLongLong();
        connect(this, SIGNAL(signal_realtimeData(const QString&, const QString&)),
                (QObject*)ob, (const char*)observer);
    }
    signal_realtimeData(strTopic, msg.payload());
    for(auto ob : obList)
    {
        QVariantMap varMap = ob.toMap();
        qlonglong ob = varMap[OBSERVER_OBJECT].toLongLong();
        qlonglong observer = varMap[OBSERVER_SLOT].toLongLong();
        disconnect(this, SIGNAL(signal_realtimeData(const QString&, const QString&)),
                (QObject*)ob, (const char*)observer);
    }
}

void CKaaSubjectMqtt::slot_published(const QMQTT::Message& msg, quint16 msgid)
{
    qDebug()<<"slot_published : "<<msgid;
}












