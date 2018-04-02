#include "kaasubject.h"
#include <qDebug>
CkaaSubject::CkaaSubject(QObject *parent) : QObject(parent)
{

}

//注册观察者（观察槽）
void CkaaSubject::registerObserver(QObject* ob, const char* observer,
                                   const QString& strTopic)
{
    if(mTopicMap.find(strTopic) == mTopicMap.end())
    {
        topicChanged(1, strTopic);
    }
    QVariantList obList = mTopicMap[strTopic].toList();
    QVariantMap var;
    var.insert(OBSERVER_OBJECT, qlonglong(ob));
    var.insert(OBSERVER_SLOT, qlonglong(observer));
    if(!obList.contains(var))
    {
        obList.append(var);
        mTopicMap[strTopic] = obList;
    }
    return;
}
//移除观察者（观察槽）
void CkaaSubject::unregisterObserver(QObject* ob, const char* observer, const QString& strTopic)
{
    if(mTopicMap.find(strTopic) == mTopicMap.end())
    {
        return ;
    }
    QVariantList obList = mTopicMap[strTopic].toList();
    QVariantMap var;
    var.insert(OBSERVER_OBJECT, qlonglong(ob));
    var.insert(OBSERVER_SLOT, qlonglong(observer));
    if(obList.contains(var))
    {
        obList.removeOne(var);
        if(obList.count() == 0)
        {
            mTopicMap.remove(strTopic);
            topicChanged(0, strTopic);
        }else
        {
            mTopicMap[strTopic] = obList;
        }
    }
    return;
}

