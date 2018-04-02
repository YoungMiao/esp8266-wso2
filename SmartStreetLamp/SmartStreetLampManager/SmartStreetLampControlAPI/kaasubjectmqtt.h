#ifndef CKAASUBJECTMQTT_H
#define CKAASUBJECTMQTT_H

#include "kaasubject.h"
#include "qmqtt/include/qmqtt.h"


#define MQTT_COMMAND_INITLAMPSTATUS "{\"cmmand\":\"GetLampStatues\"}"
class CKaaSubjectMqtt : public CkaaSubject
{
    Q_OBJECT
public:
    explicit CKaaSubjectMqtt(QObject *parent = 0);
public:
    //设置发布者
    virtual bool registerPublisher(const QVariantMap& publisherInfoMap);
    virtual void publishMSG(const QString& strTopic, const QString& strMsg);
private:
    virtual void topicChanged(int type, const QString& strTopic);
signals:

public slots:
    void slot_timeout();
    void slot_error(const QMQTT::ClientError);
    void slot_disconnected();
    void slot_received(const QMQTT::Message&);
    void slot_published(const QMQTT::Message&, quint16);
private:
    QMQTT::Client *client;
};

#endif // CKAASUBJECTMQTT_H
