#!/user/bin/python
#coding:utf-8

from aliyunsdkcore import client

from aliyunsdkiot.request.v20180120 import RegisterDeviceRequest
from aliyunsdkiot.request.v20180120 import PubRequest

accessKeyId = 'LTAINOekTF8zHzQd'
accessKeySecret = '1dC6glETe9Bm9enHkJbmhDqXPGrdJO'

productKey = 'a1HETlEuvri'
deviceName = 'x9pCSCqctPpob36WhT7V'
pub_topic_get  = '/%s/%s/get'%(productKey, deviceName)
pub_topic_data  = '/%s/%s/data'%(productKey, deviceName)
pub_topic_usr_data = '/%s/%s/user/data'%(productKey, deviceName)
pub_topic_usr_test = '/%s/%s/user/test'%(productKey, deviceName)
send_data = '''
{
    "people": "admin"
}
'''

clt = client.AcsClient(accessKeyId, accessKeySecret, 'cn-shanghai')

request = PubRequest.PubRequest()
request.set_accept_format('json')  #设置返回数据格式，默认为XML，此例中设置为JSON
request.set_ProductKey(productKey)
request.set_TopicFullName(pub_topic_usr_test)  #消息发送到的Topic全名
request.set_MessageContent('aGVsbG8gd29ybGQ=')  #hello world Base64 String
request.set_Qos(1)
result = clt.do_action_with_exception(request)
print 'result : ' + result
