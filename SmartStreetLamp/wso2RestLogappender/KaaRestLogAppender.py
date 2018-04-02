# encoding: utf-8

from flask import Flask, render_template, request, Response
import eventlet
import json
import time
# import paho.mqtt.client as mqtt_client
from flask_mqtt import Mqtt
from flask_socketio import SocketIO
from flask_bootstrap import Bootstrap

eventlet.monkey_patch()

app = Flask(__name__)

app.config['SECRET'] = 'my secret key'
app.config['TEMPLATES_AUTO_RELOAD'] = True
app.config['MQTT_BROKER_URL'] = '192.168.200.183'
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = ''
app.config['MQTT_PASSWORD'] = ''
app.config['MQTT_KEEPALIVE'] = 60
app.config['MQTT_TLS_ENABLED'] = False
app.config['MQTT_LAST_WILL_TOPIC'] = 'home/lastwill'
app.config['MQTT_LAST_WILL_MESSAGE'] = 'bye'
app.config['MQTT_LAST_WILL_QOS'] = 2

# Parameters for SSL enabled
# app.config['MQTT_BROKER_PORT'] = 8883
# app.config['MQTT_TLS_ENABLED'] = True
# app.config['MQTT_TLS_INSECURE'] = True
# app.config['MQTT_TLS_CA_CERTS'] = 'ca.crt'



mqtt_ws = Mqtt(app)
socketio = SocketIO(app)
bootstrap = Bootstrap(app)


# 这里的初始化MQTT可以让纯Python代码运行起来
# mqtt_client = mqtt_client.Client()
# mqtt_client.connect("192.168.200.183", 1883, 60)  # 连接服务器,端口为1883,维持心跳为60秒

def Response_headers(content):
    resp = Response(content)
    resp.headers['Access-Control-Allow-Origin'] = '*'
    return resp


lampStatusDic = {}

ttimestamp = time.time()
requestcount = 0
@app.route('/v1/publishLog', methods=['POST'])
def publishLog():
    if request.method == 'POST':
        global requestcount
        global ttimestamp
        requestcount = requestcount + 1
        now = time.time()
        if (now - ttimestamp) > 1:
            print(requestcount)
            ttimestamp = now
            requestcount=0
        # POST:
        # request.form获得所有post参数放在一个类似dict类中,to_dict()是字典化
        # 单个参数可以通过request.form.to_dict().get("xxx","")获得
        # ----------------------------------------------------
        # GET:
        # request.args获得所有get参数放在一个类似dict类中,to_dict()是字典化
        # 单个参数可以通过request.args.to_dict().get('xxx',"")获得
        # datax = request.form.to_dict()
        # content = str(datax)
        # resp = Response_headers(content)
        data = request.get_data()
        #print(data)
        dataJson = json.loads(data)
        header = dataJson['header']
        applicationId = header['applicationToken']['string']
        event = dataJson['event']

        lampID = event['Lampbase']['lampGuid']

        topic = "/kaa/smartlamp/loger/%s/%s" % (applicationId, lampID)

        topicLampStatus = "/kaa/smartlamp/%s/lampStatus" % (applicationId)



        topicInfluxdb = "/kaa/smartlamp/influxdb/%s" % (applicationId)
        strLampStatus = {}
        strLampStatus['lampId'] = lampID
        strLampStatus['lampStatus'] = event['LampSelf']['lampStatus']
        strLampStatus['BulbIsOk'] = event['LampSelf']['lampStatus']
        strLampStatus['IsOnLine'] = True
        value = lampStatusDic.get(lampID)
        if value is None or value['lampStatus'] != strLampStatus['lampStatus']:
            lampStatusDic[lampID] = strLampStatus
            mqtt_ws.publish(topicLampStatus, json.dumps(strLampStatus), qos=0, retain=False)
        mqtt_ws.publish(topic, json.dumps(event), qos=0, retain=False)  # 发布消息

        #拼接成influxdb的消息 分表储存
        measurement = 'SmartLamp'+applicationId
        tags = 'lampID=%s'%(lampID)
        fieldsDic = event['LampEnvironment']
        strFields = ''
        for key in fieldsDic:
            strFields += key
            strFields += '='
            strFields += str(fieldsDic[key])
            strFields += ','
        if(len(strFields) > 0):
            strFields = strFields[0:len(strFields)-1]
        times = event['Lampbase']['timestamp']*1000000000
        influxDatas = "%s,%s %s %s" % (measurement, tags, strFields, times)
        #print(influxDatas)
        mqtt_ws.publish(topicInfluxdb, influxDatas, qos=0, retain=False)  # 发布消息
        return 'ok'
    else:
        content = json.dumps({"error_code": "1001"})
        resp = Response_headers(content)
        return resp


# 每次访问这个路由将向chat主题，提交一次数据data
@app.route('/mqtts')
def mqtts():
    # mqtt_client = myMqtt()
    mqtt_ws.publish('chat', 'data')
    return 'ok'


@app.route('/ws')
def wevsocket():
    return render_template('websocket_mqtt_demo.html')


@socketio.on('publish')
def handle_publish(json_str):
    data = json.loads(json_str)
    mqtt_ws.publish(data['topic'], data['message'])


@socketio.on('subscribe')
def handle_subscribe(json_str):
    data = json.loads(json_str)
    mqtt_ws.subscribe(data['topic'])


@mqtt_ws.on_message()
def handle_mqtt_message(client, userdata, message):
    data = dict(
        topic=message.topic,
        payload=message.payload.decode()
    )
    payload = data['payload']
    dataJson = json.loads(payload)
    var = dataJson.get('cmmand')
    #print(var)
    if var is not None and var == 'GetLampStatues':
        strtopic = message.topic
        list = strtopic.split('/')
        mqtt_ws.publish('/kaa/smartlamp/' +list[3]+ '/lampStatusInit', json.dumps(lampStatusDic))

    #socketio.emit('mqtt_message', data=data)


@mqtt_ws.on_log()
def handle_logging(client, userdata, level, buf):
    return 
    #print(client, userdata, level, buf)


if __name__ == '__main__':
    mqtt_ws.subscribe('/kaa/smartlamp/+/toServer/Cmmand')
    socketio.run(app, host='0.0.0.0', port=5001, use_reloader=True, debug=False)
