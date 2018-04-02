#!/usr/lib python
#coding:utf-8

import json
import urllib
import requests

import generateAccessToken
import registerDeviceType

REGISTER_URL = "http://192.168.7.51:8280"
LAMPBASICDATA_URL = "http://192.168.200.129:5005"

def getLampBasicData():
    headers = {'Content-Type' : 'application/x-www-form-urlencoded'}
    baseUrl = LAMPBASICDATA_URL + '/operate'
    params = 'methodinfo=get-all-tables&db_label='''
    response = requests.post(baseUrl, data = params, headers = headers)
    jsonStr = json.loads(response.content);
    #print("lampBasic:", jsonStr)
    return jsonStr

def registerDevice(access_token, deviceType, deviceName, device_identifier, property):

    #register device
    headers = { 'Authorization' : 'Bearer ' + str(access_token), 'Content-Type' : 'application/json' }

    baseUrl = REGISTER_URL + '/api/device-mgt/v1.0/device/agent/enroll'
    #print("headers:", headers)
    #print("baseUrl:", baseUrl)
    params = { "name": "", "type": "", "description": "register a new device.", "deviceIdentifier": "", "enrolmentInfo": {"ownership": "BYOD", "status": "ACTIVE", "owner": "admin"} ,"properties": property}
    params["name"] = deviceName
    params["type"] = deviceType
    params["deviceIdentifier"] = device_identifier
    response = requests.post(baseUrl, data=json.dumps(params), headers=headers);
    print("registerDevice:",response.content)
    return response.status_code

if __name__ == '__main__':

    username = raw_input("Please input your IOT-Server login username:")
    password = raw_input("Please input your IOT-Server login password:")
    deviceType = 'SmartStreetLamp'
    clientKey = generateAccessToken.generateClientKey(username, password)
    access_token, refresh_token = generateAccessToken.generateToken(username, password, clientKey)
    registerDeviceType.getRegisteredDeviceTypes(access_token)
    attributes = ['city', 'region', 'street', 'longitude', 'latitude', 'status', 'circle', 'controlWay',
                'switchBox', 'lampType', 'lampCount', 'height', 'manufacturer', 'buildDepartment', 'buildTime']
    registerDeviceType.registerDeviceType(access_token, deviceType, attributes)
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
    ret = registerDeviceType.createDeviceEventStream(access_token, deviceType, propertList)
    if ret == 200:
        jsonStr = getLampBasicData()
        j = 0
        for i in jsonStr:
            j = j+1;
            if(j > 100):
                break;
            city = i["city"]
            region = i["region"]
            street = i["street"]
            longitude = i["longitude"]
            latitude = i["latitude"]
            status = i["status"]
            circle = i["circle"]
            controlWay = i["controlWay"]
            switchBox = i["switchBox"]
            lampType = i["lampType"]
            lampCount = i["lampCount"]
            height = i["height"]
            manufacturer = i["manufacturer"]
            buildDepartment = i["buildDepartment"]
            buildTime = str(i["buildTime"]).format("yyyy-MM-dd")

            manufacturer = i["manufacturer"]
            attributes = [{'name':'city','value':city},
                {'name':'region','value':region},
                {'name':'street','value':street},
                {'name':'longitude','value':longitude},
                {'name':'latitude','value':latitude},
                {'name':'status','value':status},
                {'name':'circle','value':circle},
                {'name':'controlWay','value':controlWay},
                {'name':'switchBox','value':switchBox},
                {'name':'lampType','value':lampType},
                {'name':'lampCount','value':lampCount},
                {'name':'height','value':height},
                {'name':'manufacturer','value':manufacturer},
                {'name':'buildDepartment','value':buildDepartment},
                {'name':'buildTime','value':buildTime}]
            lampId = i["lampId"]
            lampName = street + '_' + str(i["lampId"])[10:]
            registerDevice(access_token, deviceType, lampName, lampId, attributes)
        print("register devices success!!")
    else:
        print("create device event stream failed")
