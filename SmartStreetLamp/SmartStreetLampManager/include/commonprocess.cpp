#include <QString>
#include "include/lampinfo.h"
QString lampStatusToString(int status)
{
    QString strStatus = "";
    switch (status) {
    case 0:
        strStatus = QString::fromWCharArray(L"关灯状态");
        break;
    case 1:
        strStatus = QString::fromWCharArray(L"开灯状态");
        break;
    case 2:
        strStatus = QString::fromWCharArray(L"离线状态");
        break;
    default:
        break;
    }

    return strStatus;
}

QString lampControlWayToString(lampControlWay controlway)
{
    QString strControlWay;
    if(controlway == lampControlWay::controlManual)
    {
        strControlWay = QString::fromWCharArray(L"人工控制");
    }
    else if(controlway == lampControlWay::controlByTime)
    {
        strControlWay = QString::fromWCharArray(L"根据时间控制");
    }
    else if(controlway == lampControlWay::controlByLight)
    {
        strControlWay = QString::fromWCharArray(L"根据光照强度控制");
    }
    else if(controlway == lampControlWay::controlByLonLat)
    {
        strControlWay = QString::fromWCharArray(L"根据经纬度控制");
    }
    else
    {
        strControlWay = "";
    }

    return strControlWay;
}
