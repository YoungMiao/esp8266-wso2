#!/usr/bin/env python

import urllib
import requests
import json
from base64 import encodestring

REGISTER_URL = "http://192.168.7.51:8280"

def post(url, payload, basestr, type):
    headers = { 'Authorization' : 'Basic ' + basestr, 'Content-Type' : type }
    baseUrl = REGISTER_URL + url
    #print("headers:", headers)
    #print("baseUrl:", baseUrl)
    response = requests.post(baseUrl, data=payload, headers=headers);
    #print("response:",response)
    return response
    
def generateClientKey(username, password):
    basestr = encodestring('%s:%s' % (username,password))[:-1]
    print("basestr:", basestr)

    #send POST request to generate CLIENT_ID and CLIENT_SECRET
    params = {  
        "applicationName": "DefaultApplication",  
        "tags": ["device_management"]  
    }  
    data = post("/api-application-registration/register", json.dumps(params), basestr, 'application/json')
    response = json.loads(data.content)
    print("response:", data.content)
    CLIENT_ID = response["client_id"]
    CLIENT_SECRET = response["client_secret"]

    #use CLIENT_ID and CLIENT_SECRET to generate CLIENT_KEY
    CLIENT_KEY = encodestring('%s:%s' % (CLIENT_ID, CLIENT_SECRET))[:-1]
    print("client_id:", CLIENT_ID)
    print("client_secret:", CLIENT_SECRET)
    print("CLIENT_KEY:", CLIENT_KEY)
    return CLIENT_KEY

