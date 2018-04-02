#!/usr/bin/env python
# -*- coding:utf-8 -*-
#import logging
import sys
import json
from pymongo import MongoClient
#!flask/bin/python
from flask import Flask, request
app = Flask(__name__)
settings = {
    "ip":'192.168.200.140',   #ip
    "port":27017,           #端口
    "db_name" : "kaa",    #数据库名字
    "set_name" : "logs_17812572182164569343"   #集合名字
}
class MyMongoDB(object):
    def __init__(self):
        try:
            self.conn = MongoClient(settings["ip"], settings["port"])
        except Exception as e:
            print(e)
        self.db = self.conn[settings["db_name"]]
        self.my_set = self.db[settings["set_name"]]
    def dbfind(self, beginTimestamp, endTimestamp, lampID, valueType):
        #data = self.my_set.find({"header.timestamp.long": {"$gte": beginTimestamp, "$lte": endTimestamp}})
        strFilter = "event.LampEnvironment.%s" %(valueType)
        data = self.my_set.find({"header.timestamp.long": {"$gte": beginTimestamp, "$lte": endTimestamp}, "event.Lampbase.lampGuid":lampID},
                                {strFilter:1, 'event.Lampbase.timestamp':1})
        return data

mongo = MyMongoDB()

allData = {}
hisData = []


@app.route('/v1/GetHistoryData')
def api_GetHistoryData():
    #if 'beginTimestamp' in request.args and 'endTimestamp' in request.args:
        #beginTistamp = int(request.args['beginTimestamp']);
        #endTistamp = int(request.args['endTimestamp']);
        #print(type(beginTistamp))
        #mongo.dbfind(beginTistamp, endTistamp)
    #    return ''
    #else:        return 'bad'
    try:
        beginTistamp = request.args.get('beginTimestamp')
        endTistamp = request.args.get('endTimestamp')
        lampID = request.args.get('lampID')
        valueType = request.args.get('valueType')
        if beginTistamp is not None and endTistamp is not None:
            data = mongo.dbfind(int(beginTistamp), int(endTistamp), lampID, valueType)
            hisData.clear()
            allData["res"] = data.count()
            #preHisData['res'] = data.count()
            for res in data:
                #print(res)
                #preHisData['timestamp'] = res
                preHisData = {}
                event = res['event']
                Lampbase = event['Lampbase']
                LampEnvironment = event['LampEnvironment']

                preHisData['value'] = LampEnvironment[valueType]
                preHisData['timestamp'] = Lampbase['timestamp']
                #print(preHisData)
                hisData.append((preHisData))
            allData["evetn"] = hisData
            allDatajson = json.dumps(allData)
            return allDatajson
        return 'jjjjj'
    except:
        return 'yichang'
if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
    #beginTistamp = '1316759311515'
    #endTistamp = '1816759311515'
    #data = mongo.dbfind(int(beginTistamp), int(endTistamp),'01010100010001','rainFull')
    #for res in data:
    #    print(res)




