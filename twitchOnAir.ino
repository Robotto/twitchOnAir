/*
Runs on an ESP8266 (wemos D1 mini, actually), connects to the python script on $host through $tcpPort
and transmits $streamer, then waits a while for a response which is either "0\n" or "1\n" indicating whether the stream is live.
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager


//URL informationer
const char* host = "server.domain.tld"; 
const int tcpPort = 1234;


int blinkPin = D6;

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}



void setup() {
  WiFi.hostname("TwitchThing");
  pinMode(blinkPin,OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.println();


  digitalWrite(blinkPin, HIGH);
  delay(1500);
  digitalWrite(blinkPin, LOW);
  
//WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  wifiManager.setConnectTimeout(30); //try to connect to known wifis for a long time before defaulting to AP mode

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "TwitchThing"
  //and goes into a blocking loop awaiting configuration
  
  if (!wifiManager.autoConnect("TwitchThing")) {
    Serial.println("failed to connect and hit timeout");
  }

}


bool streamActive = false;
String streamer = "agcstream";

void loop() {

WiFiClient client;

  if (!client.connect(host, tcpPort)) {
    Serial.println(">>> tcp connection failed!");
    return;
  }

  client.println(streamer);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) { //wait five seconds, if no response: wait 30 seconds and return to start of loop
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(30000);
      return;
    }
  }
  
  Serial.print(">>> RX!: ");

  while(client.available()){
    String line = client.readStringUntil('\n');
    Serial.println(line);

    client.stop();
    Serial.println();
    Serial.println(">>> Disconnecting.");

    parseLine(line);

    }

    if(streamActive==true) {
      Serial.println(streamer+"'s Stream is LIVE!");
      //maybe do something with the blinkPin?
      //Go crazy with some LEDs perhaps?
    }
    else Serial.println(streamer+"'s Stream is not live..");

    

    Serial.println(">>> Sleep for 2 minutes.");
    delay(2*60*1000);  //this is bad practice.. should use millis() triggered event instead, but meh.
}


void parseLine(String line)
{
    if(line.charAt(0) == '1') streamActive=true;
    else streamActive=false;
}
  
  
