#ifndef KAASUBJECT_H
#define KAASUBJECT_H

#include <QObject>
#include <QVariantMap>

#define OBSERVER_OBJECT "ob"
#define OBSERVER_SLOT "obslot"

class CkaaSubject : public QObject
{
    Q_OBJECT
public:
    explicit CkaaSubject(QObject *parent = 0);

public:
    //设置发布者
    virtual bool registerPublisher(const QVariantMap& publisherInfoMap) = 0;
    //
    virtual void publishMSG(const QString& strTopic, const QString& strMsg) = 0;
    //注册观察者（观察槽）
    virtual void registerObserver(QObject* ob, const char* observer, const QString& strTopic);
    //移除观察者（观察槽）
    virtual void unregisterObserver(QObject* ob, const char* observer, const QString& strTopic);
private:
    virtual void topicChanged(int type, const QString& strTopic) = 0;
protected:

    QVariantMap mTopicMap;
signals:
    void signal_realtimeData(const QString&, const QString&);
public slots:


};

#endif // KAASUBJECT_H
