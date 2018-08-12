
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <SoftwareSerial.h>

//SoftwareSerial NodeSerial(D2,D3);  // RX, TX 
String a;
// Update these with values suitable for your network.

const char* ssid = "Bah";
const char* password = "5133738457";
const char* mqtt_server = "18.218.139.116";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);   
 // pinMode(D2, INPUT);
  //pinMode(D3, OUTPUT);
  Serial.begin(115200);
 // NodeSerial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
   // Serial.write((char)payload[i]); 
    message += ((char)payload[i]);
    
  }
  Serial.write(message.c_str());
  Serial.write('\r'); 
  Serial.write('\n'); 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("/Scenes");
      client.subscribe("/Security");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
while(Serial.available()) {

a= Serial.readString();// read the incoming data as string

Serial.println(a);
client.publish("pic24ep", a.c_str());
}
}
