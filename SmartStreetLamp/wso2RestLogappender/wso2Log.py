#!/usr/bin/env python
#coding:utf-8

import time
import paho.mqtt.client as mqtt
import generateAccessToken
import requests
import sys, os, json

MQTT_HOST = "192.168.7.51"
MQTT_PORT = 1886
DEVICETYPE = "SmartStreetLamp"

CONNECTED = False

def getServerName(username):
    if(username.find("@") == -1):
        return "carbon.super"
    else:
        return (username.split("@"))[1]

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        global CONNECTED
        CONNECTED = True
        TOPIC_TO_SUBSCRIBE = SERVERNAME + '/' + DEVICETYPE + '/+/events'
        print ("MQTT_LISTENER: Subscribing with topic " + TOPIC_TO_SUBSCRIBE)
        client.subscribe(TOPIC_TO_SUBSCRIBE)
    elif rc == 4:
        newAccessToken = generateAccessToken.refreshToken(REFRESH_TOKEN, CLIENTKEY)
        client.username_pw_set(newAccessToken, password="")
    else:
        global CONNECTED
        CONNECTED = False

    print("MQTT_LISTENER: Connected with result code " + str(rc))
  
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    #print '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'

    print 'MQTT_LISTENER: Message Received by Device'
    print( "TOPIC is: ", msg.topic )
    print("payload is: ", str(msg.payload))
    kaaStructDic = {
                "header":{
                    "applicationToken":{"string":"000000"}
                    },
                "event":{
                    "Lampbase":{
                        "lampGuid":"",
                        "lampLongitude":0,
                        "lampLatitude":0,
                        "timestamp":""
                    },
                    "LampSelf":{
                        "lampStatus":False,
                        "BulbIsOk": False,
                        "lampAngle": 0,
                        "lampLuxValue": 0,
                        "lampLeakageValue":0
                    },
                    "LampEnvironment":{
                        "pm2_5":0,
                        "pm10":0,
                        "So2":0,
                        "No2":0,
                        "Co":0,
                        "O3":0,
                        "rainFull":0,
                        "noise":0,
                        "lux":0,
                        "temperature":0,
                        "humidity":0,
                        "windspeed":0
                    }
                    }
                }    
    msgDic = json.loads(msg.payload)
    kaaStructDic['event']['LampEnvironment']['pm2_5']=msgDic['PM2.5']
    kaaStructDic['event']['LampEnvironment']['pm10']=msgDic['PM10']
    kaaStructDic['event']['LampEnvironment']['So2']=msgDic['SO2']
    kaaStructDic['event']['LampEnvironment']['No2']=msgDic['NO2']
    kaaStructDic['event']['LampEnvironment']['Co']=msgDic['CO']
    kaaStructDic['event']['LampEnvironment']['O3']=msgDic['O3']
    kaaStructDic['event']['LampEnvironment']['rainFull']=msgDic['rainfall']
    kaaStructDic['event']['LampEnvironment']['noise']=msgDic['noise']
    kaaStructDic['event']['LampEnvironment']['lux']=msgDic['lux']
    kaaStructDic['event']['LampEnvironment']['temperature']=msgDic['Temp']
    kaaStructDic['event']['LampEnvironment']['humidity']=msgDic['Humidity']
    kaaStructDic['event']['LampEnvironment']['windspeed']=msgDic['WindSpeed']
    
    
    kaaStructDic['event']['LampSelf']['lampStatus']=msgDic['lampStatus']
    
    
    print(msg.topic)
    strList = msg.topic.split('/')
    kaaStructDic['event']['Lampbase']['lampGuid'] =  strList[2]
    print("lampId:",strList[2])
    #kaaStructDic['event']['Lampbase']['lampGuid']=msgDic['WindSpeed(km/h)']
    kaaStructDic['event']['Lampbase']['timestamp'] = int(time.time())
    
    
    requrl = 'http://192.168.200.183:5001/v1/publishLog'
    #req = urllib2.Request(url = requrl,data =json.dumps(kaaStructDic))
    #print ("req:", req)

    #res_data = urllib2.urlopen(req)
    #res = res_data.read()
    #print ("res:", res)
    #print("json:", json.dumps(kaaStructDic))
    
    #headers = {'Content-Type' : 'application/json'}
    response = requests.post(requrl, data = json.dumps(kaaStructDic))
    print("response:", response.content)


