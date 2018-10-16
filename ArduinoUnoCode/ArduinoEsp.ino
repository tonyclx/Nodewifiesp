/*
 WiFiEsp example: WebClient
 This sketch connects to google website using an ESP8266 module to
 perform a simple web search.
 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/

#include "WiFiEsp.h"
#include <ArduinoJson.h>

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#endif

const int analogInPin = A0;

int sensorValue = 0;        // value read from the pot
int volt = 0;

char ssid[] = "SSID";            // your network SSID (name)
char pass[] = "1234QWER";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "host.com"; //your server host name 
String content = "id=5bc58842bdfea0153bb27214&volt="; //mongodb id and the voltge value HTTP Post to update volt value

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds


// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);

StaticJsonBuffer<200> jsonBuffer;
// Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.createObject();

  root["id"] = "5bc430df030d99113075430d";
  root["volt"] = "9 volt";
  
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial

}

void loop()
{
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  volt = map(sensorValue, 0, 1023, 0, 255);

  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

    // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    httpPostRequest();
  }
    if (millis() - lastConnectionTime > postingInterval) {
    httpGetRequest();


  ///jason parsor from ether net

// Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonBuffer jsonBuffer(capacity);

  // Parse JSON object
  JsonObject& root = jsonBuffer.parseObject(client);
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));
    return;
  }

  // Extract values
  Serial.println(F("Response:"));
  bool a =root["ledOnOff"];
  Serial.println(root["ledOnOff"].as<char*>());

  if(a == true)
  digitalWrite(LED_BUILTIN, HIGH);
  else
digitalWrite(LED_BUILTIN, LOW);
  // Disconnect
    
  }

  

}

void httpPostRequest()
{
    Serial.println();
    
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

    if (client.connectSSL(server, 443)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    String PostData = content + volt;
    Serial.println(PostData);
    client.println("POST /api/reading HTTP/1.1");
    client.println("Host: host.com"); //your server host name
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
  }
    else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
  
}

void httpGetRequest()
{
  Serial.println();
    
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
  Serial.println("Connected to server");
    // Make a HTTP request
    client.println("GET /api/light/5bc431bf91a04e11c13ab5d7 HTTP/1.1");
    client.println("Host: morning-cliffs-85779.herokuapp.com");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}