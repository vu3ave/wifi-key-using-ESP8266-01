#include <ArduinoHttpClient.h>


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "GNXS-58E180"; // Enter your WiFi name
const char *password = "anish7776";  // Enter WiFi password

//const char *ssid = "Anish"; // Enter your WiFi name
//const char *password = "12341234";  // Enter WiFi password

int mute =0;


// MQTT Broker
 //const char *mqtt_broker = "iot.eclipse.org";
const char *mqtt_broker = "test.mosquitto.org";

const char *topic = "esp8266/callsign";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

// http://radiobox.scienceontheweb.net/straightkey.html for sending Morse KEY

// Assign output variables to GPIO pins
const int keyerpin = 2; // pin 1 (RX Pin) of esp8266-01 wifi module, where a Blue built in LED is connected onboard
const int tonepin = 0; // pin 0 of esp8266-01 wifi module



WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

 // Initialize the output variables as outputs
  pinMode(keyerpin, OUTPUT);
  pinMode(tonepin, OUTPUT);
  
  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public mosquito mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // publish and subscribe
    client.publish(topic, "u");
    client.subscribe(topic);
// client.publish(topic, "hello emqx");
  //client.subscribe(topic);


  
}

void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
Serial.println(topic);
Serial.println();
if((char) payload[0] =='1'){
  
 digitalWrite(keyerpin, HIGH);
 
if(mute == 0){
digitalWrite(tonepin, HIGH);
}
  
  }

if((char) payload[0] =='0'){
  
  digitalWrite(keyerpin, LOW);
  digitalWrite(tonepin, LOW); 
  

  }


if((char) payload[0] =='2'){
  
mute = 1;  


  }


if((char) payload[0] =='3'){
  
mute = 0;  


  }
  

}

void loop() {
  client.loop();
}
