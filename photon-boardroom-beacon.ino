#include "SparkJson.h"
#include "MQTT.h"

boolean test = true;
int testLed = D7;
int led1[] = {D2, D1, D0}; //rgb
int led2[] = {D3, D4, D5};
int led3[] = {A0, A1, A2};
String slot1Satus = "";
String slot2Satus = "";
String slot3Satus = "";
boolean slot1HasRecievedMessage = false;
boolean slot2HasRecievedMessage = false;
boolean slot3HasRecievedMessage = false;
boolean isConnected = false;

// Log message to cloud, message is a printf-formatted string
void debug(String message, int value) {
    char msg [50];
    sprintf(msg, message.c_str(), value);
    Spark.publish("DEBUG", msg);
    Serial.println(msg);
}

// MQTT
// url structure: /homeassistant/boardroomBeacon/<roomID>/slot/<slotID>/status/<[booked, free, illegal]>
// mosquitto_pub -h 192.168.2.115 -p 8883 -t 'homeassistant/boardroomBeacon/biggie/slot/1' -u pi -P welcome -m 'biggeeeee'
// mosquitto_pub -h 192.168.2.115 -p 8883 -t 'home-assistant/test' -u pi -P welcome -m 'BAm'
void callback(char* topic, byte* payload, unsigned int length);
byte server[] = { 192,168,2,115 };
int port = 8883;
String usr = "pi";
String pwd = "welcome";
String slot1Topic = "homeassistant/boardroomBeacon/biggie/slot/0";
String slot2Topic = "homeassistant/boardroomBeacon/biggie/slot/1";
String slot3Topic = "homeassistant/boardroomBeacon/biggie/slot/2";
MQTT client(server, port, callback);
// for QoS2 MQTTPUBREL message. this messageid maybe have store list or array structure.
uint16_t qos2messageid = 0;

// recieve message
void callback(char* topic, byte* payload, unsigned int length) {

  // string message, don't parse
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.println("message: "+message);

  String top(topic);
  Serial.println("topic: "+top);
  top.trim();

  if(top.endsWith("0")){
    /*Serial.println('ENDS WITH 0!!!');*/
    slot1HasRecievedMessage = true;
    setStatus(led1, message);
  } else if (top.endsWith("1")){
    slot2HasRecievedMessage = true;
    setStatus(led2, message);
  } else if (top.endsWith("2")){
    slot3HasRecievedMessage = true;
    setStatus(led3, message);
  } else {
    Serial.println("No Match!");
  }

  // Parse json payload.
  /*StaticJsonBuffer<200> jsonBuffer;
  JsonObject& command = jsonBuffer.parseObject((char*)payload);
  Serial.println("Command received:");
  command.printTo(Serial);
  Serial.println();*/

}

// QOS ack callback.
// MQTT server sendback message id ack, if use QOS1 or QOS2.
void qoscallback(unsigned int messageid) {
    Serial.print("Ack Message Id:");
    Serial.println(messageid);

    if (messageid == qos2messageid) {
        Serial.println("Release QoS2 Message");
        client.publishRelease(qos2messageid);
    }
}

void setup() {

    Serial.begin(9600);

    /*RGB.control(true);*/
    pinMode(testLed, OUTPUT);

    // Set up our pins for output
    pinMode( led1[0], OUTPUT);
    pinMode( led1[1], OUTPUT);
    pinMode( led1[2], OUTPUT);

    pinMode( led2[0], OUTPUT);
    pinMode( led2[1], OUTPUT);
    pinMode( led2[2], OUTPUT);

    pinMode( led3[0], OUTPUT);
    pinMode( led3[1], OUTPUT);
    pinMode( led3[2], OUTPUT);

    // initializing/connecting - no previous state cached and not connected to mqtt server yet.
    digitalWrite( led1[0], HIGH);
    digitalWrite( led1[1], LOW);
    digitalWrite( led1[2], HIGH);

    digitalWrite( led2[0], HIGH);
    digitalWrite( led2[1], LOW);
    digitalWrite( led2[2], HIGH);

    digitalWrite( led3[0], HIGH);
    digitalWrite( led3[1], LOW);
    digitalWrite( led3[2], HIGH);

    // add qos callback. If don't add qoscallback, ACK message from MQTT server is ignored.
    client.addQosCallback(qoscallback);

}

void connect() {
    Serial.print("Connecting to MQTT...");
    while(!client.isConnected()) {
        client.connect("sparkclient", usr, pwd);
        if(client.isConnected()) {
            Serial.println("connected!");
            client.subscribe(slot1Topic);
            client.subscribe(slot2Topic);
            client.subscribe(slot3Topic);
        } else {
            Serial.print(".");
            delay(500);
        }
    }
}

void waiting(int* led){
  digitalWrite( led[0], LOW);
  digitalWrite( led[1], LOW);
  digitalWrite( led[2], HIGH);
  delay(250);
  digitalWrite( led[0], LOW);
  digitalWrite( led[1], LOW);
  digitalWrite( led[2], LOW);
  delay(100);
}

void loop() {

  if (!client.isConnected()) {
    digitalWrite(testLed, LOW);
    connect();
  } else {
    digitalWrite(testLed, HIGH);
    if(!slot1HasRecievedMessage){
      waiting(led1);
    }
    if (!slot2HasRecievedMessage){
      waiting(led2);
    }
    if (!slot3HasRecievedMessage){
      waiting(led3);
    }
  }
  // Loop the MQTT client.
  client.loop();
}

// LED Logic
int setStatus(int* led, String status){
  int red = led[0];
  int green = led[1];
  int blue = led[2];
  /*Serial.println(status);*/
  /*debug("status = %d", status);*/
  if(status == "free"){
    Serial.println('free');
    red = LOW;
    green = HIGH;
    blue = LOW;
  } else if (status == "booked") {
    red = HIGH;
    green = LOW;
    blue = LOW;
    Serial.println('booked');
  } else if (status == "illegal") {
    red = 150;
    green = 150;
    blue = LOW;
    Serial.println('illegal');
  } else if (status == "error") {
    red = HIGH;
    green = HIGH;
    blue = HIGH;
  } else if (status == "disconnected") {
    red = HIGH;
    green = HIGH;
    blue = HIGH;
  } else if (status == "waiting") {
    red = LOW;
    green = LOW;
    blue = HIGH;
  } else if (status == "stop") {
    red = LOW;
    green = LOW;
    blue = LOW;
  }
  /*analogWrite( led[0], red);
  analogWrite( led[1], green);
  analogWrite( led[2], blue);*/
  digitalWrite( led[0], red);
  digitalWrite( led[1], green);
  digitalWrite( led[2], blue);
  /*debug("status = %d", status);*/
  return 1;
}
