#include <QCoreApplication>
#include "smartstreetcontrolkaa.h"
#include <QDebug>
#include <QThread>
#include "lampcontrolcommanddef.h"
#include "kaasubjectmqtt.h"
#include "sslamphistorydata.h"


int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    CSSLampHistorydata hhh;
    hhh.getHistoryData(1, 2222222222222, "01010100010001","rainFull");
    //CKaaSubjectMqtt mqtt;
    //QVariantMap infoMap;
    //infoMap.insert("ip", "192.168.200.183");
    //infoMap.insert("port", 1883);

    //mqtt.registerPublisher(infoMap);

    //mqtt.registerObserver(0, 0, "topic1");
    //mqtt.registerObserver(0, 0, "topic1");
    //mqtt.registerObserver(0, 0, "topic1");



    //return a.exec();
    //exit(-1);
    //return 0;
    //CSmartStreetControlKaa kaaRest;
    //int i = 0;
    //kaaRest.TurnOffLampByStrategy("0101010001", COMMAND_TURNON_ALL_LAMP);
    //for(i = 0; i < 10; ++i)
    //{
    //    if(i%2 == 0)
    //        kaaRest.controlByConfig("2", COMMAND_TURNON_ALL_LAMP);
    //    else
    //        kaaRest.controlByConfig("2", COMMAND_TURNOFF_ALL_LAMP);
    //    QThread::sleep(3);
    //}
    qDebug()<<"test!!!!!!!!!!!!1";
}
