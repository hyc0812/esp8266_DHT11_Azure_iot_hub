#include "ESP8266WiFi.h"
#include "PubSubClient.h"

const char* ssid = "SHAW-B7C7"; //Wifi SSID
const char* password = "cellar6498badge"; //WIFI password
const char* mqtt_server = "FleetManagerHub-yong.azure-devices.net"; //MQTT Broker address
const int mqtt_port = 8883; //MQTT broker port
const char* deviceName = "DHT11_temperature_humidity_sensor";
const char* deviceSAS = "SharedAccessSignature sr=FleetManagerHub-yong.azure-devices.net%2Fdevices%2FDHT11_temperature_humidity_sensor&sig=P4aZ6hpd3kCeXqKpr6aL3SEkCBDABuY%2Bot9wHi%2FX1wU%3D&se=1691547056"";

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String hubUserString=mqtt_server;
    hubUserString+="/";
    hubUserString+=deviceName;

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(deviceName, hubUserString.c_str(), deviceSAS)) {
      Serial.println("Connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    connect();
  }

  client.loop();
  delay(5000);
}

