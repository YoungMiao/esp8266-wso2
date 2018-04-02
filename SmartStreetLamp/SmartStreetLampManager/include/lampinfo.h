#ifndef LAMPINFO_H
#define LAMPINFO_H
#include <QString>
#include <QDate>

#define NORTH_SECOND_RING  "LampInfo_northSecondRing"
#define APPLICATION_TOKEN  "0000000"
#define STREET_LAMP_TYPE "SmartStreetLamp"

enum lampControlWay
{
    controlManual = 1,
    controlByTime = 2,
    controlByLight = 3,
    controlByLonLat = 4
};

typedef struct _lampInfoStru
{
    QString  lampSerialNumber;
    QString  belongCityName;
    QString  belongRegionName;
    QString  belongStreetName;
    QString  lampName;  //belongStreetName+lampSerialNumber的后四位”
    float    longitude;
    float    latitude;
    int      lampStatus;
    QString  belongCircle;
    lampControlWay controlWay;
    QString  switchBox;
    QString lampType;
    int lampCount;
    unsigned int lampHeight;
    QString manufacturer;
    QString   department;
    QDate  buildTime;

    _lampInfoStru()
    {
        lampSerialNumber = "";
        belongCityName = "";
        belongRegionName = "";
        belongStreetName = "";
        lampName = "";
        longitude = 0.0;
        latitude = 0.0;
        lampStatus = 0;
        belongCircle = "";
        controlWay = lampControlWay::controlManual;
        switchBox = "";
        lampType = "";
        lampCount = 0;
        lampHeight = 0;
        manufacturer = "";
        department = "";
        buildTime = QDate();
    }

}lampInfoStru;


typedef struct _lampEnvInfoStru
{
    QString lampID;
    QString timeStamp;
    float  temperature;	//温度
    float  humidity;	//湿度
    float  windspeed;	//风速
    float  pm2_5;		//pm2.5
    float  pm10;		//pm10
    float  so2;			//二氧化硫
    float  no2;			//二氧化氮
    float  co;			//一氧化碳
    float  o3;			//臭氧
    float  light;		//光照
    float  rainfall;	//降水量
    float  noisy;		//噪声
    float infrared;      //红外感应值
    bool lampStatus;
    _lampEnvInfoStru()
    {
        lampID = "";
        timeStamp = "";
        temperature = 0;
        humidity = 0;
        windspeed = 0;
        pm2_5 = 0;
        pm10 = 0;
        so2 = 0;
        no2 = 0;
        co = 0;
        o3 = 0;
        light = 0;
        rainfall = 0;
        noisy = 0;
        infrared = 0;
        lampStatus = false;
    }
}lampEnvInfoStru;



#endif // LAMPINFO_H