def generateToken(username, password, clientKey ):

    #send POSTrequest to generate ACCESS_TOKEN
    strPara = "grant_type=password&username={0}&password={1}&scope=perm:sign-csr perm:admin:devices:view perm:admin:topics:view perm:roles:add perm:roles:add-users perm:roles:update perm:roles:permissions perm:roles:details perm:roles:view perm:roles:create-combined-role perm:roles:delete perm:dashboard:vulnerabilities perm:dashboard:non-compliant-count perm:dashboard:non-compliant perm:dashboard:by-groups perm:dashboard:device-counts perm:dashboard:feature-non-compliant perm:dashboard:count-overview perm:dashboard:filtered-count perm:dashboard:details perm:get-activity perm:devices:delete perm:devices:applications perm:devices:effective-policy perm:devices:compliance-data perm:devices:features perm:devices:operations perm:devices:search perm:devices:details perm:devices:update perm:devices:view perm:view-configuration perm:manage-configuration perm:policies:remove perm:policies:priorities perm:policies:deactivate perm:policies:get-policy-details perm:policies:manage perm:policies:activate perm:policies:update perm:policies:changes perm:policies:get-details perm:users:add perm:users:details perm:users:count perm:users:delete perm:users:roles perm:users:user-details perm:users:credentials perm:users:search perm:users:is-exist perm:users:update perm:users:send-invitation perm:admin-users:view perm:groups:devices perm:groups:update perm:groups:add perm:groups:device perm:groups:devices-count perm:groups:remove perm:groups:groups perm:groups:groups-view perm:groups:share perm:groups:count perm:groups:roles perm:groups:devices-remove perm:groups:devices-add perm:groups:assign perm:device-types:features perm:device-types:types perm:applications:install perm:applications:uninstall perm:admin-groups:count perm:admin-groups:view perm:notifications:mark-checked perm:notifications:view perm:admin:certificates:delete perm:admin:certificates:details perm:admin:certificates:view perm:admin:certificates:add perm:admin:certificates:verify perm:admin perm:devicetype:deployment perm:device-types:events perm:device-types:events:view perm:admin:device-type perm:device:enroll perm:geo-service:analytics-view perm:geo-service:alerts-manage perm:sign-csr perm:admin:devices:view perm:roles:add perm:roles:add-users perm:roles:update perm:roles:permissions perm:roles:details perm:roles:view perm:roles:create-combined-role perm:roles:delete perm:get-activity perm:devices:delete perm:devices:applications perm:devices:effective-policy perm:devices:compliance-data perm:devices:features perm:devices:operations perm:devices:search perm:devices:details perm:devices:update perm:devices:view perm:view-configuration perm:manage-configuration perm:policies:remove perm:policies:priorities perm:policies:deactivate perm:policies:get-policy-details perm:policies:manage perm:policies:activate perm:policies:update perm:policies:changes perm:policies:get-details perm:users:add perm:users:details perm:users:count perm:users:delete perm:users:roles perm:users:user-details perm:users:credentials perm:users:search perm:users:is-exist perm:users:update perm:users:send-invitation perm:admin-users:view perm:groups:devices perm:groups:update perm:groups:add perm:groups:device perm:groups:devices-count perm:groups:remove perm:groups:groups perm:groups:groups-view perm:groups:share perm:groups:count perm:groups:roles perm:groups:devices-remove perm:groups:devices-add perm:groups:assign perm:device-types:features perm:device-types:types perm:applications:install perm:applications:uninstall perm:admin-groups:count perm:admin-groups:view perm:notifications:mark-checked perm:notifications:view perm:admin:certificates:delete perm:admin:certificates:details perm:admin:certificates:view perm:admin:certificates:add perm:admin:certificates:verify perm:ios:enroll perm:ios:view-device perm:ios:apn perm:ios:ldap perm:ios:enterprise-app perm:ios:store-application perm:ios:remove-application perm:ios:app-list perm:ios:profile-list perm:ios:lock perm:ios:enterprise-wipe perm:ios:device-info perm:ios:restriction perm:ios:email perm:ios:cellular perm:ios:applications perm:ios:wifi perm:ios:ring perm:ios:location perm:ios:notification perm:ios:airplay perm:ios:caldav perm:ios:cal-subscription perm:ios:passcode-policy perm:ios:webclip perm:ios:vpn perm:ios:per-app-vpn perm:ios:app-to-per-app-vpn perm:ios:app-lock perm:ios:clear-passcode perm:ios:remove-profile perm:ios:get-restrictions perm:ios:wipe-data perm:admin perm:android:enroll perm:android:wipe perm:android:ring perm:android:lock-devices perm:android:configure-vpn perm:android:configure-wifi perm:android:enroll perm:android:uninstall-application perm:android:manage-configuration perm:android:location perm:android:install-application perm:android:mute perm:android:change-lock-code perm:android:blacklist-applications perm:android:set-password-policy perm:android:encrypt-storage perm:android:clear-password perm:android:enterprise-wipe perm:android:info perm:android:view-configuration perm:android:upgrade-firmware perm:android:set-webclip perm:android:send-notification perm:android:disenroll perm:android:update-application perm:android:unlock-devices perm:android:control-camera perm:android:reboot perm:android:logcat perm:device:enroll perm:device:disenroll perm:device:modify perm:device:operations perm:device:publish-event".format(username, password)

    params = strPara
    data = post("/token", params, clientKey, 'application/x-www-form-urlencoded')
    response = json.loads(data.content)
    access_token = response["access_token"]
    refresh_token = response["refresh_token"]
    print("generateToken accessToken:", access_token)
    print("generateToken refreshToken:", refresh_token)
    return [access_token, refresh_token]
    
def refreshToken(refresh_token, clientKey):

    params = urllib.urlencode({"grant_type": "refresh_token", "refresh_token": refresh_token,
                               "scope": "Enroll device"})
    data = post("/token", params, clientKey, 'application/x-www-form-urlencoded')
    response = json.loads(data.content)
    access_token = response["access_token"]
    refresh_token = response["refresh_token"]
    print("refreshToken access_token:", access_token)
    print("refreshToken refresh_token:", refresh_token)
    return access_token

if __name__ == '__main__':
    username = raw_input("Please input your IOT-Server login username:")
    password = raw_input("Please input your IOT-Server login password:")
    clientKey = generateClientKey(username, password)
    access_token, refresh_token = generateToken(username, password, clientKey)
    #refreshToken(refresh_token, clientKey)

