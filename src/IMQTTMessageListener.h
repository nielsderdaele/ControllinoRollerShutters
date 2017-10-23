#ifndef IMQTTMESSAGELISTENER_H
#define IMQTTMESSAGELISTENER_H

class IMQTTMessageListener  {
  public:
    virtual ~IMQTTMessageListener() {}
    virtual void onMQTTMessageReceived(const String&,const String&) = 0;     
};


#endif

