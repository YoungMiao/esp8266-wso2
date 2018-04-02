# -*- coding: utf-8 -*-
import csv
import math
import matplotlib.pyplot as plt
x_pi = 3.14159265358979324 * 3000.0 / 180.0
pi = 3.1415926535897932384626  # π
a = 6378245.0  # 长半轴
ee = 0.00669342162296594323  # 偏心率平方

interDis = (30 / 0.111322222) * 0.000001
print(interDis)

data = []
x0 = []
y0 = []

def _transformlat(lng, lat):
    ret = -100.0 + 2.0 * lng + 3.0 * lat + 0.2 * lat * lat + \
          0.1 * lng * lat + 0.2 * math.sqrt(math.fabs(lng))
    ret += (20.0 * math.sin(6.0 * lng * pi) + 20.0 *
            math.sin(2.0 * lng * pi)) * 2.0 / 3.0
    ret += (20.0 * math.sin(lat * pi) + 40.0 *
            math.sin(lat / 3.0 * pi)) * 2.0 / 3.0
    ret += (160.0 * math.sin(lat / 12.0 * pi) + 320 *
            math.sin(lat * pi / 30.0)) * 2.0 / 3.0
    return ret

def _transformlng(lng, lat):
    ret = 300.0 + lng + 2.0 * lat + 0.1 * lng * lng + \
          0.1 * lng * lat + 0.1 * math.sqrt(math.fabs(lng))
    ret += (20.0 * math.sin(6.0 * lng * pi) + 20.0 *
            math.sin(2.0 * lng * pi)) * 2.0 / 3.0
    ret += (20.0 * math.sin(lng * pi) + 40.0 *
            math.sin(lng / 3.0 * pi)) * 2.0 / 3.0
    ret += (150.0 * math.sin(lng / 12.0 * pi) + 300.0 *
            math.sin(lng / 30.0 * pi)) * 2.0 / 3.0
    return ret

def bd09_to_gcj02(bd_lon, bd_lat):
    """
    百度坐标系(BD-09)转火星坐标系(GCJ-02)
    百度——>谷歌、高德
    :param bd_lat:百度坐标纬度
    :param bd_lon:百度坐标经度
    :return:转换后的坐标列表形式
    """
    x = bd_lon - 0.0065
    y = bd_lat - 0.006
    z = math.sqrt(x * x + y * y) - 0.00002 * math.sin(y * x_pi)
    theta = math.atan2(y, x) - 0.000003 * math.cos(x * x_pi)
    gg_lng = z * math.cos(theta)
    gg_lat = z * math.sin(theta)
    return [gg_lng, gg_lat]
def gcj02_to_wgs84(lng, lat):
    """
    GCJ02(火星坐标系)转GPS84
     :param lng:火星坐标系的经度
     :param lat:火星坐标系纬度
     :return:
     """
    dlat = _transformlat(lng - 105.0, lat - 35.0)
    dlng = _transformlng(lng - 105.0, lat - 35.0)
    radlat = lat / 180.0 * pi
    magic = math.sin(radlat)
    magic = 1 - ee * magic * magic
    sqrtmagic = math.sqrt(magic)
    dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * pi)
    dlng = (dlng * 180.0) / (a / sqrtmagic * math.cos(radlat) * pi)
    mglat = lat + dlat
    mglng = lng + dlng
    return [lng * 2 - mglng, lat * 2 - mglat]

# 省  市 区 道路 路灯编号
# 01 01 01 0001 0001
#010103表示未央区
#0101030001表示未央区北二环路
# gen ID
def genLampID(lampID):
    strLamp = '0101030001'
    # print("%s%d" % (strLamp, lampID))
    return ("%s%04d" % (strLamp, lampID))

def getdata():
    global x0, y0
    i = 0
    with open('northSecondRing2.csv', "r") as f:
        reader = csv.DictReader(f)
        header = reader.fieldnames
        for line in reader:
            data.append(line)
        for dat in data:
            i = i + 1
            if i % 1 == 0:
                x0.append(float(dat['lon']))
                y0.append(float(dat['lat']))


def chazhi(x1, y1, x2, y2, value):
    xb = math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    xx = (x2 - x1) / xb * 0.000001
    yy = (y2 - y1) / xb * 0.000001
    # print(xx, yy)
    xTmp = []
    yTmp = []
    while True:
        xTmp.append(x1)
        yTmp.append(y1)
        x1 = x1 + xx
        y1 = y1 + yy
        #东西路比较x1和x2,南北路比较y1和y2
        if (x1 > x2):
            break
    return xTmp, yTmp


def plot_test():
    index = 0
    xxx = []
    yyy = []
    while True:
        index = index + 1
        if (index >= len(x0)):
            break
        xx0, yy0 = chazhi(x0[index - 1], y0[index - 1], x0[index], y0[index], 1)
        xxx += xx0
        yyy += yy0
    count = int(interDis / 0.000001)
    ii = 0
    xxx1 = []
    yyy1 = []

    strXXX = []
    strYYY = []
    strID = []
    strStatus = []
    len1 = len(xxx)
    id = -1
    while True:
        if ii % count == 0:
            id = id + 2
            print(id)
            #lon = xxx[ii]
            #lat = yyy[ii]
            #北二环的坐标是拾取的百度地图的，所以需要先转火星坐标系，再转WGS84
            lon, lat = bd09_to_gcj02(xxx[ii], yyy[ii])
            #西沣路/西部大道/西太路是拾取的谷歌地图的，直接转wgs84就行
            lon, lat = gcj02_to_wgs84(lon, lat)
            xxx1.append(lon)
            yyy1.append(lat)
            strXXX.append(str(round(lon, 6)))
            strYYY.append(str(round(lat, 6)))
            strID.append(genLampID(id))
            strStatus.append("0")
        ii = ii + 1
        if ii >= len1:
            break
    with open('LampInfos1.csv', 'w', encoding='utf8', newline='') as f:
        writer = csv.writer(f)
        header = ['lon', 'lat', 'lampID','lampStatus']
        writer.writerow(header)
        writer.writerows(zip(strXXX, strYYY, strID, strStatus))
    plt.figure()
    plt.plot(xxx1, yyy1, '*')
    plt.show()
    return


getdata()
plot_test()














