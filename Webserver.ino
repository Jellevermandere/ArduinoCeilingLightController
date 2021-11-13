
void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void SetUpServer()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                      // you're connected now, so print out the status
}

void LoopServer(){
    WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";
    bool gotRequest = false;                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          Serial.println(" ");
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");

            if(!gotRequest){
              client.println("Content-type:text/html");
              client.println();

              // the content of the HTTP response follows the header:
              client.print( MAIN_page);
              client.print( MAIN_page0);
              client.print( MAIN_page1);
              client.print( MAIN_page2);
              client.println(MAIN_page3);
            }

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /C" or "GET /W":
        if (currentLine.endsWith("/C")) {
          TurnOffWhite();
          //parse the rgb data   
          ControlRGBWithText(currentLine);
          gotRequest = true;
          //break;
                    
        }
        if (currentLine.endsWith("/W")) {
          TurnOffRGB();
          // parse the white led data
          ControlWhiteWithText(currentLine);
          gotRequest = true;
          //break;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void sendPage(WiFiClient &client)
{
  // Serial.println("[server] 200 response send");
  client.println( MAIN_page);

  client.stop();
}


void send404(WiFiClient &client)
{
  // Serial.println("[server] response 404 file not found");
  client.println("HTTP/1.0 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"  // simple plain text without html tags
                 "\r\n"
                 "File Fot Found");
}


void send204(WiFiClient &client)
{
  // Serial.println("[server] response 204 no content");
  client.println("HTTP/1.0 204 no content\r\n"); // no content
}
