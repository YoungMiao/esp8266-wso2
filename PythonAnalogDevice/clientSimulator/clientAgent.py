#!/usr/bin python
#coding:utf-8

import sys, os, json
import time, threading, datetime, calendar
import mqttConnector
import requests
import generateAccessToken


PUSH_INTERVAL = 30  # time interval between successive data pushes in seconds
REGISTER_URL = "http://192.168.7.51:8280"
REALDATA_URL = "http://192.168.200.129:5002"
LAMPBASICDATA_URL = "http://192.168.200.129:5005"

global DEVICETYPE
DEVICETYPE = "SmartStreetLamp"

def getServerName(username):
    if(username.find("@") == -1):
        return "carbon.super"
    else:
        return (username.split("@"))[1]


def getLampBasicData():
    headers = {'Content-Type' : 'application/x-www-form-urlencoded'}
    baseUrl = LAMPBASICDATA_URL + '/operate'
    params = 'methodinfo=get-all-tables&db_label='''
    response = requests.post(baseUrl, data = params, headers = headers)
    jsonStr = json.loads(response.content);
    #print("lampBasic:", jsonStr)
    return jsonStr

def getSensorData(timestamp, lon, lat, cof):
    headers = {'Content-Type' : 'application/x-www-form-urlencoded'}
    baseUrl = REALDATA_URL + '/dataserver'
    #print("headers:", headers)
    #print("baseUrl:", baseUrl)
    payload = {"timestamp":timestamp,"longitude":lon,"latitude":lat}
    #print("getSensorData payload:", payload)
    response = requests.post(baseUrl, data=payload, headers=headers);
    #print("getSensorData response:", response.content)
    
    jsonStr = json.loads(response.content);
    wind = jsonStr["WindSpeed(km/h)"]
    pm10 = jsonStr["PM10(ug/m3)"]
    pm2_5 = jsonStr["PM2.5(ug/m3)"]
    co = jsonStr["CO(mg/m3)"]
    humidity = jsonStr["Humidity(%)"]
    no2 = jsonStr["NO2(ug/m3)"]
    o3 = jsonStr["O3(ug/m3)"]
    rainfall = jsonStr["rainfall(mm)"]
    so2 = jsonStr["SO2(ug/m3)"]
    temp = jsonStr["Temp(C)"]
    lux = jsonStr["lux"]
    noise = jsonStr["noise"]
    sensorDic = {
                    "lampStatus":"FALSE",
                    "PM2.5":0,
                    "PM10":0,
                    "SO2":0,
                    "NO2":0,
                    "CO":0,
                    "O3":0,
                    "rainfall":0,
                    "noise":0,
                    "lux":0,
                    "Temp":0,
                    "Humidity":0,
                    "WindSpeed":0
                }  
    sensorDic["lampStatus"] = False
    sensorDic["PM2.5"] = pm2_5
    sensorDic["PM10"] = pm10
    sensorDic["SO2"] = so2
    sensorDic["NO2"] = no2
    sensorDic["CO"] = pm2_5
    sensorDic["O3"] = o3
    sensorDic["rainfall"] = rainfall
    sensorDic["noise"] = noise
    sensorDic["lux"] = lux
    sensorDic["Temp"] = temp
    sensorDic["Humidity"] = humidity
    sensorDic["WindSpeed"] = wind

    #print("getSensorData sensorStr:", sensorStr)
    return sensorDic


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#       This is a Thread object for reading sensor data continuously
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class SensorDataReaderThread(object):
    def __init__(self, servername, basicDataStr):
        self.interval = 15
        self.servername = servername
        self.basicDataStr = basicDataStr
        thread = threading.Thread(target=self.run, args=())
        thread.daemon = True  # Daemonize thread
        thread.start()  # Start the execution

    def run(self):

        # Try to grab a sensor reading.  Use the read_retry method which will retry up
        # to 15 times to get a sensor reading (waiting 2 seconds between each retry).
        while True:
            try:
                i = 0
                for basic in self.basicDataStr:
                    if(i > 10):
                        break
                    i = i+1
                    currentTime = calendar.timegm(time.gmtime())
                    longitude = basic["longitude"]
                    latitude = basic["latitude"]
                    lampId = basic["lampId"]
                    sensorDataDict = getSensorData(currentTime, longitude, latitude, 1)
                    sensorDataDict["lampStatus"] = mqttConnector.lampStatusDict[lampId]
                    #print("mqttConnector.lampStatusDict[lampId]", mqttConnector.lampStatusDict[lampId])
                    #print("sensorDataDict lampStatus", sensorDataDict["lampStatus"])
                    sensorDataStr = json.dumps(sensorDataDict)
                    strTopic = self.servername + '/' + DEVICETYPE + '/' + lampId + '/events'; 
                    #strTopic = self.servername + '/' + DEVICETYPE + '/' + '01010100010001' + '/events'; 
                    #strMsg = '{"lampStatus":false, "WindSpeed(km/h)":10,"PM10(ug/m3)":20,"PM2.5(ug/m3)":487,"CO(mg/m3)":65,"Humidity(%)":15,"NO2(ug/m3)":48,"O3(ug/m3)":37,"rainfall(mm)":0,"SO2(ug/m3)":48,"Temp(°C)":17,"lux":487,"noise":57}'
                    #print("topic:", strTopic)
                    #print("payload:", sensorDataStr)
                    mqttConnector.publish(strTopic, sensorDataStr)
                    time.sleep(1)
                #print 'RASPBERRY_STATS: Temp={0:0.1f}*C  Humidity={1:0.1f}%'.format(temperature, humidity)
                time.sleep(5)
            except Exception, e:
                print '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
                print "RASPBERRY_STATS: Exception in SensorDataReaderThread: Could not successfully read SensorData"
                print ("RASPBERRY_STATS: " + str(e))                
                pass
                time.sleep(self.interval)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#       This is a Thread object for connecting and subscribing to an MQTT Queue
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class SubscribeToMQTTQueue(object):
    def __init__(self, servername, access_token, refresh_token,clientKey):
        self.servername = servername
        self.access_token = access_token
        self.refresh_token = refresh_token
        self.clientKey = clientKey
        thread = threading.Thread(target=self.run, args=())
        thread.daemon = True  # Daemonize thread
        thread.start()  # Start the execution

    def run(self):
        mqttConnector.main(self.servername, self.access_token, self.refresh_token, self.clientKey)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


def main():
    username = raw_input("Please input your IOT-Server login username:")
    password = raw_input("Please input your IOT-Server login password:")
    clientKey = generateAccessToken.generateClientKey(username, password)
    access_token, refresh_token = generateAccessToken.generateToken(username, password, clientKey)
    servername = getServerName(username)
    basicDataStr = getLampBasicData();

    SubscribeToMQTTQueue(servername, access_token, refresh_token, clientKey)  # connects and subscribes to an MQTT Queue that receives MQTT commands from the server
    #connect success, register device
    SensorDataReaderThread(servername, basicDataStr)  # initiates and runs the thread to continuously read temperature from DHT Sensor
    time.sleep(2) #wait for agent to connect to broker before publishing data
    while True:
        try:
            #connectAndPushData(DEVICE_INFO)  # Push Sensor (Temperature) data to WSO2 BAM
            time.sleep(PUSH_INTERVAL)
            time.sleep(10)
        except (KeyboardInterrupt, Exception) as e:
            print "RASPBERRY_STATS: Exception in RaspberryAgentThread (either KeyboardInterrupt or Other)"
            print ("RASPBERRY_STATS: " + str(e))
            print '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
            time.sleep(10)
            pass
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


if __name__ == "__main__":
    main()
