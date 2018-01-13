/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {
    delay(1000);
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("Gibeaut-2.4", "password12");

    Serial.println();
    Serial.println();
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
}


void loop() {
    const uint16_t port = 5000;
    const char * host = "10.0.0.149"; // ip or dns

    
    
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }

    // This will send the request to the server
    client.print("{'ID':123}");

    //read back one line from server
    String line = client.readStringUntil('\r');
    client.println(line);

    Serial.println("closing connection");
    client.stop();
    
    Serial.println("wait 5 sec...");
    delay(5000);
}

