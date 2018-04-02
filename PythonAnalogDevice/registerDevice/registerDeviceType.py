#!/usr/bin python
#coding:utf-8

import json
import urllib
import requests
import generateAccessToken


REGISTER_URL = "http://192.168.7.51:8280"

def getRegisteredDeviceTypes(access_token):
    typeList = []
    headers = { 'Authorization' : 'Bearer ' + str(access_token), 'Content-Type' : 'application/json' }
    baseUrl = REGISTER_URL + '/api/device-mgt/v1.0/admin/device-types'
    response = requests.get(baseUrl, headers=headers);
    types = json.loads(response.content)
    for i in types:
        type = i["name"]
        typeList.append(type);
    print("typeList:",typeList)
    return typeList;
    
def registerDeviceType(access_token, deviceType, attributes):

    print("access_token:",access_token)
    headers = { 'Authorization' : 'Bearer ' + str(access_token), 'Content-Type' : 'application/json' }

    baseUrl = REGISTER_URL + '/api/device-mgt/v1.0/admin/device-types'
    #print("headers:", headers)
    #print("baseUrl:", baseUrl)
    params = {"name": deviceType,"deviceTypeMetaDefinition": {"properties": attributes, "features": [{"code": "bulb","name": "control bulb","description": "on the bulb"},{"code": "ring","name": "ring","description": "Alarm coming,trigger ring"},{"code": "reboot","name": "device reboot","description": "reboot the device"}],"pushNotificationConfig": {"type": "MQTT","scheduled": 'false'},"description": "this is a new smart street lamp", "initialOperationConfig": {"operations": ["bulb"]}}}
    print("params:",params)
     #params["name"] = deviceType
    response = requests.post(baseUrl, data=json.dumps(params), headers=headers);
    print("register deviceType:", response)
    return response.status_code


def createDeviceEventStream(access_token, deviceType, propertyList):
    headers = { 'Authorization' : 'Bearer ' + str(access_token), 'Content-Type' : 'application/json' }
    baseUrl = REGISTER_URL + '/api/device-mgt/v1.0/events/' + deviceType
    #print("headers:", headers)
    #print("baseUrl:", baseUrl)

    params = {"eventAttributes": {"attributes": propertyList},"transport": "MQTT"}
    #attributes = params["eventAttributes"]
    #attributes["attributes"] = propertyList
    #print("params:", params)
    response = requests.post(baseUrl, data=json.dumps(params), headers=headers);
    print("createDeviceEventStream:", response)
    return response.status_code

    
if __name__ == '__main__':
    username = raw_input("Please input your IOT-Server login username:")
    password = raw_input("Please input your IOT-Server login password:")
    deviceType = 'SmartStreetLamp'
    clientKey = generateAccessToken.generateClientKey(username, password)
    access_token, refresh_token = generateAccessToken.generateToken(username, password, clientKey)
    getRegisteredDeviceTypes(access_token)
    attributes = ['city', 'region', 'street', 'longitude', 'latitude', 'status', 'circle', 'controlWay',
                'switchBox', 'lampType', 'lampCount', 'height', 'manufacturer', 'buildDepartment', 'buildTime']
    registerDeviceType(access_token, deviceType, attributes)
    propertList = [{"name": "lampStatus", "type": "BOOL", },
                    {"name": "WindSpeed", "type": "FLOAT", },
                    {"name": "PM10", "type": "FLOAT",},
                    {"name": "PM2.5", "type": "FLOAT",},
                    {"name": "CO", "type": "FLOAT",},
                    {"name": "Humidity", "type": "FLOAT",},
                    {"name": "NO2", "type": "FLOAT",},
                    {"name": "O3", "type": "FLOAT",},
                    {"name": "rainfall", "type": "FLOAT",},
                    {"name": "SO2", "type": "FLOAT",},
                    {"name": "Temp", "type": "FLOAT",},
                    {"name": "lux", "type": "FLOAT",},
                    {"name": "noise", "type": "FLOAT",}]
    createDeviceEventStream(access_token, deviceType, propertList)
