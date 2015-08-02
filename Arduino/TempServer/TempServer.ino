#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>

#include <DallasTemperature.h>

#include <SPI.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = { 
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
// assign an IP address for the controller:
IPAddress ip(192,168,1,209);
IPAddress gateway(192,168,1,1);	
IPAddress subnet(255, 255, 255, 0);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  //Serial.begin(9600);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  
  oneWireSetup();
  
  // give the sensor and Ethernet shield time to set up:
  delay(3000);
}

void loop() { 
  // listen for incoming Ethernet connections:
  listenForEthernetClients();
}


void getData() {

}

void listenForEthernetClients() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Got a client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          // print the current readings, in HTML format:
          client.print("Indoor.value ");
          client.print(getOneWireTempF(0));
          client.print("<br />");
          client.print("Outdoor.value ");
          client.print(getOneWireTempF(1));
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1000);
    // close the connection:
    client.stop();
  }
}
