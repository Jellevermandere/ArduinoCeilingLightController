void SetUpWhite()
{
  pinMode(WARM_WHITE_PIN, OUTPUT); 
  pinMode(COOL_WHITE_PIN, OUTPUT); 
  ControlWhite(1023,512);
  Serial.println("White is ready!");
}

//control the white led light by providing:
// brightnessInput:  0-1023
// temperatureInput: 0(cold)-1023(warm)
void ControlWhite(int brightnessInput, int temperatureInput)
{
  //set up the cool and warm temperatures
  int warmTemperature = temperatureInput;
  int coolTemperature = 1023-temperatureInput;

  // multiply them with the max brightness
  warmTemperature = map(warmTemperature, 0, 1023, 0, brightnessInput/4);
  coolTemperature = map(coolTemperature, 0, 1023, 0, brightnessInput/4);

  // write to the PWM signal
  analogWrite(WARM_WHITE_PIN, warmTemperature);
  analogWrite(COOL_WHITE_PIN, coolTemperature);

  Serial.flush();
  Serial.println("");
  Serial.print("WarmValue: ");
  Serial.print(warmTemperature);
  Serial.print("\t CoolValue: ");
  Serial.println(coolTemperature);
  delay(100);

}
// string format: "/GET {brightness},{temperature}/W"
void ControlWhiteWithText(String data)
{
  int firstIndex = data.indexOf('/');
  int nextIndex = data.indexOf(',', firstIndex);
  int brightness =  data.substring(firstIndex+1, nextIndex).toInt();
  int temperature = data.substring(nextIndex+1, data.length()-2).toInt();

  Serial.print("brightness: ");
  Serial.println(brightness);
  Serial.print("temperature: ");
  Serial.println(temperature);


  ControlWhite(brightness, temperature);

}
//todo add fade
void TurnOffWhite(){
  ControlWhite(0,0);
}
