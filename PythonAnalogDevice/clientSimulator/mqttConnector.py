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
REGISTER_URL = "http://192.168.7.51:8280"
CONNECTED = False

global lampStatusDict

lampStatusDict = {
        "01010100010001":False,
        "01010100010002":False,
        "01010100010003":False,
        "01010100010004":False,
        "01010100010005":False,
        "01010100010006":False,
        "01010100010007":False,
        "01010100010008":False,
        "01010100010009":False,
        "01010100010010":False,
        "01010100010011":False
}

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
        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        TOPIC_TO_SUBSCRIBE = SERVERNAME + '/' + DEVICETYPE + '/+/operation/#'
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
    print '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    print 'MQTT_LISTENER: Message Received by Device'
    print( "TOPIC is: ", msg.topic )
    print("payload is: ", str(msg.payload))
    lampId = msg.topic.split('/')[2]
    if msg.payload.upper() == "ON":
        print("BULB Switched ON")
        lampStatusDict[lampId] = True
    if msg.payload.upper() == "OFF":
        print("BULB Switched OFF")
        lampStatusDict[lampId] = False
    print("lampStatusDict:", lampStatusDict)


def on_publish(client, userdata, mid):
    print '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
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
    #print("userData:", userdata)
    #print("rc:",rc)
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


    global mqttClient
    mqttClient = mqtt.Client(client_id='ClientSimulator')
    mqttClient.on_connect = on_connect
    mqttClient.on_message = on_message
    mqttClient.on_publish = on_publish
    #mqttClient.on_subscribe = on_subscribe
    mqttClient.on_disconnect = on_disconnect
    mqttClient.username_pw_set(access_token, password = "")

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
