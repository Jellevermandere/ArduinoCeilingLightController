void ControlWithSerial(){
  
  while(Serial.available()){
    String data = Serial.readString();
    Serial.println(data);
    // Check to see if the client request was "/C" or "/W":
    if (data.indexOf("/C") >0) {
      TurnOffWhite();
      //parse the rgb data   
      ControlRGBWithText(data);          
    }
    if (data.indexOf("/W") >0) {
      TurnOffRGB();
      // parse the white led data
      ControlWhiteWithText(data);
    }

  }
  
  
}