#def on_publish(client, userdata, mid):
    #$print '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    #print 'sensor Data Published Succesfully'
    #print (client)
    #print (userdata)
    #print (mid)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#       The callback for when a PUBLISH message to the server when door is open or close
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def publish(topic, msg):
    if CONNECTED:
        print '~~~~~~~~~~~~~~~~~~~~~~~~ Publishing Device-Data ~~~~~~~~~~~~~~~~~~~~~~~~~'
        #print ('PUBLISHED TOPIC: ' + topic)
        #print ('PUBLISHED DATA: ' + msg)
        global mqttClient
        mqttClient.publish(topic, msg)
    #else:
        #print("disconnection, cannot publish data.")

def on_subscribe(client, userdata, mid, granted_qos):
    print " subscribe Successfully. " 
    #+ TOPIC_TO_SUBSCRIBE

def on_disconnect(client, userdata, rc):
    global CONNECTED
    CONNECTED = False
    print ("Agent disconnected from broker")
    global CLIENTKEY
    global REFRESH_TOKEN
    print("Obtaining new access token")
    #print("REFRESH_TOKEN", REFRESH_TOKEN)
    #print("CLIENTKEY", CLIENTKEY)
    newAccessToken = generateAccessToken.refreshToken(REFRESH_TOKEN, CLIENTKEY)
    client.username_pw_set(newAccessToken, password="")


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#       The Main method of the server script
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def main(servername, access_token, refresh_token, clientKey):
    global CLIENTKEY
    CLIENTKEY = clientKey
    global REFRESH_TOKEN
    REFRESH_TOKEN = refresh_token

    global SERVERNAME
    SERVERNAME = servername
    #print(CLIENTKEY)
    #print(REFRESH_TOKEN)
    #print(ACCESS_TOKEN)
    #print(SERVERNAME)

    global mqttClient
    mqttClient = mqtt.Client(client_id='WSO2LogClient')
    mqttClient.on_connect = on_connect
    mqttClient.on_message = on_message
    #mqttClient.on_publish = on_publish
    #mqttClient.on_subscribe = on_subscribe
    mqttClient.on_disconnect = on_disconnect
    mqttClient.username_pw_set(access_token, password = "")
    mqttClient.connect(MQTT_HOST, MQTT_PORT, 60)
   
    while True:
        try:
            mqttClient.connect(MQTT_HOST, MQTT_PORT, 180)
            print "MQTT_LISTENER: " + time.asctime(), "Connected to MQTT Broker - %s:%s" % (MQTT_HOST, MQTT_PORT)
    
            # Blocking call that processes network traffic, dispatches callbacks and
            # handles reconnecting.
            # Other loop*() functions are available that give a threaded interface and a
            # manual interface.
            mqttClient.loop_forever()
    
        except (KeyboardInterrupt, Exception) as e:
            print "MQTT_LISTENER: Exception in MQTTServerThread (either KeyboardInterrupt or Other)"
            print ("MQTT_LISTENER: " + str(e))
    
            mqttClient.disconnect()
            print "MQTT_LISTENER: " + time.asctime(), "Connection to Broker closed - %s:%s" % (MQTT_HOST, MQTT_PORT)
            print '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
            pass
    

if __name__ == '__main__':
    username = raw_input("Please input your IOT-Server login username:")
    password = raw_input("Please input your IOT-Server login password:")
    clientKey = generateAccessToken.generateClientKey(username, password)
    access_token, refresh_token = generateAccessToken.generateToken(username, password, clientKey)
    servername = getServerName(username)
    main(servername, access_token, refresh_token, clientKey)
